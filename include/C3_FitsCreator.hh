#ifndef C3_FITS_CREATOR_HH
#define C3_FITS_CREATOR_HH

#include "C3_FitsResource.hh"

namespace C3
{

    /// Convenience function to create a simple primary-image HDU output.
    template< class T > void fits_create_one( const Block< T >& block, const std::string& path );

    /// @class FitsCreator
    /// @brief Populate a FITS file with data from Blocks.

    class FitsCreator : public FitsResource
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsCreator( const std::string& path );

            /// Select/create HDU and place data into it.
            template< class T > void operator() ( Block< T >& block, const std::string& extname, const std::vector< C3::size_type >& shape );

//          /// Select/create HDU.
//          void select( const std::string& extname );

//          /// Place data from a Block into selected HDU.
//          template< class T > void save( const Block< T >& block );

    };

}

#include "inline/C3_FitsCreator.hh"

#endif
