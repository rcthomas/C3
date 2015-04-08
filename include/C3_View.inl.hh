#include <algorithm>
#include <cassert>
#include <iostream>

// Create a view from an image and bounding box.

template< typename T >
inline C3::View< T > C3::View< T >::create( C3::Image< T >& image, 
        const size_t stcol, const size_t ncols, const size_t strow, const size_t nrows )
{
    return C3::View< T >( ncols, nrows, image._ncols, image.data() + stcol + strow * image._ncols );
}

// Constructor, copy.

template< typename T >
inline C3::View< T >::View( const C3::View< T >& view ) :
    _ncols ( view._ncols  ),
    _nrows ( view._nrows  ),
    _stride( view._stride ),
    _data  ( view._data   )
{}

// Destructor.

template< typename T >
inline C3::View< T >::~View()
{}

// Assignment, view.

template< typename T >
inline C3::View< T >& C3::View< T >::operator = ( const C3::View< T >& view )
{
    if( this != &view )
    {
        assert( _ncols == view._ncols && _nrows == view._nrows );
        for( size_t k = 0; k < _nrows; ++ k )
        {
            const T* begin = view._data + k * view._stride;
            const T* end   = begin + _ncols;
            std::copy( begin, end, _data + k * _stride );
        }
    }
    return *this;
}

// Compound assignment, view, addition.

template< typename T >
inline C3::View< T >& C3::View< T >::operator += ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + k * view._stride, begin, std::plus< T >() );
    }
    return *this;
}

// Compound assignment, view, subtraction.

template< typename T >
inline C3::View< T >& C3::View< T >::operator -= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + k * view._stride, begin, std::minus< T >() );
    }
    return *this;
}

// Compound assignment, view, multiplication.

template< typename T >
inline C3::View< T >& C3::View< T >::operator *= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + k * view._stride, begin, std::multiplies< T >() );
    }
    return *this;
}

// Compound assignment, view, division.

template< typename T >
inline C3::View< T >& C3::View< T >::operator /= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + k * view._stride, begin, std::divides< T >() );
    }
    return *this;
}

// Equality comparison, view.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator == ( const C3::View< T >& view ) const
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        if( std::equal( begin, end, view._data + k * view._stride ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, view.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator != ( const C3::View< T >& view ) const
{
    return ! ( *this == view );
}

// Assignment, image.

template< typename T >
inline C3::View< T >& C3::View< T >::operator = ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        const T* begin = image._data + k * _ncols;
        const T* end   = begin + _ncols;
        std::copy( begin, end, _data + k * _stride );
    }
    return *this;
}

// Compound assignment, image, addition.

template< typename T >
inline C3::View< T >& C3::View< T >::operator += ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, image._data + k * _ncols, begin, std::plus< T >() );
    }
    return *this;
}

// Compound assignment, image, subtraction.

template< typename T >
inline C3::View< T >& C3::View< T >::operator -= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, image._data + k * _ncols, begin, std::minus< T >() );
    }
    return *this;
}

// Compound assignment, image, multiplication.

template< typename T >
inline C3::View< T >& C3::View< T >::operator *= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, image._data + k * _ncols, begin, std::multiplies< T >() );
    }
    return *this;
}

// Compound assignment, image, division.

template< typename T >
inline C3::View< T >& C3::View< T >::operator /= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, image._data + k * _ncols, begin, std::divides< T >() );
    }
    return *this;
}

// Equality comparison, image.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator == ( const C3::Image< T >& image ) const
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        if( std::equal( begin, end, image._data + k * _ncols ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, image.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator != ( const C3::Image< T >& image ) const
{
    return ! ( *this == image );
}

// Assignment, column.

template< typename T >
inline C3::View< T >& C3::View< T >::operator = ( const C3::Column< T >& column )
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::fill( begin, end, column( k ) );
    }
    return *this;
}

// Compound assignment, column, addition.

template< typename T >
inline C3::View< T >& C3::View< T >::operator += ( const C3::Column< T >& column )
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::plus< T >(), column( k ) ) );
    }
    return *this;
}

// Compound assignment, column, subtraction.

template< typename T >
inline C3::View< T >& C3::View< T >::operator -= ( const C3::Column< T >& column )
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::minus< T >(), column( k ) ) );
    }
    return *this;
}

// Compound assignment, column, multiplication.

template< typename T >
inline C3::View< T >& C3::View< T >::operator *= ( const C3::Column< T >& column )
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::multiplies< T >(), column( k ) ) );
    }
    return *this;
}

// Compound assignment, column, division.

template< typename T >
inline C3::View< T >& C3::View< T >::operator /= ( const C3::Column< T >& column )
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::divides< T >(), column( k ) ) );
    }
    return *this;
}

// Equality comparison, column.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator == ( const C3::Column< T >& column ) const
{
    assert( _nrows == column.nrows() );
    for( size_t k = 0; k < _nrows; ++ k ) 
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        if( std::all_of( begin, end, std::bind2nd( std::equal_to< T >(), column( k ) ) ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, column.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator != ( const C3::Column< T >& column ) const
{
    return ! ( *this == column );
}

// Assignment, row.

template< typename T >
inline C3::View< T >& C3::View< T >::operator = ( const C3::Row< T >& row )
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k ) std::copy( row._data, row._data + _ncols, _data + k * _stride );
    return *this;
}

// Compound assignment, row, addition.

template< typename T >
inline C3::View< T >& C3::View< T >::operator += ( const C3::Row< T >& row )
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::plus< T >() );
    }
    return *this;
}

// Compound assignment, row, subtraction.

template< typename T >
inline C3::View< T >& C3::View< T >::operator -= ( const C3::Row< T >& row )
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::minus< T >() );
    }
    return *this;
}

// Compound assignment, row, multiplication.

template< typename T >
inline C3::View< T >& C3::View< T >::operator *= ( const C3::Row< T >& row )
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::multiplies< T >() );
    }
    return *this;
}

// Compound assignment, row, division.

template< typename T >
inline C3::View< T >& C3::View< T >::operator /= ( const C3::Row< T >& row )
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::divides< T >() );
    }
    return *this;
}

// Equality comparison, row.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator == ( const C3::Row< T >& row ) const
{
    assert( _ncols == row.ncols() );
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        if( std::equal( begin, end, row._data ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, row.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator != ( const C3::Row< T >& row ) const
{
    return ! ( *this == row );
}

// Assignment, scalar.

template< typename T >
inline C3::View< T >& C3::View< T >::operator = ( const T scalar )
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::fill( begin, end, scalar );
    }
    return *this;
}

// Compound assignment, scalar, addition.

template< typename T >
inline C3::View< T >& C3::View< T >::operator += ( const T scalar )
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, scalar, std::bind2nd( std::plus< T >(), scalar ) );
    }
    return *this;
}

// Compound assignment, scalar, subtraction.

template< typename T >
inline C3::View< T >& C3::View< T >::operator -= ( const T scalar )
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, scalar, std::bind2nd( std::minus< T >(), scalar ) );
    }
    return *this;
}

// Compound assignment, scalar, multiplication.

template< typename T >
inline C3::View< T >& C3::View< T >::operator *= ( const T scalar )
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, scalar, std::bind2nd( std::multiplies< T >(), scalar ) );
    }
    return *this;
}

// Compound assignment, scalar, division.

template< typename T >
inline C3::View< T >& C3::View< T >::operator /= ( const T scalar )
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        std::transform( begin, end, scalar, std::bind2nd( std::divides< T >(), scalar ) );
    }
    return *this;
}

// Equality comparison, scalar.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator == ( const T scalar ) const
{
    for( size_t k = 0; k < _nrows; ++ k )
    {
        T* begin = _data + k * _stride;
        T* end   = begin + _ncols;
        if( std::all_of( begin, end, bind2nd( std::equal_to< T >(), scalar ) ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, scalar.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::View< T >::operator != ( const T scalar ) const
{
    return ! ( *this == scalar );
}

// Constructor.

template< typename T >
inline C3::View< T >::View( const size_t ncols, const size_t nrows, const size_t stride, T* data ) :
    _ncols ( ncols  ),
    _nrows ( nrows  ),
    _stride( stride ),
    _data  ( data   )
{}

// Stream output.

template< typename T >
inline std::ostream& C3::operator << ( std::ostream& stream, const C3::View< T >& view )
{
    for( size_t k = 0; k < view._nrows; ++ k )
    {
        for( size_t j = 0; j < view._ncols; ++ j ) stream << view( j, k ) << " ";
        stream << std::endl;
    }
    return stream;
}
