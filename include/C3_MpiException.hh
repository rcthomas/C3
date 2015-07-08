#ifndef C3_MPI_EXCEPTION_HH
#define C3_MPI_EXCEPTION_HH

#include "C3_Exception.hh"

namespace C3
{

    /// Raise exception if MPI status passed is not MPI_SUCCESS.
    void assert_mpi_status( const int mpi_status );

    /// @class FitsException
    /// @brief Exception to raise when FITS errors happen.

    struct MpiException : public Exception
    {

        /// Constructor.
        MpiException( const int mpi_status );

        /// Translate an MPI error status code into an error status string.
        static std::string status_string( const int mpi_status );

    };

}

#include "C3_MpiException.inl.hh"

#endif
