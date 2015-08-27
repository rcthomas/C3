
// IRAF-style section definitions.

template< class T >
inline C3::View< T > C3::View< T >::iraf_style( C3::Frame< T >& frame, 
        const size_type first_column, const size_type final_column   , 
        const size_type first_row   , const size_type final_row      )
{
    auto column_offset = first_column - 1;
    auto row_offset    = first_row    - 1;
    auto ncolumns = final_column - column_offset;
    auto nrows    = final_row    - row_offset;
    return C3::View< T >( frame, ncolumns, nrows, column_offset, row_offset );
}

// Constructor.

template< class T >
inline C3::View< T >::View( Frame< T >& frame, const size_type ncolumns, const size_type nrows, 
        const size_type column_offset, const size_type row_offset ) noexcept :
    _data( frame.data() ), _ncolumns( ncolumns ), _nrows( nrows ), 
    _offset( column_offset + frame.ncolumns() * row_offset ), _stride( frame.ncolumns() ),
    _begin( _data + _offset )
{}

// Pixel assignment.

template< class T >
inline C3::View< T >& C3::View< T >::operator = ( const T pixel ) noexcept
{
    C3::assign( *this, pixel );
    return *this;
}
