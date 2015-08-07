#ifndef C3_FITS_LOADER_HH
#define C3_FITS_LOADER_HH

#include "C3_FitsResource.hh"

namespace C3
{

    /// @class FitsLoader
    /// @brief Populate a Block with data from a FITS file.

    class FitsLoader : public FitsResource
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsLoader( const std::string& path );

            /// Select HDU and load data into pre-allocated block.
            template< class T > Block< T >& load( Block< T >& block, const std::string& extname );

            /// Load data into pre-allocated block from previously selected HDU.
            template< class T > Block< T >& load( Block< T >& block );

            /// Select HDU.
            void select( const std::string& extname );


    };

}

#include "inline/C3_FitsLoader.hh"

#endif
