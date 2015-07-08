#ifndef C3_EXCEPTION_HH
#define C3_EXCEPTION_HH

#include <stdexcept>

namespace C3
{

    /// @class Exception
    /// @brief Base exception class.

    struct Exception : public std::runtime_error
    {

        /// Constructor.
        Exception( const std::string& message ) : std::runtime_error( message ) {}

    };

}

#endif
