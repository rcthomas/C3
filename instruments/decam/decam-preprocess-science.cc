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
class SecRef {
    // merge into C3?
    public:
    int imin;
    int jmin;
    int imax;
    int jmax;
    int isize;
    int jsize;
    SecRef (const char buffer[]) {
        char tmp;
        std::stringstream ss( buffer );
        ss >> tmp >> imin >> tmp >> imax >> tmp >> jmin >> tmp >> jmax;
        imin -= 1;
        jmin -= 1;
        isize = imax - imin;
        jsize = jmax - jmin;
    }
};

const char * fits_read_string (fitsfile* fptr, 
        const char * key, char * buffer) {

    // alternative is to add a constructor to SecRef. but that's equally
    // ugly!
    int cfitsio_status = 0;
    fits_read_key( fptr, TSTRING, key, buffer, 0, &cfitsio_status );
    assert_cfitsio_status( cfitsio_status );
    return buffer;
}

C3::View< float > view_secref(C3::Image<float> image, SecRef& ref) {
    // merge into C3?
    return C3::View< float >::create(image, ref.imin, ref.isize, ref.jmin, ref.jsize);
}

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
            char buffer[ FLEN_VALUE ];
            SecRef DATASEC (fits_read_string(fptr, "DATASEC" , buffer)),
                   DATASECA(fits_read_string(fptr, "DATASECA", buffer)),
                   DATASECB(fits_read_string(fptr, "DATASECB", buffer)),
                   BIASSECA(fits_read_string(fptr, "BIASSECA", buffer)),
                   BIASSECB(fits_read_string(fptr, "BIASSECB", buffer));

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

            C3::View< float > dataseca  = view_secref(image, DATASECA);
            C3::View< float > biasseca  = view_secref(image, BIASSECA);
            dataseca -= C3::Reduce< C3::Median, C3::View< float >, C3::Column< float > >::compute( biasseca );

            C3::View< float > datasecb = view_secref(image, DATASECB);
            C3::View< float > biassecb = view_secref(image, BIASSECB);
            datasecb -= C3::Reduce< C3::Median, C3::View< float >, C3::Column< float > >::compute( biassecb );

            // New image from the view.

            C3::View< float >  datasec = view_secref(image, DATASEC);
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
