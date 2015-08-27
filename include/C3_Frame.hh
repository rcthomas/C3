#ifndef C3_FRAME_HH
#define C3_FRAME_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class Frame
    /// @brief Two-dimensional arrangement of pixels.

    template< class T >
    class Frame : public Block< T >
    {

        public :    // Public methods.

            /// Constructor.
            Frame( const size_type ncolumns, const size_type nrows ) noexcept;

            /// Initializing constructor.
            Frame( const size_type ncolumns, const size_type nrows, const T pixel ) noexcept;

            /// Number of columns and rows.
            ///@{
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j, const size_type k )       { return (*this)[ j + _ncolumns * k ]; }
            T  operator() ( const size_type j, const size_type k ) const { return (*this)[ j + _ncolumns * k ]; }
            ///@}

            /// Pixel assignment.
            Frame& operator = ( const T pixel ) noexcept;

            /// Value type conversion.
            template< class U >
            operator Frame< U >() const noexcept;

        private :   // Private data members.

            size_type   _ncolumns;  ///< Total columns.
            size_type   _nrows;     ///< Total rows.

    };

}

#include "inline/C3_Frame.hh"

#endif
