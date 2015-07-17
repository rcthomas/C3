
// IRAF-style section definitions.

template< typename T >
C3::View< T > C3::View< T >::iraf_style( C3::Frame< T >& frame, const size_type first_column, const size_type final_column, 
        const size_type first_row, const size_type final_row )
{
    auto column_offset = first_column - 1;
    auto row_offset    = first_row    - 1;
    auto ncolumns = final_column - column_offset;
    auto nrows    = final_row    - row_offset;
    return C3::View< T >( frame, ncolumns, nrows, column_offset, row_offset );
}
