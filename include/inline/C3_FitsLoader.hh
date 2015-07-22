
#include "../C3_Block.hh"
#include "../C3_FitsException.hh"
#include "../C3_FitsTraits.hh"

template< class T > 
C3::Block< T >& fits_load_one( C3::Block< T >& block, const std::string& path, const std::string& extname )
{
    C3::FitsLoader loader( path );
    return loader.load( block, extname );
}

inline C3::FitsLoader::FitsLoader( const std::string& path )
{
    int cfitsio_status = 0;
    fits_open_file( &_fits, path.c_str(), READONLY, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

template< class T >
C3::Block< T >& C3::FitsLoader::operator () ( C3::Block< T >& block, const std::string& extname )
{
    select( extname );
    load( block );
    return block;
}

void C3::FitsLoader::select( const std::string& extname )
{
    char value[ FLEN_VALUE ];
    std::copy( extname.begin(), extname.end(), value );
    value[ extname.size() ] = '\0';
    int cfitsio_status = 0;
    fits_movnam_hdu( fits(), IMAGE_HDU, value, 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
}

template< class T >
C3::Block< T >& C3::FitsLoader::load( C3::Block< T >& block )
{           // FIXME check these numbers I always forget them...
    int cfitsio_status = 0;
    fits_read_img( fits(), C3::FitsType< T >::datatype, 1, block.size(), 0, block.data(), 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );
    return block;
}
