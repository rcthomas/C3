#include <algorithm>
#include <cassert>
#include <iostream>

// Create image.

template< typename T >
inline C3::Image< T > C3::Image< T >::create( const size_t ncols, const size_t nrows )
{
    return C3::Image< T >( ncols, nrows, new T [ ncols * nrows ] );
}

// Create image and initialize with a scalar value.

template< typename T >
inline C3::Image< T > C3::Image< T >::create( const size_t ncols, const size_t nrows, const T scalar )
{
    C3::Image< T > image = C3::Image< T >::create( ncols, nrows );
    return image = scalar;
}

// Constructor.

template< typename T >
inline C3::Image< T >::Image( const size_t ncols, const size_t nrows, T* data ) :
    _ncols( ncols ),
    _nrows( nrows ),
    _data ( data  )
{}

// Constructor, copy.

template< typename T >
inline C3::Image< T >::Image( const C3::Image< T >& image ) :
    _ncols(         image._ncols   ),
    _nrows(         image._nrows   ),
    _data ( new T [ image.size() ] )
{
    std::copy( image._data, image._data + size(), _data );
}

// Destructor.

template< typename T >
inline C3::Image< T >::~Image()
{
    delete [] _data;
}

// Assignment, image.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator = ( const C3::Image< T >& image )
{
    if( this != &image )
    {
        assert( _ncols == image._ncols && _nrows == image._nrows );
        std::copy( image._data, image._data + size(), _data );
    }
    return *this;
}

// Compound assignment, image, addition.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator += ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    std::transform( _data, _data + size(), image._data, _data, std::plus< T >() );
    return *this;
}

// Compound assignment, image, subtraction.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator -= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    std::transform( _data, _data + size(), image._data, _data, std::minus< T >() );
    return *this;
}

// Compound assignment, image, multiplication.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator *= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    std::transform( _data, _data + size(), image._data, _data, std::multiplies< T >() );
    return *this;
}

// Compound assignment, image, division.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator /= ( const C3::Image< T >& image )
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    std::transform( _data, _data + size(), image._data, _data, std::divides< T >() );
    return *this;
}

// Equality comparison, image.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator == ( const C3::Image< T >& image ) const
{
    assert( _ncols == image._ncols && _nrows == image._nrows );
    return std::equal( _data, _data + size(), image._data );
}

// Inequality comparison, image.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator != ( const C3::Image< T >& image ) const
{
    return ! ( *this == image );
}

// Assignment, column.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator = ( const C3::Column< T >& column )
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::fill( begin, end, column( j ) );
    }
    return *this;
}

// Compound assignment, column, addition.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator += ( const C3::Column< T >& column )
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::plus< T >(), column( j ) ) );
    }
    return *this;
}

// Compound assignment, column, subtraction.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator -= ( const C3::Column< T >& column )
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::minus< T >(), column( j ) ) );
    }
    return *this;
}

// Compound assignment, column, multiplication.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator *= ( const C3::Column< T >& column )
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::multiplies< T >(), column( j ) ) );
    }
    return *this;
}

// Compound assignment, column, division.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator /= ( const C3::Column< T >& column )
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, begin, std::bind2nd( std::divides< T >(), column( j ) ) );
    }
    return *this;
}

// Equality comparison, column.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator == ( const C3::Column< T >& column ) const
{
    assert( _nrows == column._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        if( std::all_of( begin, end, std::bind2nd( std::equal_to< T >(), column( j ) ) ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, column.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator != ( const C3::Column< T >& column ) const
{
    return ! ( *this == column );
}

// Assignment, row.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator = ( const C3::Row< T >& row )
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) std::copy( row._data, row._data + _ncols, _data + j * _ncols );
    return *this;
}

// Compound assignment, row, addition.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator += ( const C3::Row< T >& row )
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::plus< T >() );
    }
    return *this;
}

// Compound assignment, row, subtraction.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator -= ( const C3::Row< T >& row )
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::minus< T >() );
    }
    return *this;
}

// Compound assignment, row, multiplication.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator *= ( const C3::Row< T >& row )
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::multiplies< T >() );
    }
    return *this;
}

// Compound assignment, row, division.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator /= ( const C3::Row< T >& row )
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, row._data, begin, std::divides< T >() );
    }
    return *this;
}

// Equality comparison, row.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator == ( const C3::Row< T >& row ) const
{
    assert( _ncols == row._ncols );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        if( std::equal( begin, end, row._data ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, row.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator != ( const C3::Row< T >& row ) const
{
    return ! ( *this == row );
}

// Assignment, scalar.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator = ( const T scalar )
{
    std::fill( _data, _data + size(), scalar );
    return *this;
}

// Compound assignment, scalar, addition.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator += ( const T scalar )
{
    std::transform( _data, _data + size, _data, std::bind2nd( std::plus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, subtraction.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator -= ( const T scalar )
{
    std::transform( _data, _data + size, _data, std::bind2nd( std::minus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, multiplication.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator *= ( const T scalar )
{
    std::transform( _data, _data + size, _data, std::bind2nd( std::multiplies< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, division.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator /= ( const T scalar )
{
    std::transform( _data, _data + size, _data, std::bind2nd( std::divides< T >(), scalar ) );
    return *this;
}

// Equality comparison, scalar.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator == ( const T scalar ) const
{
    return std::all_of( _data, _data + size(), std::bind2nd( std::equal_to< T >(), scalar ) );
}

// Inequality comparison, scalar.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator != ( const T scalar ) const
{
    return ! ( *this == scalar );
}

// Assignment, view.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator = ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        const T* begin = view._data + j * view._stride;
        const T* end   = begin + _ncols;
        std::copy( begin, end, _data + j * _ncols );
    }
    return *this;
}

// Compound assignment, view, addition.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator += ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + j * view._stride, begin, std::plus< T >() );
    }
    return *this;
}

// Compound assignment, view, subtraction.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator -= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + j * view._stride, begin, std::minus< T >() );
    }
    return *this;
}

// Compound assignment, view, multiplication.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator *= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + j * view._stride, begin, std::multiplies< T >() );
    }
    return *this;
}

// Compound assignment, view, division.

template< typename T >
inline C3::Image< T >& C3::Image< T >::operator /= ( const C3::View< T >& view )
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j )
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        std::transform( begin, end, view._data + j * view._stride, begin, std::divides< T >() );
    }
    return *this;
}

// Equality comparison, view.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator == ( const C3::View< T >& view ) const
{
    assert( _ncols == view._ncols && _nrows == view._nrows );
    for( size_t j = 0; j < _nrows; ++ j ) 
    {
        T* begin = _data + j * _ncols;
        T* end   = begin + _ncols;
        if( std::equal( begin, end, view._data + j * view._stride ) ) continue;
        return false;
    }
    return true;
}

// Inequality comparison, view.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Image< T >::operator != ( const C3::View< T >& view ) const
{
    return ! ( *this == view );
}

// Cast to image of another type.

template< typename T >
template< typename U >
inline C3::Image< T >::operator C3::Image< U > () const
{
    C3::Image< U > image = C3::Image< U >::create( _ncols, _nrows );
    std::copy( _data, _data + size(), image._data );
    return image;
}

// Stream output.

template< typename T >
inline std::ostream& C3::operator << ( std::ostream& stream, const C3::Image< T >& image )
{
    for( size_t k = 0; k < image._nrows; ++ k )
    {
        for( size_t j = 0; j < image._ncols; ++ j ) stream << image( j, k ) << " ";
        stream << std::endl;
    }
    return stream;
}
