#include <algorithm>
#include <cassert>
#include <iostream>

// Create column.

template< typename T >
inline C3::Column< T > C3::Column< T >::create( const size_t size )
{
    return C3::Column< T >( size, new T [ size ] );
}

// Create column and initialize with a scalar value.

template< typename T >
inline C3::Column< T > C3::Column< T >::create( const size_t size, const T scalar )
{
    C3::Column< T > column = C3::Column< T >::create( size );
    return column = scalar;
}

// Constructor, copy.

template< typename T >
inline C3::Column< T >::Column( const C3::Column< T >& column ) :
    _size(         column._size   ),
    _data( new T [ column._size ] )
{
    std::copy( column._data, column._data + _size, _data );
}

// Destructor.

template< typename T >
inline C3::Column< T >::~Column()
{
    delete [] _data;
}

// Assignment, column.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator = ( const C3::Column< T >& column )
{
    if( this != &column )
    {
        assert( _size == column._size );
        std::copy( column._data, column._data + _size, _data );
    }
    return *this;
}

// Compound assignment, column, addition.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator += ( const C3::Column< T >& column )
{
    assert( _size == column._size );
    std::transform( _data, _data + _size, column._data, _data, std::plus< T >() );
    return *this;
}

// Compound assignment, column, subtraction.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator -= ( const C3::Column< T >& column )
{
    assert( _size == column._size );
    std::transform( _data, _data + _size, column._data, _data, std::minus< T >() );
    return *this;
}

// Compound assignment, column, multiplication.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator *= ( const C3::Column< T >& column )
{
    assert( _size == column._size );
    std::transform( _data, _data + _size, column._data, _data, std::multiplies< T >() );
    return *this;
}

// Compound assignment, column, division.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator /= ( const C3::Column< T >& column )
{
    assert( _size == column._size );
    std::transform( _data, _data + _size, column._data, _data, std::divides< T >() );
    return *this;
}

// Equality comparison, column, not guaranteed for floating point types.

template< typename T >
inline bool C3::Column< T >::operator == ( const C3::Column< T >& column ) const
{
    assert( _size == column._size );
    return std::equal( _data, _data + _size, column._data );
}

// Inquality comparison, column, not guaranteed for floating point types.

template< typename T >
inline bool C3::Column< T >::operator != ( const C3::Column< T >& column ) const
{
    return ! ( *this == column );
}

// Assignment, scalar.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator = ( const T scalar )
{
    std::fill( _data, _data + _size, scalar );
    return *this;
}

// Compound assignment, scalar, addition.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator += ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::plus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, subtraction.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator -= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::minus< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, multiplication.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator *= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::multiplies< T >(), scalar ) );
    return *this;
}

// Compound assignment, scalar, division.

template< typename T >
inline C3::Column< T >& C3::Column< T >::operator /= ( const T scalar )
{
    std::transform( _data, _data + _size, _data, std::bind2nd( std::divides< T >(), scalar ) );
    return *this;
}

// Equality comparison, scalar, not guaranteed for floating point types.

template< typename T >
inline bool C3::Column< T >::operator == ( const T scalar ) const
{
    return std::all_of( _data, _data + _size, std::bind2nd( std::equal_to< T >(), scalar ) );
}

// Inequality comparison, scalar, not guaranteed for floating point types.

template< typename T >
inline bool C3::Column< T >::operator != ( const T scalar ) const
{
    return ! ( *this == scalar );
}

// Cast to column of another type.

template< typename T >
template< typename U >
inline C3::Column< T >::operator C3::Column< U > () const
{
    C3::Column< U > column = C3::Column< U >::create( _size );
    std::copy( _data, _data + _size, column._data );
    return column;
}

// Constructor.

template< typename T >
inline C3::Column< T >::Column( const size_t size, T* data ) :
    _size( size ),
    _data( data )
{}

// Stream output.

template< typename T >
inline std::ostream& C3::operator << ( std::ostream& stream, const C3::Column< T >& column )
{
    for( size_t k = 0; k < column._size; ++ k ) stream << column( k ) << std::endl;
    return stream;
}
