
#include <sstream>

#include <mpi.h>

#include "C3_MpiException.hh"

inline void C3::assert_mpi_status( const int mpi_status )
{
    if( mpi_status == 0 ) return;
    throw C3::MpiException( mpi_status );
}

inline C3::MpiException::MpiException( const int mpi_status ) :
    C3::Exception( C3::MpiException::status_string( mpi_status ) )
{}


inline std::string C3::MpiException::status_string( const int mpi_status )
{
   
    int error_class;
    MPI_Error_class( mpi_status, &error_class );

    char error_string[ MPI_MAX_ERROR_STRING ];
    int length;
    MPI_Error_string( mpi_status, error_string, &length );

    std::stringstream ss;
    ss << "MPI error code " << mpi_status;
    ss << " | error class " << error_class;
    ss << " | "             << error_string;
    return ss.str();

}
