#ifndef C3_FITS_CREATOR_HH
#define C3_FITS_CREATOR_HH

#include "C3_FitsResource.hh"

namespace C3
{

    /// @class FitsCreator
    /// @brief Populate a FITS file with data from Blocks.

    class FitsCreator : public FitsResource
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsCreator( const std::string& path );

            /// Create HDU and store unconverted data in it.
            template< class T > void create( Block< T >& block, const std::string& extname, const int naxis, long* naxes );

            /// Create HDU and store unconverted data in it.
            template< class T, class U > void create( Block< U >& block, const std::string& extname, const int naxis, long* naxes );

    };

}

#include "inline/C3_FitsCreator.hh"

#endif
