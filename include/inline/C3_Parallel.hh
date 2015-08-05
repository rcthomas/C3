
#include <cstring>
#include <fstream>
#include <iomanip>
#include <map>

#include <omp.h>

#include "../C3_Exception.hh"
#include "../C3_FitsCreator.hh"
#include "../C3_FitsLoader.hh"
#include "../C3_MpiTraits.hh"

// Initialize.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::init( int& argc, char**& argv )
{

    // Initialization is broken into steps mostly for readability here.  Can't
    // generally be done in any order.  If you change this be sure to respect
    // when you can use world communicator and when you should not.

    _init_mpi( argc, argv );
    _init_world();
    _init_hostname();
    _init_mpi_processes_per_node();
    _init_frame_unpacked(); // TBD: packed frame comm, see below.
    _init_exposure();
    _init_node();
    _init_openmp();

    //

    if( frame_comm().root() )
    {

        std::ifstream stream( argv[ 1 ] ); // std::ifstream in(filename, std::ios::in | std::ios::binary);
        if( ! stream ) throw 123; // FIXME actual exception

        stream.seekg( 0, std::ios::end );

        int size = stream.tellg();
        size ++;
        C3::OwnedBlock< char > buffer( size );

        stream.seekg( 0, std::ios::beg );
        stream.read( buffer.data(), buffer.size() - 1 );
        stream.close();

        buffer[ size - 1 ] = '\0';

        int status = MPI_Bcast( &size, 1, MPI_INT, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        status = MPI_Bcast( buffer.data(), size, MPI_CHAR, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        _config = YAML::Load( buffer.data() );

    }
    else
    {
        
        int size   = 0;
        int status = MPI_Bcast( &size, 1, MPI_INT, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );

        C3::OwnedBlock< char > buffer( size );
        status = MPI_Bcast( buffer.data(), size, MPI_CHAR, 0, frame_comm().comm() );
        C3::assert_mpi_status( status );
       
        _config = YAML::Load( buffer.data() );

    }

    // Initiate logging.

    if( _config[ "logger" ] )
    {

        const YAML::Node& node = _config[ "logger" ];

        C3::LogLevel level = C3::LogLevel::DEFAULT;
        if( node[ "level" ] ) level = C3::loglevel_enum( node[ "level" ].template as< std::string >() );

        std::string path( "." );
        if( node[ "path" ] ) path = node[ "path" ].template as< std::string >(); // default...
        if( path.back() != '/' ) path += "/";

        std::string fullprefix = path + "default";
        if( node[ "prefix" ] ) fullprefix = path + node[ "prefix" ].template as< std::string >();

        std::stringstream ss;
        ss.fill( '0' );
        ss.width( 3 );
        ss << exposure_lane();
        _logger.reset( new C3::FileLogger( fullprefix + "." + ss.str() + "." + frame() + ".log", level ) );
        
    }

    // Parse other arguments into list of task files.

    for( auto i = 2; i < argc; ++ i ) _task_files.push( argv[ i ] );
    _task_position = 0;

}

// Executed on exit, this shuts down MPI too.

template< class InstrumentTraits >
inline int C3::Parallel< InstrumentTraits >::finalize()
{
    int status = MPI_Finalize();
    C3::assert_mpi_status( status );
    return EXIT_SUCCESS;
}

// Returns the exposure's next task in the stream.

template< class InstrumentTraits >
inline YAML::Node C3::Parallel< InstrumentTraits >::next_task()
{

    if( _tasks.size() == 0 && _task_files.size() > 0 )
    {

        std::string task_file = _task_files.front();
        _task_files.pop();

        if( exposure_comm().root() )
        {

            std::ifstream stream( task_file.c_str() ); // std::ifstream in(filename, std::ios::in | std::ios::binary);
            if( ! stream ) throw 123; // FIXME actual exception

            stream.seekg( 0, std::ios::end );

            int size = stream.tellg();
            size ++;
            C3::OwnedBlock< char > buffer( size );

            stream.seekg( 0, std::ios::beg );
            stream.read( buffer.data(), buffer.size() - 1 );
            stream.close();

            buffer[ size - 1 ] = '\0';

            int status = MPI_Bcast( &size, 1, MPI_INT, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            status = MPI_Bcast( buffer.data(), size, MPI_CHAR, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            std::vector< YAML::Node > tasks = YAML::LoadAll( buffer.data() );
            for( auto& task : tasks ) if( _task_position ++ % exposure_lanes() == exposure_lane() ) _tasks.push( std::move( task ) );

        }
        else
        {

            int size   = 0;
            int status = MPI_Bcast( &size, 1, MPI_INT, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );

            C3::OwnedBlock< char > buffer( size );
            status = MPI_Bcast( buffer.data(), size, MPI_CHAR, 0, exposure_comm().comm() );
            C3::assert_mpi_status( status );
            
            std::vector< YAML::Node > tasks = YAML::LoadAll( buffer.data() );
            for( auto& task : tasks ) if( _task_position ++ % exposure_lanes() == exposure_lane() ) _tasks.push( std::move( task ) );

        }

    }

    YAML::Node task = _tasks.front(); 
    _tasks.pop();
    return task;

}

// Load frame.

template< class InstrumentTraits >
template< class T >
void C3::Parallel< InstrumentTraits >::load( C3::Frame< T >& frame, const std::string& path )
{
    // Musical chairs loader...
    C3::FitsLoader loader( path );
    loader( frame.block(), this->frame() );
}

// Save frame tuple.

template< class InstrumentTraits >
template< class T, class U >
void C3::Parallel< InstrumentTraits >::save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path )
{

    int  naxis = 2;
    long naxes[ 2 ] { output.ncolumns(), output.nrows() };

    if( exposure_comm().root() )
    {

        C3::FitsCreator creator( path );
        logger().debug( "Writing to", path );
       
        logger().debug( "... frame", this->frame() );
        creator( output.block(), this->frame(), naxis, naxes );
        creator( invvar.block(), this->frame() + "_INVVAR", naxis, naxes );
        creator(  flags.block(), this->frame() + "_FLAGS" , naxis, naxes );

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

///// // Save frame.
///// 
///// template< class InstrumentTraits >
///// template< class T >
///// void C3::Parallel< InstrumentTraits >::save( C3::Frame< T >& output, const std::string& path )
///// {
/////     // FIXME TBD
///// }

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

}

// Determine OpenMP threads per MPI process.

template< class InstrumentTraits >
inline void C3::Parallel< InstrumentTraits >::_init_openmp()
{

    // Suggests utilization of all threads per MPI process.

    _threads_per_mpi_process = omp_get_max_threads();

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
