#ifndef C3_FITS_EXCEPTION_HH
#define C3_FITS_EXCEPTION_HH

#include "C3_Exception.hh"

namespace C3
{

    /// Raise exceptoin if CFITSIO status passed is nonzero.
    void assert_fits_status( const int cfitsio_status );

    /// @class FitsException
    /// @brief Exception to raise when FITS errors happen.

    struct FitsException : public Exception
    {

        /// Constructor.
        FitsException( const int cfitsio_status );

        /// Translate a CFITSIO status code into an error status string.
        static std::string status_string( const int cfitsio_status );

    };

}

#include "inline/C3_FitsException.hh"

#endif
