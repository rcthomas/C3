#ifndef C3__EXCEPTION
#define C3__EXCEPTION

#include <stdexcept>

namespace C3
{

    /// @class Exception
    /// @brief Basic exception class.

    class Exception : public std::runtime_error
    {

        public :

            /// Constructor.
            Exception( const std::string& message );

    };

}

#include "C3_Exception.inl.hh"

#endif
