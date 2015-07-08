#ifndef C3_FITS_LOADER_HH
#define C3_FITS_LOADER_HH

namespace C3
{

    /// Convenience function to load data from one HDU from a file.
    template< class T > Block< T >& fits_load_one( Block< T >& block, const std::string& path, const std::string& extname );

    /// @class FitsLoader
    /// @brief Populate a Block with data from a FITS file.
    ///
    /// A FitsLoader object manages the lifecycle of a read-only CFITSIO file
    /// object and provides methods of populating Blocks with data stored in
    /// the associated FITS file.

    template< class T >
    class FitsLoader
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsLoader( const std::string& path );

            /// Copy constructor is deleted.
            FitsLoader( const FitsLoader& loader ) = delete;

            /// Move constructor is deleted.
            FitsLoader( FitsLoader&& loader ) = delete;

            /// Copy assignment is deleted.
            FitsLoader& operator = ( const FitsLoader& loader ) = delete;

            /// Move assignment is deleted.
            FitsLoader& operator = ( FitsLoader&& loader ) = delete;

            /// Destructor.
            ~FitsLoader();

            /// Load data from extension into previously allocated block.
            Block< T >& operator () ( Block< T >& block, const std::string& extname );

        private :   // Private methods.

            fitsfile* _fits;  ///< CFITSIO file structure.

    };

}

#include "C3_FitsLoader.inl.hh"

#endif
