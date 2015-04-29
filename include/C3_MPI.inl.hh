
#include <sstream>

#include <mpi.h>

#include "C3_Exception.hh"

// Raise an exception if MPI status is not MPI_SUCCESS.

inline void C3::MPI::assert_status( const int status )
{

    if( status == MPI_SUCCESS ) return;
   
    int error_class;
    MPI_Error_class( status, &error_class );

    char error_string[ MPI_MAX_ERROR_STRING ];
    int length;
    MPI_Error_string( status, error_string, &length );

    std::stringstream ss;
    ss << "MPI error code " << status;
    ss << " | error class " << error_class;
    ss << " | "             << error_string;

    throw C3::Exception( ss.str() );

}
