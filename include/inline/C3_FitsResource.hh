
#include "C3_FitsException.hh"

C3::FitsResource::FitsResource( const std::string& path, const int mode ) :
    _fits( nullptr )
{
    int cfitsio_status = 0;
    fits_open_file( &_fits, path.c_str(), mode, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

C3::FitsResource::~FitsResource()
{
    int cfitsio_status = 0;
    fits_close_file( _fits, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

