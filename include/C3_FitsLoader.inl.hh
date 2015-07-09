
#include "C3_Block.hh"
#include "C3_FitsException.hh"
#include "C3_FitsTraits.hh"

template< class T > 
C3::Block< T >& fits_load_one( C3::Block< T >& block, const std::string& path, const std::string& extname )
{
    FitsLoader loader( path );
    return loader.load( block, extname );
}

C3::FitsLoader< T >::FitsLoader( const std::string& path ) :
    _fits( nullptr )
{
    int cfitsio_status = 0;
    fits_open_file( &_fits, path.c_str(), READONLY, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

C3::FitsLoader< T >::~FitsLoader()
{
    int cfitsio_status = 0;
    fits_close_file( _fits, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

template< class T >
C3::Block< T >& operator () ( C3::Block< T >& block, const std::string& extname )
{
    select_hdu( extname );
    return load( block );
}

template< class T >
void select( const std::string& extname )
{
    fits_movnam_hdu( fits, IMAGE_HDU, extname.c_str(), 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

template< class T >
C3::Block< T >& load( C3::Block< T >& block )
{           // FIXME check these numbers I always forget them...
    fits_read_img( fits, C3::FitsType< T >::value, 0, block.size(), 0, block.data(), 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
    return block;
}
