// NOTE: Breadboard code --- can refactor into nice bits later.  This is 
// not meant to be final in any way, but I'd like to lay as much out here
// and see what it looks like before we go making fancy classes to do 
// stuff and realize we did something stupid.
//
// The MPI environment needs to be sectioned away in a singleton that can
//  work even if the MPI isn't present.  I have code that can do this with
//  minimal confusion.
// It would probably be nice to have some kind of coherent logging here but
//  I haven't thought hard about it.  I would like to not have to depend
//  on anything external, unless there is a compelling reason.  Like there
//  are some drop-in header-only logging things we could use.
// Since this is in MPI the logging either has to know that (which would
//  be easy to mess up) or dead simple (like one per MPI task).
// Bunch of cut-n-pasted stuff for reading header stuff.  This should be
//  sectioned off into an IO manager, etc.  Also should not come from header?
// The image math looks clunky, I had better looking stuff in an earlier 
//  iteration of C3.  I am not opposed to extending the containers 
//  to have more math semantics, but I want it done cleanly.
// Probably using OpenMP to iterate over HDUs is enough work per thread to
//  get a payoff.  We are probably looking at 1-2 MPI tasks per edison node.
//  More than that is not enough memory and we seem to peg the I/O. 
// IO interface through CFITSIO needs to have traits to map types.

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <fitsio.h>
#include <mpi.h>

#include "C3.hh"
#include <vector>
//#include "DECam.hh"

// Convert MPI error status into an exception.

void assert_mpi_status( const int mpi_status )
{
    if( mpi_status == MPI_SUCCESS ) return;
   
    int error_class;
    MPI_Error_class( mpi_status, &error_class );

    char error_string[ MPI_MAX_ERROR_STRING ];
    int length;
    MPI_Error_string( mpi_status, error_string, &length );

    std::stringstream ss;
    ss << "MPI error code " << mpi_status;
    ss << " | error class " << error_class;
    ss << " | "             << error_string;

    throw C3::Exception( ss.str() );
}

// Convert CFITSIO error status into an exception.

void assert_cfitsio_status( const int cfitsio_status )
{
    if( cfitsio_status == 0 ) return;

    char message[ FLEN_STATUS ];
    fits_get_errstatus( cfitsio_status, message );

    std::stringstream ss;
    ss << message;

    throw C3::Exception( ss.str() );
}

// 

int main( int argc, char* argv[] )
{

    // Initialize MPI environment.

    int mpi_status;
    mpi_status = MPI_Init( &argc, &argv );
    assert_mpi_status( mpi_status );

    int mpi_size;
    mpi_status = MPI_Comm_size( MPI_COMM_WORLD, &mpi_size );
    assert_mpi_status( mpi_status );
    
    int mpi_rank;
    mpi_status = MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank );
    assert_mpi_status( mpi_status );

    // Need to load up our assets (bias, flats, cross-talk model, etc.)
    // TODO

    // Main loop over exposures, round-robin over MPI task.

    for( size_t arg = 1 + mpi_rank; arg < argc; arg += mpi_size )
    {

        // Open FITS file.

        int cfitsio_status = 0;

        fitsfile* fptr = 0;
        fits_open_file( &fptr, argv[ arg ], READONLY, &cfitsio_status );
        assert_cfitsio_status( cfitsio_status );

        // Number of HDUs.

        int numhdus = 0;
        fits_get_num_hdus( fptr, &numhdus, &cfitsio_status );
        assert_cfitsio_status( cfitsio_status );

        // Load remaining HDUs into the exposure.

        std::vector< C3::Image< float > > exposure; // or std::map< std::string, C3::Image< float >* >, whatevz

        for( int hdunum = 1; hdunum < numhdus; ++ hdunum )
        {
            
            // Move to the next HDU.

            int hdutype = IMAGE_HDU;
            fits_movrel_hdu( fptr, 1, &hdutype, &cfitsio_status );
            assert_cfitsio_status( cfitsio_status );

            // DATASEC.

            size_t d_imin, d_imax, d_jmin, d_jmax;

            {
                char buffer[ FLEN_VALUE ];

                fits_read_key( fptr, TSTRING, "DATASEC", buffer, 0, &cfitsio_status );
                assert_cfitsio_status( cfitsio_status );

                char tmp;
                std::stringstream ss( buffer );
                ss >> tmp >> d_imin >> tmp >> d_imax >> tmp >> d_jmin >> tmp >> d_jmax;
                d_imin -= 1;
                d_jmin -= 1;
            }

            // DATASEC A.

            size_t da_imin, da_imax, da_jmin, da_jmax;

            {
                char buffer[ FLEN_VALUE ];

                fits_read_key( fptr, TSTRING, "DATASECA", buffer, 0, &cfitsio_status );
                assert_cfitsio_status( cfitsio_status );

                char tmp;
                std::stringstream ss( buffer );
                ss >> tmp >> da_imin >> tmp >> da_imax >> tmp >> da_jmin >> tmp >> da_jmax;
                da_imin -= 1;
                da_jmin -= 1;
            }

            // BIASSEC A.

            size_t ba_imin, ba_imax, ba_jmin, ba_jmax;

            {
                char buffer[ FLEN_VALUE ];

                fits_read_key( fptr, TSTRING, "BIASSECA", buffer, 0, &cfitsio_status );
                assert_cfitsio_status( cfitsio_status );

                char tmp;
                std::stringstream ss( buffer );
                ss >> tmp >> ba_imin >> tmp >> ba_imax >> tmp >> ba_jmin >> tmp >> ba_jmax;
                ba_imin -= 1;
                ba_jmin -= 1;
            }

            // DATASEC B.

            size_t db_imin, db_imax, db_jmin, db_jmax;

            {
                char buffer[ FLEN_VALUE ];

                fits_read_key( fptr, TSTRING, "DATASECB", buffer, 0, &cfitsio_status );
                assert_cfitsio_status( cfitsio_status );

                char tmp;
                std::stringstream ss( buffer );
                ss >> tmp >> db_imin >> tmp >> db_imax >> tmp >> db_jmin >> tmp >> db_jmax;
                db_imin -= 1;
                db_jmin -= 1;
            }

            // BIASSEC B.

            size_t bb_imin, bb_imax, bb_jmin, bb_jmax;

            {
                char buffer[ FLEN_VALUE ];

                fits_read_key( fptr, TSTRING, "BIASSECB", buffer, 0, &cfitsio_status );
                assert_cfitsio_status( cfitsio_status );

                char tmp;
                std::stringstream ss( buffer );
                ss >> tmp >> bb_imin >> tmp >> bb_imax >> tmp >> bb_jmin >> tmp >> bb_jmax;
                bb_imin -= 1;
                bb_jmin -= 1;
            }

            // Image parameters.

            int  maxdim = 2;
            int  bitpix = 0;
            int  naxis  = 0;
            long naxes[ 2 ] = { 0, 0 };

            fits_get_img_param( fptr, maxdim, &bitpix, &naxis, naxes, &cfitsio_status );
            assert_cfitsio_status( cfitsio_status );

            // Create empty image.

            C3::Image< float > image = C3::Image< float >::create( naxes[ 0 ], naxes[ 1 ] );

            // Fill image.

            int     datatype  = TFLOAT; //C3::Traits< T >::cfitsio_type;
            long    firstelem = 1;
            long    nelements = naxes[ 0 ] * naxes[ 1 ];
            float   nulval    = 0;
            int     anynul    = 0;

            fits_read_img( fptr, datatype, firstelem, nelements, &nulval, image._data, &anynul, &cfitsio_status );
            assert_cfitsio_status( cfitsio_status );

            // Overscan subtraction.

            C3::View< float > dataseca  = C3::View< float >::create( image, da_imin, da_imax - da_imin, da_jmin, da_jmax - da_jmin );
            C3::View< float > biasseca  = C3::View< float >::create( image, ba_imin, ba_imax - ba_imin, ba_jmin, ba_jmax - ba_jmin );
            dataseca -= C3::Reduce< C3::Median, C3::View< float >, C3::Column< float > >::compute( biasseca );

            C3::View< float > datasecb = C3::View< float >::create( image, db_imin, db_imax - db_imin, db_jmin, db_jmax - db_jmin );
            C3::View< float > biassecb = C3::View< float >::create( image, bb_imin, bb_imax - bb_imin, bb_jmin, bb_jmax - bb_jmin );
            datasecb -= C3::Reduce< C3::Median, C3::View< float >, C3::Column< float > >::compute( biassecb );

            // New image from the view.

            C3::View< float >  datasec = C3::View< float >::create( image, d_imin, d_imax - d_imin, d_jmin, d_jmax - d_jmin );
            C3::Image< float > trimmed = C3::Image< float >::create( datasec.ncols(), datasec.nrows() );
            trimmed = datasec;
            exposure.push_back( trimmed );

        }

        // Close FITS file.

        fits_close_file( fptr, &cfitsio_status );
        assert_cfitsio_status( cfitsio_status );

        if( mpi_rank == 0 ) std::cerr << argv[ arg ] << std::endl; // tmp

        // now do more stuff, subtract bias, flat field, etc...

    }

    // Goodbye.

    mpi_status = MPI_Finalize();
    return 0;

}
