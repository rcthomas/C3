#ifndef C3_SERIAL_HH
#define C3_SERIAL_HH

#include <queue>

#include <yaml-cpp/yaml.h>

#include "C3_Logger.hh"

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
            int finalize();

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

            /// Logger.
            Logger& logger() { return *_logger; }

        protected : // Protected methods.

            /// Destructor.
            ~Serial() = default;

        private :   // Private methods.

            void _validate_command_line( int& argc, char**& argv );
            void _init_config( int& argc, char**& argv );
            void _validate_frame();
            void _init_logger();
            void _init_logger_defined();
            void _init_logger_default();
            void _init_task_queue( int& argc, char**& argv );

        private : // Private data members.

            YAML::Node                  _config;        ///< Configuration.
            std::queue< std::string >   _task_files;    ///< Task stream.
            std::queue< YAML::Node  >   _tasks;         ///< Current task chunk.

            std::unique_ptr< Logger >   _logger;        ///< Logger, either standard or file-based.

    };

}

#include "inline/C3_Serial.hh"

#endif
