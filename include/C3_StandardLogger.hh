#ifndef C3_STANDARD_LOGGER_HH
#define C3_STANDARD_LOGGER_HH

#include "C3_Logger.hh"

namespace C3
{

    /// @class StandardLogger
    /// @brief Sends log messages to standard output

    class StandardLogger : public Logger
    {

        public :    // Public methods.

            /// Constructor.
            explicit StandardLogger( const LogLevel level = LogLevel::DEFAULT ) :
                Logger( level ) {}

        private :   // Private methods.

            /// Output stream access.
            std::ostream& _stream() { return std::cout; }

    };

}

#endif
