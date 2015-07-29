
#include "../C3_Block.hh"
#include "../C3_FitsException.hh"
#include "../C3_FitsTraits.hh"

template< class T > 
void C3::fits_create_one( const C3::Block< T >& block, const std::string& path )
{} 

C3::FitsCreator::FitsCreator( const std::string& path )
{

    int cfitsio_status = 0;
    fits_create_file( &_fits, ( "!" + path ).c_str(), &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    fits_create_img( fits(), BYTE_IMG, 0, 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

}

template< class T > 
void C3::FitsCreator::operator() ( Block< T >& block, const std::string& extname, const std::vector< C3::size_type >& shape )
{           // FIXME check these numbers I always forget them...

    int   naxis  = shape.size();
    long* naxes  = new long [ naxis ];
    long* fpixel = new long [ naxis ];

    for( auto i = 0; i < naxis; ++ i )
    {
        naxes [ i ] = shape[ i ];
        fpixel[ i ] = 1;
    }

    int cfitsio_status = 0;
    fits_create_img( fits(), C3::FitsType< T >::bitpix, naxis, naxes, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    delete [] naxes;

    fits_write_pix( fits(), C3::FitsType< T >::datatype, fpixel, block.size(), block.data(), &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    delete [] fpixel;

    char value[ FLEN_VALUE ];
    std::copy( extname.begin(), extname.end(), value );
    value[ extname.size() ] = '\0';
    fits_write_key( fits(), TSTRING, "EXTNAME", value, 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

}