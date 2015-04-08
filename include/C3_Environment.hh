#ifndef C3__ENVIRONMENT
#define C3__ENVIRONMENT

#include <string>
#include <vector>

#include <mpi.h>

namespace C3
{

    /// Raise exception if MPI status passed is not MPI_SUCCESS.
    void assert_mpi_status( const int mpi_status );

    /// @class Environment
    /// @brief Singleton encapsulating parallel processing information.
    // 
    //  This contains not only information about the local process environment,
    //  but useful information about the global process environment.
    //  Assumptions
    //  * The number of frame tasks per exposure lane is constant.
    //  * The number of tasks per node is constant.
    //  After start up user should not use MPI_COMM_WORLD, for things that
    //  would nonimally use the MPI_COMM_WORLD communicator they should use
    //  the frame communicator.
    
    template< class DetectorPolicy >
    class Environment : public DetectorPolicy
    {
    
        public :
    
            /// Start-up access point.
            static Environment& instance( int& argc, char**& argv );
    
            /// General access point.
            static Environment& instance();

            /// Stream a summary of the environment (to frame root).
            void summary( std::ostream& stream );

            /// Frame communicator access.
            MPI_Comm& frame_comm() { return _frame_comm; }

            /// Number of frame tasks per exposure lane.
            int frame_tasks_per_exposure_lane() const { return DetectorPolicy::frames(); } 
    
            /// Shutdown, perform MPI finalize.
            void finalize();

        private :

            /// Constructor.
            Environment() {}
    
            /// Copy constructor.
            Environment( const Environment& env ) {}
    
            /// Destructor.
            ~Environment() {}
    
            /// Assignment.
            Environment& operator = ( const Environment& env ) { return *this; }
    
            /// Main initializer.
            void _init( int& argc, char**& argv );

            /// MPI world communicator.
            void _init_mpi_world( int& argc, char**& argv );

            /// Hostname information.
            void _init_hostname();

            /// Derive tasks per node.
            void _init_tasks_per_node();

            /// Hostname of each task in an MPI communicator in rank order.
            std::vector< std::string > _gather_hostnames( const MPI_Comm& comm, const int tasks );

            /// Configure unpacked frame communicator.
            void _init_unpacked_frame_comm();

            /// Configure packed frame communicator (TBD).
            void _init_packed_frame_comm();

            /// Configure exposure communicators.
            void _init_exposure_comms();

            /// Configure node communicators.
            void _init_node_comms();

            /// Configure OpenMP information.
            void _init_openmp();
    
            static Environment< DetectorPolicy >* _instance;    ///< Singleton instance.
   
            int          _world_tasks;                          ///< Total MPI ranks in world communicator at start up.
            int          _world_task_id;                        ///< MPI rank of this process in world communicator at start up.
            bool         _world_task_root;                      ///< True if MPI rank of this process in world communicator is zero at start up.
    
            std::string _hostname;                              ///< Name of this task's node.
            int         _tasks_per_node;                        ///< Number of tasks on each node.

            MPI_Comm    _frame_comm;                            ///< MPI communicator including all frame tasks.
            int         _frame_tasks;                           ///< Total MPI ranks in frame communicator.
            int         _frame_task_id;                         ///< MPI rank of this process in frame communicator.
            bool        _frame_task_root;                       ///< True if MPI rank of this process in frame communicator is zero.

            int         _exposure_lanes;                        ///< Number of exposure lanes.
            int         _exposure_lane_id;                      ///< Identifier of exposure lane this process belongs to.

            MPI_Comm    _exposure_comm;                         ///< MPI communicator including all frame tasks in this exposure lane.
            int         _exposure_tasks;                        ///< Total MPI ranks in this process's exposure communicator.
            int         _exposure_task_id;                      ///< MPI rank of this process in its exposure communicator.
            bool        _exposure_task_root;                    ///< True if MPI rank of this process in its exposure communicator.

            MPI_Comm    _node_comm;                             ///< MPI communicator including all tasks from this exposure-lane on this node.
            int         _node_tasks;                            ///< Total MPI ranks in this process's node communicator.
            int         _node_task_id;                          ///< MPI rank of this process in its node communicator.
            bool        _node_task_root;                        ///< True if MPI rank of this process in its node communicator.
    
            int         _threads_per_task;                      ///< Number of OpenMP threads per MPI rank.
    
    };

}

#include "C3_Environment.inl.hh"

#endif
