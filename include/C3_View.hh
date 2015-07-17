#ifndef C3_VIEW_HH
#define C3_VIEW_HH

#include "C3_AliasBlock.hh"

namespace C3
{

    template< class T > class Frame;

    /// @class View
    /// @brief Reference to rectangular section of a frame.

    template< class T >
    class View 
    {

        public :    // Public methods.

            /// IRAF-style section definitions.
            static View iraf_style( Frame< T >& frame, const size_type first_column, const size_type final_column,
                    const size_type first_row, const size_type final_row );

            /// Constructor.
            View( Frame< T >& frame, const size_type ncolumns, const size_type nrows, const size_type column_offset, const size_type row_offset ) noexcept :
                _block( frame.block() ), _ncolumns( ncolumns ), _nrows( nrows ), 
                _offset( column_offset + frame.ncolumns() * row_offset ), _stride( frame.ncolumns() ) {}

            /// Underlying block.
            ///@{
                  AliasBlock< T >& block()       { return _block; }
            const AliasBlock< T >& block() const { return _block; }
            ///@}

            /// Number of columns and rows.
            ///@{
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Referenced frame offset position and stride.
            ///@{
            size_type offset() const { return _offset; }
            size_type stride() const { return _stride; }
            ///@}

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j, const size_type k )       { return _block[ _offset + j + _stride * k ]; }
            T  operator() ( const size_type j, const size_type k ) const { return _block[ _offset + j + _stride * k ]; }
            ///@}

        private :   // Private data members.

            AliasBlock< T > _block;     ///< Content.
            size_type       _ncolumns;  ///< Total columns.
            size_type       _nrows;     ///< Total rows.
            size_type       _offset;    ///< Block start.
            size_type       _stride;    ///< Block stride.

    };

}

#include "inline/C3_View.hh"

#endif
