
#include "../C3_Block.hh"
#include "../C3_FitsException.hh"
#include "../C3_FitsTraits.hh"

// Constructor.

inline C3::FitsCreator::FitsCreator( const std::string& path )
{

    int cfitsio_status = 0;
    fits_create_file( &_fits, ( "!" + path ).c_str(), &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    fits_create_img( fits(), BYTE_IMG, 0, 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

}

// Create HDU and store unconverted data in it.

template< class T >
inline void C3::FitsCreator::create( Block< T >& block, const std::string& extname, const int naxis, long* naxes )
{
    create< T, T >( block, extname, naxis, naxes );
}

// Create HDU and store converted data in it.

template< class T, class U > 
inline void C3::FitsCreator::create( Block< U >& block, const std::string& extname, const int naxis, long* naxes )
{

    int cfitsio_status = 0;
    fits_create_img( fits(), C3::FitsType< T >::bitpix, naxis, naxes, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    long* fpixel = new long [ naxis ];
    for( auto i = 0; i < naxis; ++ i ) fpixel[ i ] = 1;

    fits_write_pix( fits(), C3::FitsType< T >::datatype, fpixel, block.size(), block.data(), &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

    delete [] fpixel;

    char value[ FLEN_VALUE ];
    std::copy( extname.begin(), extname.end(), value );
    value[ extname.size() ] = '\0';
    fits_write_key( fits(), TSTRING, "EXTNAME", value, 0, &cfitsio_status );
    C3::assert_fits_status( cfitsio_status );

}
