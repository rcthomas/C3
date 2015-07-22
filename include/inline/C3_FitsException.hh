
#include <sstream>

#include <fitsio.h>

inline void C3::assert_fits_status( const int cfitsio_status )
{
    if( cfitsio_status == 0 ) return;
    throw C3::FitsException( cfitsio_status );
}

inline C3::FitsException::FitsException( const int cfitsio_status ) :
    C3::Exception( C3::FitsException::status_string( cfitsio_status ) )
{}

inline std::string C3::FitsException::status_string( const int cfitsio_status ) 
{

    char status_message[ FLEN_STATUS ];
    fits_get_errstatus( cfitsio_status, status_message );

    std::stringstream ss;
    ss << "CFITSIO error code " << cfitsio_status;
    ss << " | "                 << status_message;
    return ss.str();

}
