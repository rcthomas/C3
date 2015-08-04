#ifndef C3_FILE_LOGGER_HH
#define C3_FILE_LOGGER_HH

#include <fstream>

#include "C3_Logger.hh"

namespace C3
{

    /// @class FileLogger
    /// @brief Sends log messages to a file.

    class FileLogger : public Logger
    {

        public :    // Public methods.

            /// Constructor.
            FileLogger( const std::string& path, const LogLevel level = LogLevel::DEFAULT ) :
                _ofstream( path ), Logger( level ) {}

        private :   // Private methods.

            /// Reference to the open file stream object.
            std::ostream& _stream() { return _ofstream; }

        private :   // Private data.

            std::ofstream   _ofstream;  ///< File stream object.

    };

}

#endif
