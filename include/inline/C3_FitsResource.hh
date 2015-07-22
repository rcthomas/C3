
#include "../C3_FitsException.hh"

C3::FitsResource::FitsResource() :
    _fits( nullptr )
{}

C3::FitsResource::~FitsResource()
{
    int cfitsio_status = 0;
    fits_close_file( _fits, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

