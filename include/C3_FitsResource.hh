#ifndef C3_FITS_RESOURCE_HH
#define C3_FITS_RESOURCE_HH

#include <fitsio.h>

namespace C3
{

    /// @class FitsResource
    /// @brief CFITSIO object management.
    ///
    /// Provides object life cycle to CFITSIO file objects.  Since this class
    /// manages a file resource, its instances are not copyable.

    class FitsResource
    {

        public :    // Public methods.

            /// Constructor.
            explicit FitsResource( const std::string& path, const int mode );

            /// Copy constructor.
            FitsResource( const FitsResource& resource ) = delete;

            /// Move constructor.
            FitsResource( FitsResource&& resource ) = delete;

            /// Copy assignment.
            FitsResource& operator = ( const FitsResource& resource ) = delete;

            /// Move constructor.
            FitsResource& operator = ( FitsResource&& resource ) = delete;

            /// Destructor.
            ~FitsResource();

            /// FITS struct.
            ///@{
                  fitsfile* fits()       { return _fits; }
            const fitsfile* fits() const { return _fits; }
            ///@}

        private :   // Private data members.

            fitsfile*   _fits;  ///< CFITSIO file object.

    };

}

#include "C3_FitsResource.inl.hh"

#endif
