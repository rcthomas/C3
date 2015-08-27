#ifndef C3_ROW_HH
#define C3_ROW_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class Row
    /// @brief Horizontal arrangement of pixels.

    template< class T >
    class Row : public Block< T >
    {

        public :    // Public methods.

            /// Constructor.
            explicit Row( const size_type ncolumns ) noexcept;

            /// Initializing constructor.
            Row( const size_type ncolumns, const T pixel ) noexcept;

            /// Number of columns.
            size_type ncolumns() const { return this->size(); }

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j )       { return (*this)[ j ]; }
            T  operator() ( const size_type j ) const { return (*this)[ j ]; }
            ///@}

            /// Pixel assignment.
            Row& operator = ( const T pixel ) noexcept;

            /// Value type conversion.
            template< class U >
            operator Row< U >() const noexcept;

    };

}

#include "inline/C3_Row.hh"

#endif
