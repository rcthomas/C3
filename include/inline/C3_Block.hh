
#include "../C3_Assign.hh"

// Constructor.

template< class T >
inline C3::Block< T >::Block( const C3::size_type size ) noexcept :
    _size( size ), _data( new T [ size ] )
{}

// Initializing constructor.

template< class T >
inline C3::Block< T >::Block( const C3::size_type size, const T pixel ) noexcept :
    _size( size ), _data( new T [ size ] )
{
    C3::assign( *this, pixel );
}

// Copy constructor.

template< class T >
inline C3::Block< T >::Block( const C3::Block< T >& block ) noexcept :
    _size( block.size() ), _data( new T [ block.size() ] )
{
    C3::assign( *this, block );
}

// Move constructor.

template< class T >
inline C3::Block< T >::Block( C3::Block< T >&& block ) noexcept :
    _size( block.size() ), _data( block.data() )
{
    block._size = 0;
    block._data = nullptr;
}

// Copy assignment.  Copying pixels from a block with one size to a block with
// a different size has undefined behavior with probably bad consequences.
// When C3 applications are compiled with the DEBUG flag, such a condition
// breaks an assertion.

template< class T >
inline C3::Block< T >& C3::Block< T >::operator = ( const C3::Block< T >& block ) noexcept
{
    return this != &block ? C3::assign( *this, block ) : *this;
}

// Move assignment.

template< class T >
inline C3::Block< T >& C3::Block< T >::operator = ( C3::Block< T >&& block ) noexcept
{
    if( this != &block )
    {
        _size = block.size();
        _data = block.data();
        block._size = 0;
        block._data = nullptr;
    }
    return *this;
}

// Pixel assignment.

template< class T >
inline C3::Block< T >& C3::Block< T >::operator = ( const T pixel ) noexcept 
{
    return C3::assign( *this, pixel );
}

// Overload conversion to block of another type.

template< class T >
template< class U >
inline C3::Block< T >::operator C3::Block< U >() const noexcept 
{
    C3::Block< U > block( size() );
    return C3::assign( block, *this );
}

// Destructor.

template< class T >
inline C3::Block< T >::~Block()
{
    delete [] _data;
}
