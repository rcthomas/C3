#include <algorithm>
#include <cassert>
#include <iostream>

// Create a row.

template< typename T >
inline C3::Row< T > C3::Row< T >::create( const size_t size )
{
    return C3::Row< T >( size, new T [ size ] );
}

// Create a row and initialize all pixels to a scalar value.

template< typename T >
inline C3::Row< T > C3::Row< T >::create( const size_t size, const T scalar )
{
    C3::Row< T > row = C3::Row< T >::create( size );
    return row = scalar;
}

// Constructor.

template< typename T >
inline C3::Row< T >::Row( const size_t size, T* data ) :
    _size( size ),
    _data( data )
{}

// Constructor, copy.

template< typename T >
inline C3::Row< T >::Row( const C3::Row< T >& row ) :
    _size(         row._size   ),
    _data( new T [ row._size ] )
{
    std::copy( row._data, row._data + _size, _data );
}

// Destructor.

template< typename T >
inline C3::Row< T >::~Row()
{
    delete [] _data;
}

// Assignment, row.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator = ( const C3::Row< T >& row )
{
    if( this != &row )
    {
        assert( _size == row._size );
        std::copy( row._data, row._data + _size, _data );
    }
    return *this;
}

// Compound assignment, row, addition.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator += ( const C3::Row< T >& row )
{
    assert( _size == row._size );
    std::transform( _data, _data + _size, row._data, _data, std::plus< T >() );
    return *this;
}

// Compound assignment, row, subtraction.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator -= ( const C3::Row< T >& row )
{
    assert( _size == row._size );
    std::transform( _data, _data + _size, row._data, _data, std::minus< T >() );
    return *this;
}

// Compound assignment, row, multiplication.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator *= ( const C3::Row< T >& row )
{
    assert( _size == row._size );
    std::transform( _data, _data + _size, row._data, _data, std::multiplies< T >() );
    return *this;
}

// Compound assignment, row, division.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator /= ( const C3::Row< T >& row )
{
    assert( _size == row._size );
    std::transform( _data, _data + _size, row._data, _data, std::divides< T >() );
    return *this;
}

// Equality comparison, row, not guaranteed for floating point types.

template< typename T >
inline bool C3::Row< T >::operator == ( const C3::Row< T >& row ) const
{
    assert( _size == row._size );
    return std::equal( _data, _data + _size, row._data );
}

// Inquality comparison, row, not guaranteed for floating point types.

template< typename T >
inline bool C3::Row< T >::operator != ( const C3::Row< T >& row ) const
{
    return ! ( *this == row );
}

// Assignment, scalar.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator = ( const T scalar )
{
    std::fill( _data, _data + _size, scalar );
    return *this;
}

// Compound assignment, scalar, addition.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator += ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::plus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, subtraction.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator -= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::minus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, multiplication.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator *= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::multiplies< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, division.

template< typename T >
inline C3::Row< T >& C3::Row< T >::operator /= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::divides< T >(), scalar ) );
    return *this;
}

// Equality comparison, scalar, not guaranteed for floating point types.

template< typename T >
inline bool C3::Row< T >::operator == ( const T scalar ) const
{
    return std::all_of( _data, _data + _size, std::bind2nd( std::equal_to< T >(), scalar ) );
}

// Inequality comparison, scalar, not guaranteed for floating point types.

template< typename T >
inline bool C3::Row< T >::operator != ( const T scalar ) const
{
    return ! ( *this == scalar );
}

// Cast to row of another type.

template< typename T >
template< typename U >
inline C3::Row< T >::operator C3::Row< U > () const
{
    C3::Row< U > row = C3::Row< U >::create( _size );
    std::copy( _data, _data + _size, row._data );
    return row;
}

// Stream output.

template< typename T >
inline std::ostream& C3::operator << ( std::ostream& stream, const C3::Row< T >& row )
{
    for( size_t j = 0; j < row._size; ++ j ) stream << row( j ) << " ";
    stream << std::endl;
    return stream;
}
