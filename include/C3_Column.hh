#ifndef C3_COLUMN_HH
#define C3_COLUMN_HH

#include "C3_Block.hh"

namespace C3
{

    /// @class Column
    /// @brief Vertical arrangement of pixels.

    template< class T >
    class Column : public Block< T >
    {

        public :    // Public methods.
        
            /// Constructor.
            explicit Column( const size_type nrows ) noexcept;
        
            /// Initializing constructor.
            Column( const size_type nrows, const T pixel ) noexcept;

            /// Number of rows.
            size_type nrows() const { return this->size(); }
        
            /// Coordinate access.
            /// @{
            T& operator() ( const size_type k )       { return (*this)[ k ]; }
            T  operator() ( const size_type k ) const { return (*this)[ k ]; }
            /// @}

            /// Pixel assignment.
            Column& operator = ( const T pixel ) noexcept;

            /// Value type conversion.
            template< class U >
            operator Column< U >() const noexcept;

    };

}

#include "inline/C3_Column.hh"

#endif
