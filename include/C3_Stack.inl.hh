#include <algorithm>
#include <cassert>
#include <iostream>

// Create stack.

template< typename T >
inline C3::Stack< T > C3::Stack< T >::create( const size_t nimgs, const size_t ncols, const size_t nrows )
{
    return C3::Stack< T >( nimgs, ncols, nrows, new T [ nimgs * ncols * nrows ] );
}

// Create stack and initialize with a scalar value.

template< typename T >
inline C3::Stack< T > C3::Stack< T >::create( const size_t nimgs, const size_t ncols, const size_t nrows, const T scalar )
{
    C3::Stack< T > stack = C3::Stack< T >::create( nimgs, ncols, nrows );
    return stack = scalar;
}

// Constructor, copy.

template< typename T >
inline C3::Stack< T >::Stack( const C3::Stack< T >& stack ) :
    _nimgs(         stack._nimgs   ),
    _ncols(         stack._ncols   ),
    _nrows(         stack._nrows   ),
    _data ( new T [ stack.size() ] )
{
    std::copy( stack._data, stack._data + size(), _data );
}

// Destructor.

template< typename T >
inline C3::Stack< T >::~Stack()
{
    delete [] _data;
}

// Assignment, stack.

template< typename T >
inline C3::Stack< T >& C3::Stack< T >::operator = ( const C3::Stack< T >& stack )
{
    if( this != &stack )
    {
        assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
        std::copy( stack._data, stack._data + size(), _data );
    }
    return *this;
}

// Compound assignment, stack, addition.

template< typename T >
inline C3::Stack< T >& C3::Stack< T >::operator += ( const C3::Stack< T >& stack )
{
    assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
    std::transform( _data, _data + size(), stack._data, _data, std::plus< T >() );
    return *this;
}

// Compound assignment, stack, subtraction.

template< typename T >
inline C3::Stack< T >& C3::Stack< T >::operator -= ( const C3::Stack< T >& stack )
{
    assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
    std::transform( _data, _data + size(), stack._data, _data, std::minus< T >() );
    return *this;
}

// Compound assignment, stack, multiplication.

template< typename T >
inline C3::Stack< T >& C3::Stack< T >::operator *= ( const C3::Stack< T >& stack )
{
    assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
    std::transform( _data, _data + size(), stack._data, _data, std::multiplies< T >() );
    return *this;
}

// Compound assignment, stack, division.

template< typename T >
inline C3::Stack< T >& C3::Stack< T >::operator /= ( const C3::Stack< T >& stack )
{
    assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
    std::transform( _data, _data + size(), stack._data, _data, std::divides< T >() );
    return *this;
}

// Equality comparison, stack.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Stack< T >::operator == ( const C3::Stack< T >& stack ) const
{
    assert( _nimgs == stack._nimgs && _ncols == stack._ncols && _nrows == stack._nrows );
    return std::equal( _data, _data + size(), stack._data );
}

// Inequality comparison, stack.  Not guaranteed for floating point types.

template< typename T >
inline bool C3::Stack< T >::operator != ( const C3::Stack< T >& stack ) const
{
    return ! ( *this == stack );
}

// Cast to stack of another type.

template< typename T >
template< typename U >
inline C3::Stack< T >::operator C3::Stack< U > () const
{
    C3::Stack< U > stack = C3::Stack< U >::create( _nimgs, _ncols, _nrows );
    std::copy( _data, _data + size(), stack._data );
    return stack;
}

// Constructor.

template< typename T >
inline C3::Stack< T >::Stack( const size_t nimgs, const size_t ncols, const size_t nrows, T* data ) :
    _nimgs( nimgs ),
    _ncols( ncols ),
    _nrows( nrows ),
    _data ( data  )
{}

// Stream output.  Mostly for debugging.

template< typename T >
inline std::ostream& C3::operator << ( std::ostream& stream, const C3::Stack< T >& stack )
{
    for( size_t k = 0; k < stack._nrows; ++ k )
    {
        for( size_t j = 0; j < stack._ncols; ++ j ) 
        {
            for( size_t i = 0; i < stack._nimgs; ++ i ) stream << stack( i, j, k ) << " ";
            stream << std::endl;
        }
    }
    return stream;
}
