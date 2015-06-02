#ifndef C3_ENVIRONMENT_HH
#define C3_ENVIRONMENT_HH

#include <memory>
#include <vector>

#include "C3_Communicator.hh"

namespace C3
{

    /// @class Environment
    /// @brief Singleton encapsulating parallel processing information.
    
    template< class DetectorPolicy >
    class Environment
    {

        public :    // Public methods.

            /// Copy constructor.
            Environment( const Environment& env ) = delete;

            /// Move constructor.
            Environment( Environment&& env ) = delete;

            /// Copy assignment.
            Environment& operator = ( const Environment& env ) = delete;

            /// Move assignment.
            Environment& operator = ( Environment&& env ) = delete;
    
            /// Start-up access point.
            static Environment& instance( int& argc, char**& argv );
    
            /// General access point used after start-up.
            static Environment& instance();

            /// Stream a summary of the environment (to frame root).
            void summary( std::ostream& stream );

            /// Communicator wrappers.
            const Communicator& world()    const { return *_world;    }
            const Communicator& frame()    const { return *_frame;    }
            const Communicator& exposure() const { return *_exposure; }
            const Communicator& node()     const { return *_node;     }

            /// Exposure lane information.
            ///@{
            int exposure_lanes()      const { return _exposure_lanes; }
            int exposure_lane()       const { return _exposure_lane;  }
            int exposure_lane_width() const { return DetectorPolicy::frames(); }
            ///@}

            /// Shutdown.
            int finalize();

        private :   // Private methods.

            /// Constructor.
            Environment() = default;
    
            /// Destructor.
            ~Environment() = default;
    
            /// Initializers for various components.
            ///@{
            void _init( int& argc, char**& argv );      // Main initializer.
            void _init_mpi( int& argc, char**& argv );  // Launch MPI.
            void _init_world();                         // World communicator wrapper.
            void _init_hostname();                      // Hostname of this MPI process.
            void _init_mpi_processes_per_node();        // Per node in world at startup.
            void _init_frame_unpacked();                // Unpacked frame communicator.
            void _init_frame_packed();                  // Packed frame communicator.
            void _init_exposure();                      // Exposure communicators.
            void _init_node();                          // Node communicators.
            void _init_openmp();                        // OpenMP information.
            ///@}

            /// Hostname of each process in an MPI communicator in rank order.
            std::vector< std::string > _gather_hostnames( const C3::Communicator& comm );

        private :   // Private data members.

            std::unique_ptr< C3::Communicator > _world;     ///< All MPI processes contained at startup.
            std::unique_ptr< C3::Communicator > _frame;     ///< MPI processes actively handling frames.
            std::unique_ptr< C3::Communicator > _exposure;  ///< MPI processes within an exposure lane.
            std::unique_ptr< C3::Communicator > _node;      ///< MPI processes with the same hostname.
    
            int                 _exposure_lanes;            ///< Number of exposure lanes.
            int                 _mpi_processes_per_node;    ///< MPI processes per node.
            int                 _threads_per_mpi_process;   ///< OpenMP threads per MPI process.

            int                 _exposure_lane;             ///< Exposure lane this MPI process belongs to.
            std::string         _hostname;                  ///< Hostname of this MPI process.
    
    };

}

#include "C3_Environment.inl.hh"

#endif
