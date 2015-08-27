#ifndef C3_VIEW_HH
#define C3_VIEW_HH

#include "C3_Frame.hh"

namespace C3
{

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
            View( Frame< T >& frame, const size_type ncolumns, const size_type nrows, 
                    const size_type column_offset, const size_type row_offset ) noexcept;

            /// Number of columns and rows.
            ///@{
            size_type ncolumns() const { return _ncolumns; }
            size_type nrows()    const { return _nrows;    }
            ///@}

            /// Native C++ array access.
            /// @{
                  T* data()       { return _data; }
            const T* data() const { return _data; }
            /// @}

            /// Start of native C++ array.
            /// @{
                  T* begin()       { return data(); }
            const T* begin() const { return data(); }
            /// @}

            /// Referenced frame offset position and stride.
            ///@{
            size_type offset() const { return _offset; }
            size_type stride() const { return _stride; }
            ///@}

            /// Coordinate access.
            ///@{
            T& operator() ( const size_type j, const size_type k )       { return _begin[ j + _stride * k ]; }
            T  operator() ( const size_type j, const size_type k ) const { return _begin[ j + _stride * k ]; }
            ///@}

            /// Pixel assignment.
            View& operator = ( const T pixel ) noexcept;

        private :   // Private data members.

            T*              _data;      ///< Start of data array.
            size_type       _ncolumns;  ///< Total columns.
            size_type       _nrows;     ///< Total rows.
            size_type       _offset;    ///< Block start.
            size_type       _stride;    ///< Block stride.
            T*              _begin;     ///< Data plus offset.

    };

}

#include "inline/C3_View.hh"

#endif
