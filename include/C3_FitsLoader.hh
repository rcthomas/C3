#ifndef C3_FITS_LOADER_HH
#define C3_FITS_LOADER_HH

#include "C3_FitsResource.hh"

namespace C3
{
    template< class T >
    class Block;
}

namespace C3
{

    /// Convenience function to load data from one HDU from a file.
    template< class T > Block< T >& fits_load_one( Block< T >& block, const std::string& path, const std::string& extname );

    /// @class FitsLoader
    /// @brief Populate a Block with data from a FITS file.

    class FitsLoader : public FitsResource
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsLoader( const std::string& path );

            /// Select HDU and load data into pre-allocated block.
            template< class T > Block< T >& operator() ( Block< T >& block, const std::string& extname );

            /// Select HDU.
            void select( const std::string& extname );

            /// Load data into pre-allocated block.
            template< class T > Block< T >& load( Block< T >& block );

    };

}

#include "inline/C3_FitsLoader.hh"

#endif
