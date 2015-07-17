
#include <cstring>
#include <iomanip>
#include <map>

#include <omp.h>

#include "../C3_Exception.hh"

// Start-up access point.

template< class DetectorPolicy >
inline C3::Environment< DetectorPolicy >& C3::Environment< DetectorPolicy >::instance( int& argc, char**& argv )
{
    C3::Environment< DetectorPolicy >& env = C3::Environment< DetectorPolicy >::instance();
    env._init( argc, argv );
    return env;
}

// General access point.  Meyers singleton.

template< class DetectorPolicy >
inline C3::Environment< DetectorPolicy >& C3::Environment< DetectorPolicy >::instance()
{
    static C3::Environment< DetectorPolicy > env;
    return env;
}

// Tabular summary of the processing environment.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::summary( std::ostream& stream )
{

    std::stringstream ss;
    ss << std::left;

    ss << std::setw( 7 ) << world().size();
    ss << std::setw( 1 ) << ( world().root() ? "*" : " " );
    ss << std::setw( 7 ) << world().rank();
    ss << std::setw( 7 ) << frame().size();
    ss << std::setw( 1 ) << ( frame().root() ? "*" : " " );
    ss << std::setw( 7 ) << frame().rank();
    ss << std::setw( 7 ) << exposure().size();
    ss << std::setw( 1 ) << ( exposure().root() ? "*" : " " );
    ss << std::setw( 7 ) << exposure().rank();
    ss << std::setw( 7 ) << node().size();
    ss << std::setw( 1 ) << ( node().root() ? "*" : " " );
    ss << std::setw( 7 ) << node().rank();
    ss << std::setw( 7 ) << _exposure_lanes;
    ss << std::setw( 7 ) << _exposure_lane;
    ss << std::setw( 6 ) << _mpi_processes_per_node;
    ss << std::setw( 8 ) << _threads_per_mpi_process;
    ss << _hostname;

    std::string string = ss.str();

    int size = 120;
    string.resize( size );

    char* buffer = new char [ size * frame().size() ];
    memset( buffer, 0, size * frame().size() );

    int status = MPI_Gather( string.c_str(), size, MPI_CHAR, buffer, size, MPI_CHAR, 0, frame().comm() );
    C3::MPI::assert_status( status );

    if( frame().root() )
    {

        stream << std::setfill( '=' ) << std::setw( 120 ) << "" << std::setfill( ' ' ) << std::endl;

        stream << std::left;
        stream << std::setw( 8 ) << "world";
        stream << std::setw( 7 ) << "world";
        stream << std::setw( 8 ) << "frame";
        stream << std::setw( 7 ) << "frame";
        stream << std::setw( 8 ) << "expo";
        stream << std::setw( 7 ) << "expo";
        stream << std::setw( 8 ) << "node";
        stream << std::setw( 7 ) << "node";
        stream << std::setw( 7 ) << "expo";
        stream << std::setw( 7 ) << "expo";
        stream << std::setw( 6 ) << "MPI/";
        stream << std::setw( 8 ) << "OpenMP/";
        stream << std::endl;

        stream << std::left;
        stream << std::setw( 8 ) << "size";
        stream << std::setw( 7 ) << "rank";
        stream << std::setw( 8 ) << "size";
        stream << std::setw( 7 ) << "rank";
        stream << std::setw( 8 ) << "size";
        stream << std::setw( 7 ) << "rank";
        stream << std::setw( 8 ) << "size";
        stream << std::setw( 7 ) << "rank";
        stream << std::setw( 7 ) << "lanes";
        stream << std::setw( 7 ) << "lane";
        stream << std::setw( 6 ) << "node";
        stream << std::setw( 8 ) << "MPI";
        stream << "hostname";
        stream << std::endl;

        stream << std::setfill( '-' ) << std::setw( 120 ) << "" << std::setfill( ' ' ) << std::endl;

        for( int rank = 0; rank < frame().size(); ++ rank ) stream << buffer + rank * size << std::endl;

        stream << std::setfill( '=' ) << std::setw( 120 ) << "" << std::setfill( ' ' ) << std::endl;

    }

    delete [] buffer;

}

// Sweet dreams MPI.

template< class DetectorPolicy >
inline int C3::Environment< DetectorPolicy >::finalize()
{
    int status = MPI_Finalize();
    C3::MPI::assert_status( status );
    return EXIT_SUCCESS;
}

// Initialize singleton.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init( int& argc, char**& argv )
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

}

// Usual MPI launch.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_mpi( int& argc, char**& argv )
{
    int status = MPI_Init( &argc, &argv );
    C3::MPI::assert_status( status );
}

// World communicator wrapper.  Probably should not be used after start-up.
// Frame, exposure, or node communicators should be used instead.  Frame has
// all MPI processes active after start-up, should be used as a replacement for
// world from then on.  So, world still exists, just don't touch it.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_world()
{
    _world.reset( new C3::Communicator( MPI_COMM_WORLD ) );
}

// Configure hostname of this MPI process.  This should be the same for every
// MPI process running on the same node.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_hostname()
{

    char hostname[ MPI_MAX_PROCESSOR_NAME ];
    int length;

    // Alternative would be gethostbybame().  Linux MPICH uses it already.
    // But that's just one implementation on one platform.

    int status = MPI_Get_processor_name( hostname, &length );
    C3::MPI::assert_status( status );

    _hostname = hostname;

}

// MPI processes per node in world at start-up.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_mpi_processes_per_node()
{

    // Count occurrence of each hostname in world.

    std::map< std::string, int > count;

    std::vector< std::string > hostnames = _gather_hostnames( world() );
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

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_frame_unpacked()
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
    C3::MPI::assert_status( status );

    // See what MPI processes are frame MPI processes and what exposure lanes
    // they will belong to.

    std::vector< int  > exposure_lane;
    std::vector< bool > frame_mpi_process;

    for( int rank = 0; rank < world().size(); ++ rank )
    {
        exposure_lane.push_back    ( rank / mpi_processes_per_exposure_lane                         );
        frame_mpi_process.push_back( rank % mpi_processes_per_exposure_lane < exposure_lane_width() );
    }

    // User might launch a number of MPI processes that ends up creating what
    // would end up a partial exposure lane.  MPI processes in such a partial
    // exposure lane are marked as surplus to deactivate.

    std::map< int, int > exposure_lane_count;

    for( int rank = 0; rank < world().size(); ++ rank )
    {
        if( ! frame_mpi_process[ rank ] ) continue;
        exposure_lane_count[ exposure_lane[ rank ] ] ++;
    }

    for( auto it = exposure_lane_count.begin(); it != exposure_lane_count.end(); ++ it )
    {
        if( it->second == exposure_lane_width() ) continue;
        for( int rank = 0; rank < world().size(); ++ rank )
        {
            if( exposure_lane[ rank ] != it->first ) continue;
            frame_mpi_process[ rank ] = false;
        }
    }

    // Gather all frame MPI processes from complete exposure lanes and put them
    // in new "frame" group.

    std::vector< int > ranks;
    for( int rank = 0; rank < world().size(); ++ rank )
    {
        if( ! frame_mpi_process[ rank ] ) continue;
        ranks.push_back( rank );
    }

    MPI_Group frame_group;
    status = MPI_Group_incl( world_group, ranks.size(), ranks.data(), &frame_group );
    C3::MPI::assert_status( status );

    // Form unpacked frame communicator from frame group and wrap it.

    MPI_Comm frame;
    status = MPI_Comm_create( world().comm(), frame_group, &frame );
    C3::MPI::assert_status( status );

    // Release all MPI processes not in frame communicator.  Frame communicator
    // is now effectively the "world" communicator to use from now on.

    if( frame == MPI_COMM_NULL ) exit( finalize() );

    // Frame communicator wrapper.

    _frame.reset( new C3::Communicator( frame ) );

}

// Configure packed frame communicator (exposures can share nodes).

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_frame_packed()
{

    // Haven't worried about this yet, probably matters more for Bok, SNIFS...
    // When we write this method we should examine the unpacked configuration
    // code and factor common things out, it is kind of long.  One thought I
    // have had is that packed/unpacked should be a policy template argument.

    throw C3::Exception( "Packed frame communicator not implemented yet." );

}

// Exposure (exposure-lane) communicators, exposure lane setup.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_exposure()
{

    // MPI_Comm_split frame communicator to make exposure-lane communicators.

    _exposure_lanes = frame().size() / exposure_lane_width();
    _exposure_lane  = frame().rank() / exposure_lane_width();  // split "color"
    int key         = frame().rank() % exposure_lane_width();

    MPI_Comm exposure;
    int status = MPI_Comm_split( frame().comm(), _exposure_lane, key, &exposure );
    C3::MPI::assert_status( status );

    // Exposure lane communicator wrapper.

    _exposure.reset( new C3::Communicator( exposure ) );

}

// Node communicators.  These are split from exposure communicators, not the
// frame communicator, so there is a communicator for each node within an
// exposure lane.  That is, there may be more than one node communicator per
// node, but they correspond to different exposure lanes.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_node()
{

    // Index unique hostnames in exposure communicator.

    std::map< std::string, int > index;

    int count = 0; 
    std::vector< std::string > hostnames = _gather_hostnames( exposure().comm() );
    for( auto it = hostnames.begin(); it != hostnames.end(); ++ it )
    {
        if( index.find( *it ) != index.end() ) continue;
        index[ *it ] = count ++;
    }

    // Split on unique hostname.

    int color = index[ _hostname ];
    int key   = exposure().rank();  // FIXME verify the only thing that matters is the ORDER in determining the rank on the new comm.

    MPI_Comm node;
    int status = MPI_Comm_split( exposure().comm(), color, key, &node );
    C3::MPI::assert_status( status );

    // Node communicator wrapper.

    _node.reset( new C3::Communicator( node ) );

}

// Determine OpenMP threads per MPI process.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_openmp()
{

    // Suggests utilization of all threads per MPI process.

    _threads_per_mpi_process = omp_get_max_threads();

}

// Hostname of each MPI process in communicator in rank order.

template< class DetectorPolicy >
inline std::vector< std::string > C3::Environment< DetectorPolicy >::_gather_hostnames( const C3::Communicator& comm )
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
    C3::MPI::assert_status( status );

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
