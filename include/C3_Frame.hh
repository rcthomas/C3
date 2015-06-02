#ifndef C3_FRAME_HH
#define C3_FRAME_HH

#include "C3_OwnedBlock.hh"

namespace C3
{

    /// @class Frame
    /// @brief Two-dimensional arrangement of pixels.
    ///
    /// This is a CCD read-out.  First coordinate indexes over column, second
    /// indexes over row.

    template< class T >
    class Frame : public OwnedBlock< T >
    {

        public :    // Public type definitions.

            using super_type = OwnedBlock< T >;                 ///< Super-class type.
            using value_type = typename super_type::value_type; ///< Content type.

        public :    // Public methods.

            /// Constructor.
            explicit Frame( const size_type ncolumns, const size_type nrows ) noexcept : 
                super_type( ncolumns * nrows ), _ncolumns( ncolumns ), _nrows( nrows ) {}

            /// Number of columns and rows.
            ///@{
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            ///@{
            value_type& operator() ( const size_type j, const size_type k )       { return (*this)[ j + _ncolumns * k ]; }
            value_type  operator() ( const size_type j, const size_type k ) const { return (*this)[ j + _ncolumns * k ]; }
            ///@}

        private :   // Private data members.

            size_type _ncolumns;    ///< Total columns.
            size_type _nrows;       ///< Total rows.

    };

}

#endif
