#ifndef C3_PARALLEL_HH
#define C3_PARALLEL_HH

#include <queue>

#include <yaml-cpp/yaml.h>

#include "C3_Communicator.hh"
#include "C3_FileLogger.hh"

namespace C3
{

    template< class T > class Frame;

    /// @class Parallel
    /// @brief Multi-frame, parallel execution policy.

    template< class InstrumentTraits >
    class Parallel
    {

        public :    // Public methods.

            /// Configuration and task stream from command line.
            void init( int& argc, char**& argv );

            /// Executed on exit.
            int finalize();

            /// Return the config.
            const YAML::Node& config() const { return _config; }

            /// True if tasks remain in the stream.
            bool has_tasks() const { return _task_files.size() > 0 || _tasks.size() > 0; }

            /// Returns the next task in the stream.
            YAML::Node next_task();

            /// Frame.
            std::string frame() const { return InstrumentTraits::frames[ exposure_comm().rank() ]; }

            /// Logger.
            Logger& logger() { return *_logger; }

            /// Load frame.
            template< class T >
            void load( C3::Frame< T >& frame, const std::string& path );

            /// Save unconverted frame.
            template< class T >
            void save( C3::Frame< T >& output, const std::string& path );

            /// Save converted frame.
            template< class T, class U >
            void save( C3::Frame< U >& output, const std::string& path );

            /// Save frame tuple without conversion of output and inverse variance.
            template< class T, class U >
            void save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path );

            /// Save frame tuple with conversion of output and inverse variance.
            template< class T, class U, class V >
            void save( C3::Frame< U >& output, C3::Frame< U >& invvar, C3::Frame< V >& flags, const std::string& path );

            /// Communicator wrappers.
            ///@{
            const Communicator& world_comm()    const { return *_world_comm;    }
            const Communicator& frame_comm()    const { return *_frame_comm;    }
            const Communicator& exposure_comm() const { return *_exposure_comm; }
            const Communicator& node_comm()     const { return *_node_comm;     }
            ///@}

            /// Exposure lane information.
            ///@{
            int exposure_lanes()      const { return _exposure_lanes; }
            int exposure_lane()       const { return _exposure_lane;  }
            int exposure_lane_width() const { return InstrumentTraits::frames.size(); }
            ///@}

        protected : // Protected methods.

            /// Destructor.
            ~Parallel() = default;

        private :   // Private methods.

            /// Initializers for various components.
            ///@{
            void _init_mpi( int& argc, char**& argv );              // Launch MPI.
            void _init_world();                                     // World communicator wrapper.
            void _validate_command_line( int& argc, char**& argv ); // 
            void _init_hostname();                                  // Hostname of this MPI process.
            void _init_mpi_processes_per_node();                    // Per node in world at startup.
            void _init_frame_unpacked();                            // Unpacked frame communicator.
            void _init_frame_packed();                              // Packed frame communicator.
            void _init_config( int& argc, char**& argv );           // 
            void _init_exposure();                                  // Exposure communicators.
            void _init_logger();
            void _init_openmp();                                    // OpenMP information.
            void _init_task_queue( int& argc, char**& argv );
            ///@}

            /// Hostname of each process in an MPI communicator in rank order.
            std::vector< std::string > _gather_hostnames( const C3::Communicator& comm );

        protected : // Protected data members.

            YAML::Node                  _config;            ///< Configuration.
            std::queue< std::string >   _task_files;        ///< Task stream.
            std::queue< YAML::Node  >   _tasks;             ///< Current task chunk.

            int                         _task_position;        ///<

        private :   // Private data members.

            std::unique_ptr< C3::Communicator > _world_comm;     ///< All MPI processes contained at startup.
            std::unique_ptr< C3::Communicator > _frame_comm;     ///< MPI processes actively handling frames.
            std::unique_ptr< C3::Communicator > _exposure_comm;  ///< MPI processes within an exposure lane.
    
            int                 _exposure_lanes;            ///< Number of exposure lanes.
            int                 _mpi_processes_per_node;    ///< MPI processes per node.
            int                 _threads_per_mpi_process;   ///< OpenMP threads per MPI process.

            int                 _exposure_lane;             ///< Exposure lane this MPI process belongs to.
            std::string         _hostname;                  ///< Hostname of this MPI process.

            std::unique_ptr< FileLogger >   _logger;        ///< Always a file logger.

    };

}

#include "inline/C3_Parallel.hh"

#endif
