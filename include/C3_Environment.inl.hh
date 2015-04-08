
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include <omp.h>

#include "C3_Exception.hh"

// Raise an exception if MPI status is not MPI_SUCCESS.

void C3::assert_mpi_status( const int mpi_status )
{

    if( mpi_status == MPI_SUCCESS ) return;
   
    int error_class;
    MPI_Error_class( mpi_status, &error_class );

    char error_string[ MPI_MAX_ERROR_STRING ];
    int length;
    MPI_Error_string( mpi_status, error_string, &length );

    std::stringstream ss;
    ss << "MPI error code " << mpi_status;
    ss << " | error class " << error_class;
    ss << " | "             << error_string;

    throw C3::Exception( ss.str() );

}

// Start-up access point.

template< class DetectorPolicy >
inline C3::Environment< DetectorPolicy >& C3::Environment< DetectorPolicy >::instance( int& argc, char**& argv )
{
    if( ! _instance )
    {
        _instance = new C3::Environment< DetectorPolicy >();
        _instance->_init( argc, argv );
    }
    return *_instance;
}

// General access point.

template< class DetectorPolicy >
inline C3::Environment< DetectorPolicy >& C3::Environment< DetectorPolicy >::instance()
{
    return *_instance;
}

// FIXME COMMENT ME

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::summary( std::ostream& stream )
{

    std::stringstream ss;
    ss << std::left;

    ss << std::setw( 20 ) <<  "";
    ss << std::setw( 20 ) << "tasks";
    ss << std::setw( 20 ) << "task ID";
    ss << std::setw( 20 ) << "task root";
    ss << std::endl;

    ss << std::setw( 20 ) << "world";
    ss << std::setw( 20 ) << _world_tasks;
    ss << std::setw( 20 ) << _world_task_id;
    ss << std::setw( 20 ) << ( _world_task_root ? "yes" : "no" );
    ss << std::endl;

    ss << std::setw( 20 ) << "frame";
    ss << std::setw( 20 ) << _frame_tasks;
    ss << std::setw( 20 ) << _frame_task_id;
    ss << std::setw( 20 ) << ( _frame_task_root ? "yes" : "no" );
    ss << std::endl;

    ss << std::setw( 20 ) << "exposure";
    ss << std::setw( 20 ) << _exposure_tasks;
    ss << std::setw( 20 ) << _exposure_task_id;
    ss << std::setw( 20 ) << ( _exposure_task_root ? "yes" : "no" );
    ss << std::endl;

    ss << std::setw( 20 ) << "node";
    ss << std::setw( 20 ) << _node_tasks;
    ss << std::setw( 20 ) << _node_task_id;
    ss << std::setw( 20 ) << ( _node_task_root ? "yes" : "no" );
    ss << std::endl;

    ss << std::setw( 20 ) << "hostname"         << _hostname         << std::endl;
    ss << std::setw( 20 ) << "tasks per node"   << _tasks_per_node   << std::endl;
    ss << std::setw( 20 ) << "exposure lanes"   << _exposure_lanes   << std::endl;
    ss << std::setw( 20 ) << "exposure lane ID" << _exposure_lane_id << std::endl;
    ss << std::setw( 20 ) << "threads per task" << _threads_per_task << std::endl;

    std::string string = ss.str();

    int size = 10 * 80;
    string.resize( size );

    char* buffer = new char [ size * _frame_tasks ];
    memset( buffer, 0, size * _frame_tasks );

    int status = MPI_Gather( string.c_str(), size, MPI_CHAR, buffer, size, MPI_CHAR, 0, _frame_comm );
    assert_mpi_status( status );

    if( _frame_task_root )
    {
        for( int i = 0; i < _frame_tasks; ++ i )
        {
            stream << "--------------------------------------------------------------------------------" << std::endl;
            stream << buffer + i * size;
        }
        stream << "--------------------------------------------------------------------------------" << std::endl;
    }

    delete [] buffer;

}

// Finalize MPI.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::finalize()
{
    int status = MPI_Finalize();
    assert_mpi_status( status );
}

// Initialize singleton environment instance at application start-up.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init( int& argc, char**& argv )
{

    // Initialization is broken into steps mostly for readability here.  They 
    // generally can't be done in any order, so if you change this be sure to
    // respect when you can use MPI_COMM_WORLD and when you should not.

    _init_mpi_world( argc, argv );
    _init_hostname();
    _init_tasks_per_node();
    _init_unpacked_frame_comm(); // TBD: packed frame comm, see below.
    _init_exposure_comms();
    _init_node_comms();
    _init_openmp();

}

// MPI world communicator information.  In general MPI_COMM_WORLD should not be
// used after initialization.  Rather, the appropriate frame, exposure, or node
// communicator should be used instead.  The Frame communicator includes all
// the MPI tasks active after initialization, and should be used as a sort of
// replacement for MPI_COMM_WORLD from that point forward.  It still exists, 
// just don't touch it.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_mpi_world( int& argc, char**& argv )
{

    // Initialize MPI in the usual way.
    
    int status = MPI_Init( &argc, &argv );
    assert_mpi_status( status );
    
    // Total MPI tasks in world communicator.
    
    status = MPI_Comm_size( MPI_COMM_WORLD, &_world_tasks );
    assert_mpi_status( status );
    
    // MPI rank of this process in world communicator.
    
    status = MPI_Comm_rank( MPI_COMM_WORLD, &_world_task_id );
    assert_mpi_status( status );

    // If MPI rank is zero, this is root task on world communicator.

    _world_task_root = _world_task_id == 0;

}

// Configure hostname of this MPI process.  This should be the same for every
// MPI process running on the same node.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_hostname()
{

    char hostname[ MPI_MAX_PROCESSOR_NAME ];
    int length;

    // For now we use MPI_Get_processor_name but this may not always work.  An
    // alternative would be gethostbyname(), but it is a bit more complicated.

    int status = MPI_Get_processor_name( hostname, &length );
    assert_mpi_status( status );

    _hostname = hostname;

}

// Number of tasks per node in world communicator at start-up.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_tasks_per_node()
{

    // Count occurrence of each hostname in the world communicator.

    std::map< std::string, int > task_counts;

    std::vector< std::string > hostnames = _gather_hostnames( MPI_COMM_WORLD, _world_tasks );
    for( auto it = hostnames.begin(); it != hostnames.end(); ++ it ) task_counts[ *it ] ++;

    // Abort launch if the number of tasks per node is not the same for each
    // node.  In principle, this requirement could be relaxed.

    for( auto it = task_counts.begin(); it != task_counts.end(); ++ it )
    {
        if( it->second == task_counts[ _hostname ] ) continue;
        C3::Exception( "Tasks per node is not uniform." );
    }

    _tasks_per_node = task_counts[ _hostname ];

}

// Hostname of each task in an MPI communicator in rank order.

template< class DetectorPolicy >
inline std::vector< std::string > C3::Environment< DetectorPolicy >::_gather_hostnames( const MPI_Comm& comm, const int tasks )
{

    // Send/hostname buffer.

    char hostname[ MPI_MAX_PROCESSOR_NAME ];
    int  size = _hostname.copy( hostname, _hostname.size() );
    hostname[ size ] = '\0';

    // Receive buffer.  Documentation on C++11 std::string says "modifying the
    // character array accessed through data is undefined behavior" so we stick
    // to C-style char array sadly.

    int    recv_size = tasks * MPI_MAX_PROCESSOR_NAME;
    char*  recv      = new char [ recv_size ];
    memset( recv, 0, recv_size );

    // All-gather the hostnames.

    int status = MPI_Allgather( hostname, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, recv, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, comm );
    assert_mpi_status( status );

    // Construct vector.

    std::vector< std::string > hostnames;
    for( int i = 0; i < tasks; ++ i )
    {
        strcpy( hostname, recv + i * MPI_MAX_PROCESSOR_NAME );
        hostnames.push_back( hostname );
    }

    // Free receive buffer, return answer.

    delete [] recv;
    return hostnames;

}

// Configure unpacked frame communicator (exposures do not share nodes).  What
// happens here is that we assign whole nodes to exposures and if there are
// left-over MPI tasks we deactivate them (they call MPI_Finalize and exit).
// Technique from http://stackoverflow.com/a/13777288.  After this point it is
// no longer a good idea to use MPI_COMM_WORLD.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_unpacked_frame_comm()
{

    // Number of nodes assigned to an exposure lane is the number of frame
    // tasks divided by tasks per node, rounded up.  The surplus tasks on
    // the last node will MPI_Finalize and exit.
    
    int nodes_per_exposure_lane = ( frame_tasks_per_exposure_lane() + ( _tasks_per_node - 1 ) ) / _tasks_per_node;

    // Total tasks per exposure lane includes frame tasks and surplus tasks
    // that we will deactivate.

    int tasks_per_exposure_lane = nodes_per_exposure_lane * _tasks_per_node;

    // Construct world group.  We will prune this down to the frame tasks
    // we want to keep in an unpacked layout.

    MPI_Group world_group;
    int status = MPI_Comm_group( MPI_COMM_WORLD, &world_group );
    assert_mpi_status( status );

    // Model which tasks are frame tasks and what exposure lanes they will
    // belong to.

    std::vector< int  > exposure_lane_id;
    std::vector< bool > is_frame_task;

    for( int i = 0; i < _world_tasks; ++ i )
    {
        exposure_lane_id.push_back( i / tasks_per_exposure_lane                                   );
        is_frame_task.push_back   ( i % tasks_per_exposure_lane < frame_tasks_per_exposure_lane() );
    }

    // Estimate the number of frame tasks per exposure lane.  The user might
    // request a number of tasks for the environment that ends up creating what
    // would be a partial exposure lane (math too hard user lazy, forgive).  In
    // that case the frame tasks in that exposure lane are marked as surplus to
    // be deactivated.

    std::map< int, int > exposure_lane_count;

    for( int i = 0; i < _world_tasks; ++ i )
    {
        if( ! is_frame_task[ i ] ) continue;
        exposure_lane_count[ exposure_lane_id[ i ] ] ++;
    }

    for( auto it = exposure_lane_count.begin(); it != exposure_lane_count.end(); ++ it )
    {
        if( it->second == frame_tasks_per_exposure_lane() ) continue;
        for( int i = 0; i < _world_tasks; ++ i )
        {
            if( exposure_lane_id[ i ] != it->first ) continue;
            is_frame_task[ i ] = false;
        }
    }

    // Gather all frame ranks from complete exposure lanes and put them into a
    // new "frame" group.

    std::vector< int > ranks;
    for( int i = 0; i < _world_tasks; ++ i )
    {
        if( ! is_frame_task[ i ] ) continue;
        ranks.push_back( i );
    }

    MPI_Group frame_group;
    status = MPI_Group_incl( world_group, ranks.size(), ranks.data(), &frame_group );
    assert_mpi_status( status );

    // Form unpacked frame communicator from frame group.

    status = MPI_Comm_create( MPI_COMM_WORLD, frame_group, &_frame_comm );
    assert_mpi_status( status );

    // Release all tasks not in frame communicator.  The frame communicator is
    // effectively the new "world" communicator to use from now on.

    if( _frame_comm == MPI_COMM_NULL )
    {
        finalize();
        exit( 0 );
    }

    // Total MPI tasks in frame communicator.
    
    status = MPI_Comm_size( _frame_comm, &_frame_tasks );
    assert_mpi_status( status );
    
    // MPI rank of this process in frame communicator.
    
    status = MPI_Comm_rank( _frame_comm, &_frame_task_id );
    assert_mpi_status( status );

    // If MPI rank is zero, this is root task on frame communicator.

    _frame_task_root = _frame_task_id == 0;

}

// Configure packed frame communicator (exposures can share nodes).

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_packed_frame_comm()
{

    // Haven't worried about this yet, probably matters more for Bok, SNIFS...

    _init_unpacked_frame_comm();

}

// Initialize exposure-lane communicators.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_exposure_comms()
{

    // Use MPI_Comm_split on the frame communicator to make exposure-lane
    // communicators.

    _exposure_lanes   = _frame_tasks   / frame_tasks_per_exposure_lane();
    _exposure_lane_id = _frame_task_id / frame_tasks_per_exposure_lane();  // split "color"
    int key           = _frame_task_id % frame_tasks_per_exposure_lane();

    int status = MPI_Comm_split( _frame_comm, _exposure_lane_id, key, &_exposure_comm );
    assert_mpi_status( status );

    // Total MPI tasks in this exposure communicator.
    
    status = MPI_Comm_size( _exposure_comm, &_exposure_tasks );
    assert_mpi_status( status );
    
    // MPI rank of this process in its exposure communicator.
    
    status = MPI_Comm_rank( _exposure_comm, &_exposure_task_id );
    assert_mpi_status( status );

    // If MPI rank is zero, this is root task on exposure communicator.

    _exposure_task_root = _exposure_task_id == 0;

}

// FIXME COMMENT ME

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_node_comms()
{

    std::vector< std::string > hostnames = _gather_hostnames( _exposure_comm, _exposure_tasks );

    std::map< std::string, int > hostname_index;
    
    int count = 0; 
    for( auto it = hostnames.begin(); it != hostnames.end(); ++ it )
    {
        if( hostname_index.find( *it ) != hostname_index.end() ) continue;
        hostname_index[ *it ] = count ++;
    }

    int color = hostname_index[ _hostname ];
    int key   = _exposure_task_id;

    int status = MPI_Comm_split( _exposure_comm, color, key, &_node_comm );
    assert_mpi_status( status );

    // Total MPI tasks in this node communicator.
    
    status = MPI_Comm_size( _node_comm, &_node_tasks );
    assert_mpi_status( status );
    
    // MPI rank of this process in its node communicator.
    
    status = MPI_Comm_rank( _node_comm, &_node_task_id );
    assert_mpi_status( status );

    // If MPI rank is zero, this is root task on node communicator.

    _node_task_root = _node_task_id == 0;

}

// Determine OpenMP threads per task.

template< class DetectorPolicy >
inline void C3::Environment< DetectorPolicy >::_init_openmp()
{

    // Suggests utilization of all threads per task.

    _threads_per_task = omp_get_max_threads();

}
