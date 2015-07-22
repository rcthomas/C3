#ifndef C3_SERIAL_HH
#define C3_SERIAL_HH

#include <queue>

#include <yaml-cpp/yaml.h>

namespace C3
{

    template< class T > class Frame;

    /// @class Serial
    /// @brief Single-frame, serial execution policy.

    template< class InstrumentTraits >
    class Serial
    {

        public :    // Public methods.

            /// Configuration and task stream from command line.
            void init( int& argc, char**& argv );

            /// Executed on exit.
            int finalize() { return EXIT_SUCCESS; }

            /// Return the config.
            const YAML::Node& config() const { return _config; }

            /// True if tasks remain in the stream.
            bool has_tasks() const { return _task_files.size() > 0 || _tasks.size() > 0; }

            /// Returns the next task in the stream.
            YAML::Node next_task();

            /// Frame.
            std::string frame() const { return _config[ "frame" ].template as< std::string >(); }

            /// Load frame.
            template< class T >
            void load( C3::Frame< T >& input, const std::string& path );

            /// Save frame.
            template< class T >
            void save( C3::Frame< T >& output, const std::string& path );

            /// Save frame tuple.
            template< class T, class U >
            void save( C3::Frame< T >& output, C3::Frame< T >& invvar, C3::Frame< U >& flags, const std::string& path );

//          /// Save tuple.
//          template< class T >
//          void save( const C3::Tuple< T >& output, const std::string& path );

        protected : // Protected methods.

            /// Destructor.
            ~Serial() = default;

        protected : // Protected data members.

            YAML::Node                  _config;        ///< Configuration.
            std::queue< std::string >   _task_files;    ///< Task stream.
            std::queue< YAML::Node  >   _tasks;         ///< Current task chunk.

    };

}

#include "inline/C3_Serial.hh"

#endif
