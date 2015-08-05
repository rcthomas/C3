
#include <cstring>
#include <fstream>
#include <iomanip>
#include <map>

#include <omp.h>

#include "../C3_Exception.hh"
#include "../C3_FitsCreator.hh"
#include "../C3_FitsLoader.hh"
#include "../C3_MpiTraits.hh"

// Initialize command line, config, validation, tasks, etc.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::init( int& argc, char**& argv )
{

    // Initialization is broken into steps mostly for readability here.  Can't
    // generally be done in any order.  If you change this be sure to respect
    // when you can use world communicator and when you should not.

    _init_mpi( argc, argv );
    _init_world();
    _validate_command_line( argc, argv );
    _init_hostname();
    _init_mpi_processes_per_node();
    _init_frame_unpacked(); // TBD: Packed frame comm? See below.
    _init_config();
    _init_logger();
    _init_exposure();
    _init_node();
    _init_openmp();
    _init_task_queue( argc, argv );

    logger().info( "Parallel context initialization complete." );

}

// Executed on exit, this shuts down MPI too.

template< class InstrumentTraits >
inline int C3::Parallel< InstrumentTraits >::finalize()
{
    logger().info( "Parallel context finalizing." );
    int status = MPI_Finalize();
    C3::assert_mpi_status( status );
    logger().info( "Goodbye!" );
    return EXIT_SUCCESS;
}

// Grab next task from stream.  If task queue is exhausted but task files
// remain, replenish queue with tasks from next task file.  Note the pattern
// here is the same as config initialization, except the communicator is the
// exposure-lane communicator.  Exposure-lane communicator root reads the next
// task file and broadcasts it to all the other ranks in the exposure lane.
// Then all ranks in the exposure lane parse the task file into their task
// queues.  Finally, note that tasks are collected according to a round-robin
// rule.

template< class InstrumentTraits >
inline YAML::Node C3::Parallel< InstrumentTraits >::next_task()
{

    if( _tasks.size() == 0 && _task_files.size() > 0 )
    {

        std::string task_file = _task_files.front();
        _task_files.pop();

        logger().debug( "Enqueuing tasks from from:", task_file );

        if( exposure_comm().root() )
        {

            // Open config file, except if cannot.

            std::ifstream stream( task_file );
            if( ! stream ) throw C3::Exception::create( "Can't open task file:", task_file );

            // Measure file size and add one to account for null-terminator we add.

            stream.seekg( 0, std::ios::end );
            C3::size_type size = stream.tellg();
            size ++;

            // Allocate, rewind, fill buffer, close stream, add null-terminator.

            C3::OwnedBlock< char > buffer( size );
            stream.seekg( 0, std::ios::beg );
            stream.read( buffer.data(), buffer.size() - 1 );
            stream.close();
            buffer[ size - 1 ] = '\0';

            // Tell all other frame ranks how large a buffer to allocate.

            int status = MPI_Bcast( &size, 1, C3::MpiTraits< C3::size_type >::datatype, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            // Broadcast buffer contents.

            status = MPI_Bcast( buffer.data(), size, C3::MpiTraits< char >::datatype, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            // Parse tasks into queue, following round-robin rule.

            std::vector< YAML::Node > tasks = YAML::LoadAll( buffer.data() );
            for( auto& task : tasks ) if( _task_position ++ % exposure_lanes() == exposure_lane() ) _tasks.push( std::move( task ) );

        }
        else
        {

            // Wait to be told how large a buffer needs to be allocated.
            
            C3::size_type size = 0;
            int status = MPI_Bcast( &size, 1, C3::MpiType< C3::size_type >::datatype, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            // Allocate buffer and wait for content.

            C3::OwnedBlock< char > buffer( size );
            status = MPI_Bcast( buffer.data(), size, C3::MpiType< char >::datatype, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            // Parse tasks into queue, following round-robin rule.

            std::vector< YAML::Node > tasks = YAML::LoadAll( buffer.data() );
            for( auto& task : tasks ) if( _task_position ++ % exposure_lanes() == exposure_lane() ) _tasks.push( std::move( task ) );

        }

    }

    logger().debug( "Next task. Tasks currently in queue:", _tasks.size() );

    YAML::Node task = _tasks.front(); 
    _tasks.pop();
    return task;

}

// Load frame.  Every frame rank just opens the file, goes to its HDU, and
// loads it.  Not beautiful but until it breaks this is what we will use.

template< class InstrumentTraits >
template< class T >
void C3::Parallel< InstrumentTraits >::load( C3::Frame< T >& input, const std::string& path )
{

    logger().debug( "Loading frame", frame(), "from", path, "[START]" );

    C3::FitsLoader loader( path );
    loader( input.block(), frame() );

    logger().debug( "Loading frame", frame(), "from", path, "[DONE]" );

}

// Save frame tuple.
// FIXME more debug messages in here.

template< class InstrumentTraits >
template< class T, class U >
void C3::Parallel< InstrumentTraits >::save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path )
{

    logger().debug( "Saving to", path, "[START]" );

    int  naxis = 2;
    long naxes[ 2 ] { output.ncolumns(), output.nrows() };

    if( exposure_comm().root() )
    {

        C3::FitsCreator creator( path );
       
        logger().debug( "... frame", frame() );
        creator( output.block(), frame(), naxis, naxes );
        creator( invvar.block(), frame() + "_INVVAR", naxis, naxes );
        creator(  flags.block(), frame() + "_FLAGS" , naxis, naxes );

        // FIXME check statuses.

        for( auto rank = 1; rank < exposure_comm().size(); ++ rank )
        {

            logger().debug( "... frame", InstrumentTraits::frames[ rank ] );

            MPI_Status status;
            MPI_Recv( naxes, naxis, C3::MpiType< long >::datatype, rank, 0, exposure_comm().comm(), &status );

            // Write out frame HDU and inverse variance HDU.

            {
                C3::OwnedBlock< T > tmp( naxes[ 0 ] * naxes[ 1 ] );
                MPI_Recv( tmp.data(), tmp.size(), C3::MpiType< T >::datatype, rank, 0, exposure_comm().comm(), &status );
                creator( tmp, InstrumentTraits::frames[ rank ], naxis, naxes );

                MPI_Recv( tmp.data(), tmp.size(), C3::MpiType< T >::datatype, rank, 0, exposure_comm().comm(), &status );
                creator( tmp, InstrumentTraits::frames[ rank ] + "_INVVAR", naxis, naxes );
            }

            // Write out the flags HDU.

            {
                C3::OwnedBlock< U > tmp( naxes[ 0 ] * naxes[ 1 ] );
                MPI_Recv( tmp.data(), tmp.size(), C3::MpiType< U >::datatype, rank, 0, exposure_comm().comm(), &status );
                creator( tmp, InstrumentTraits::frames[ rank ] + "_FLAGS", naxis, naxes );
            }

        }

    }
    else
    {
        /// FIXME Check statuses
        MPI_Send(                 naxes,                 naxis, C3::MpiType< long >::datatype, 0, 0, exposure_comm().comm() );
        MPI_Send( output.block().data(), output.block().size(), C3::MpiType< T    >::datatype, 0, 0, exposure_comm().comm() );
        MPI_Send( invvar.block().data(), invvar.block().size(), C3::MpiType< T    >::datatype, 0, 0, exposure_comm().comm() );
        MPI_Send(  flags.block().data(),  flags.block().size(), C3::MpiType< U    >::datatype, 0, 0, exposure_comm().comm() );
    }

}

// Usual MPI launch.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_mpi( int& argc, char**& argv )
{
    int status = MPI_Init( &argc, &argv );
    C3::assert_mpi_status( status );
}

// World communicator wrapper.  Probably should not be used after start-up.
// Frame, exposure, or node communicators should be used instead.  Frame has
// all MPI processes active after start-up, should be used as a replacement for
// world from then on.  So, world still exists, just don't touch it.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_world()
{
    _world_comm.reset( new C3::Communicator( MPI_COMM_WORLD ) );
}

// Validate command line.  World root throws exception if looks wrong.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_validate_command_line( int& argc, char**& argv )
{
    if( world_comm().root() && argc < 3 ) throw C3::Exception( "Bad command line. Need executable and at least two arguments." );
}

// Configure hostname of this MPI process.  This should be the same for every
// MPI process running on the same node.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_hostname()
{

    char hostname[ MPI_MAX_PROCESSOR_NAME ];
    int length;

    // Alternative would be gethostbybame().  Linux MPICH uses it already.
    // But that's just one implementation on one platform.

    int status = MPI_Get_processor_name( hostname, &length );
    C3::assert_mpi_status( status );

    _hostname = hostname;

}

// MPI processes per node in world at start-up.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_mpi_processes_per_node()
{

    // Count occurrence of each hostname in world.

    std::map< std::string, int > count;

    std::vector< std::string > hostnames = _gather_hostnames( world_comm() );
    for( auto it = hostnames.begin(); it != hostnames.end(); ++ it ) count[ *it ] ++;

    // Abort if MPI processes per node is not same for each node.  Just how we
    // roll for now.  Could be done different in future.

    for( auto it = count.begin(); it != count.end(); ++ it )
    {
        if( it->second == count[ _hostname ] ) continue;
        throw C3::Exception( "MPI processes per node not uniform." );
    }

    _mpi_processes_per_node = count[ _hostname ];

}

// Configure unpacked frame communicator (exposures do not share nodes).
// Assign whole nodes to exposures and if there are surplus MPI processes,
// deactivate them.  Call MPI_Finalize and exit on them.  Technique from
// http://stackoverflow.com/a/13777288.  After this point it is no longer a
// good idea to use world.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_frame_unpacked()
{

    // Number of nodes assigned to an exposure lane is exposure lane width in
    // active MPI processes divided by MPI processes per node, rounded up.
    // Exposure lane width probably, not necessarily, corresponds to the number
    // of frames per exposure depending on detector definition.  Surplus MPI
    // processes on the last node will MPI_Finalize and exit.
    
    int nodes_per_exposure_lane = ( exposure_lane_width() + ( _mpi_processes_per_node - 1 ) ) / _mpi_processes_per_node;

    // Total processes per exposure lane includes active MPI frame processes
    // and surplus MPI processes to deactivate.

    int mpi_processes_per_exposure_lane = nodes_per_exposure_lane * _mpi_processes_per_node;

    // World group.  This will be pruned down to frame MPI processes to keep in
    // unpacked layout.

    MPI_Group world_group;
    int status = MPI_Comm_group( MPI_COMM_WORLD, &world_group );
    C3::assert_mpi_status( status );

    // See what MPI processes are frame MPI processes and what exposure lanes
    // they will belong to.

    std::vector< int  > exposure_lane;
    std::vector< bool > frame_mpi_process;

    for( int rank = 0; rank < world_comm().size(); ++ rank )
    {
        exposure_lane.push_back    ( rank / mpi_processes_per_exposure_lane                         );
        frame_mpi_process.push_back( rank % mpi_processes_per_exposure_lane < exposure_lane_width() );
    }

    // User might launch a number of MPI processes that ends up creating what
    // would end up a partial exposure lane.  MPI processes in such a partial
    // exposure lane are marked as surplus to deactivate.

    std::map< int, int > exposure_lane_count;

    for( int rank = 0; rank < world_comm().size(); ++ rank )
    {
        if( ! frame_mpi_process[ rank ] ) continue;
        exposure_lane_count[ exposure_lane[ rank ] ] ++;
    }

    for( auto it = exposure_lane_count.begin(); it != exposure_lane_count.end(); ++ it )
    {
        if( it->second == exposure_lane_width() ) continue;
        for( int rank = 0; rank < world_comm().size(); ++ rank )
        {
            if( exposure_lane[ rank ] != it->first ) continue;
            frame_mpi_process[ rank ] = false;
        }
    }

    // Gather all frame MPI processes from complete exposure lanes and put them
    // in new "frame" group.

    std::vector< int > ranks;
    for( int rank = 0; rank < world_comm().size(); ++ rank )
    {
        if( ! frame_mpi_process[ rank ] ) continue;
        ranks.push_back( rank );
    }

    MPI_Group frame_group;
    status = MPI_Group_incl( world_group, ranks.size(), ranks.data(), &frame_group );
    C3::assert_mpi_status( status );

    // Form unpacked frame communicator from frame group and wrap it.

    MPI_Comm frame;
    status = MPI_Comm_create( world_comm().comm(), frame_group, &frame );
    C3::assert_mpi_status( status );

    // Release all MPI processes not in frame communicator.  Frame communicator
    // is now effectively the "world" communicator to use from now on.

    if( frame == MPI_COMM_NULL ) exit( finalize() );

    // Frame communicator wrapper.

    _frame_comm.reset( new C3::Communicator( frame ) );

}

// Configure packed frame communicator (exposures can share nodes).

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_frame_packed()
{

    // Haven't worried about this yet, probably matters more for Bok, SNIFS...
    // When we write this method we should examine the unpacked configuration
    // code and factor common things out, it is kind of long.  One thought I
    // have had is that packed/unpacked should be a policy template argument.

    throw C3::Exception( "Packed frame communicator not implemented yet." );

}

// Parse config.  Exception if it can't be read or parsed.  

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_config( int& argc, char**& argv )
{

    // Frame communicator root reads config as plain text and broadcasts it to
    // all frame ranks.  Each frame rank including root parses the config.
    // There is no point parsing the config at frame root, serializing it, and
    // then parsing it again.

    if( frame_comm().root() )
    {

        // Open config file, except if cannot.

        std::ifstream stream( argv[ 1 ] );
        if( ! stream ) throw C3::Exception::create( "Can't open config:", argv[ 1 ] );

        // Measure file size and add one to account for null-terminator we add.

        stream.seekg( 0, std::ios::end );
        C3::size_type size = stream.tellg();
        size ++;

        // Allocate, rewind, fill buffer, close stream, add null-terminator.

        C3::OwnedBlock< char > buffer( size );
        stream.seekg( 0, std::ios::beg );
        stream.read( buffer.data(), buffer.size() - 1 );
        stream.close();
        buffer[ size - 1 ] = '\0';

        // Tell all other frame ranks how large a buffer to allocate.

        int status = MPI_Bcast( &size, 1, C3::MpiType< C3::size_type >::datatype, 0, frame_comm().comm() ); // does this work?
        C3::assert_mpi_status( status );

        // Broadcast buffer contents.

        status = MPI_Bcast( buffer.data(), size, C3::MpiType< char >::datatype, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        // Parse config.

        _config = YAML::Load( buffer.data() );

    }
    else
    {

        // Wait to be told how large a buffer needs to be allocated.
        
        C3::size_type size = 0;
        int status = MPI_Bcast( &size, 1, C3::MpiType< C3::size_type >::datatype, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        // Allocate buffer and wait for content.

        C3::OwnedBlock< char > buffer( size );
        status = MPI_Bcast( buffer.data(), size, C3::MpiType< char >::datatype, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        // Parse config.
       
        _config = YAML::Load( buffer.data() );

    }

}

// Initiate file logger.  If the config doesn't contain a logger then we set up
// file-based logger with a default path and prefix.

template< class InstrumentTraits >
inline std::vector< std::string > C3::Parallel< InstrumentTraits >::_init_logger()
{

    // Shortcut to node.

    const YAML::Node& node = _config[ "logger" ];

    // Minimum log message level.  Default from Loglevel enum class definition.

    C3::LogLevel level = C3::LogLevel::DEFAULT;
    if( node[ "loglevel" ] ) level = C3::LogLevel_enum( node[ "loglevel" ].template as< std::string >() );

    // Log-file path, default is current working directory (dot).  Trailing
    // slash added if necessary.

    std::string path( "." );
    if( node[ "path" ] ) path = node[ "path" ].template as< std::string >(); // default...
    if( path.back() != '/' ) path += "/";

    // Concatenate path, prefix, exposure lane, frame identifier, and log-file
    // suffix to create logger path.

    std::string fullprefix = path + "default";
    if( node[ "prefix" ] ) fullprefix = path + node[ "prefix" ].template as< std::string >();

    std::stringstream ss;
    ss.fill( '0' );
    ss.width( 3 );
    ss << exposure_lane();

    _logger.reset( new C3::FileLogger( fullprefix + "." + ss.str() + "." + frame() + ".log", level ) );

    // Debug messages.

    logger().debug( "Logger initiated for frame:", frame() );
    logger().debug( "Frame    communicator rank / size :", frame_comm().rank(), "/", frame_comm().size() );

}

// Exposure (exposure-lane) communicators, exposure lane setup.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_exposure()
{

    // MPI_Comm_split frame communicator to make exposure-lane communicators.

    _exposure_lanes = frame_comm().size() / exposure_lane_width();
    _exposure_lane  = frame_comm().rank() / exposure_lane_width();  // split "color"
    int key         = frame_comm().rank() % exposure_lane_width();

    MPI_Comm exposure;
    int status = MPI_Comm_split( frame_comm().comm(), _exposure_lane, key, &exposure );
    C3::assert_mpi_status( status );

    // Exposure lane communicator wrapper.

    _exposure_comm.reset( new C3::Communicator( exposure ) );

    // Debug messages.

    logger().debug( "Exposure lane       index / count:", exposure_lane(), "/", exposure_lanes() );
    logger().debug( "Exposure communicator rank / size:", exposure_comm().rank(), "/", exposure_comm().size() );

}

// Node communicators.  These are split from exposure communicators, not the
// frame communicator, so there is a communicator for each node within an
// exposure lane.  That is, there may be more than one node communicator per
// node, but they correspond to different exposure lanes.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_node()
{

    // Index unique hostnames in exposure communicator.

    std::map< std::string, int > index;

    int count = 0; 
    std::vector< std::string > hostnames = _gather_hostnames( exposure_comm() );
    for( auto it = hostnames.begin(); it != hostnames.end(); ++ it )
    {
        if( index.find( *it ) != index.end() ) continue;
        index[ *it ] = count ++;
    }

    // Split on unique hostname.

    int color = index[ _hostname ];
    int key   = exposure_comm().rank();  // FIXME verify the only thing that matters is the ORDER in determining the rank on the new comm.

    MPI_Comm node;
    int status = MPI_Comm_split( exposure_comm().comm(), color, key, &node );
    C3::assert_mpi_status( status );

    // Node communicator wrapper.

    _node_comm.reset( new C3::Communicator( node ) );

    // Debug messages.

    logger().debug( "Node     communicator rank / size:", node_comm().rank(), "/", node_comm().size() );

}

// Determine OpenMP threads per MPI process.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_openmp()
{

    // Suggests utilization of all threads per MPI process.

    _threads_per_mpi_process = omp_get_max_threads();

    // Debug messages.

    logger().debug( "OpenMP threads per MPI process   :", _threads_per_mpi_process );

}

// Parse other arguments into list of task files, and set the task position to
// zero.  We use this to round-robin tasks among exposure lanes.

template< class InstrumentTraits >
inline std::vector< std::string > C3::Parallel< InstrumentTraits >::_init_task_queue( argc, argv )
{
    for( auto i = 2; i < argc; ++ i ) _task_files.push( argv[ i ] );
    logger().debug( "Task files in queue:", _task_files.size() );
    _task_position = 0;
}

// Hostname of each MPI process in communicator in rank order.

template< class InstrumentTraits >
inline std::vector< std::string > C3::Parallel< InstrumentTraits >::_gather_hostnames( const C3::Communicator& comm )
{

    // Send/hostname buffer.

    char hostname[ MPI_MAX_PROCESSOR_NAME ];
    int  size = _hostname.copy( hostname, _hostname.size() );
    hostname[ size ] = '\0';

    // Receive buffer.

    int    recv_size = comm.size() * MPI_MAX_PROCESSOR_NAME;
    char*  recv      = new char [ recv_size ];
    memset( recv, 0, recv_size );

    // All-gather hostnames.

    int status = MPI_Allgather( hostname, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, recv, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, comm.comm() );
    C3::assert_mpi_status( status );

    // Construct vector.

    std::vector< std::string > hostnames;
    for( int i = 0; i < comm.size(); ++ i )
    {
        strcpy( hostname, recv + i * MPI_MAX_PROCESSOR_NAME );
        hostnames.push_back( hostname );
    }

    // Free receive buffer, return answer.

    delete [] recv;
    return hostnames;

}

