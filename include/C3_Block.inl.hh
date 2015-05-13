#include <algorithm>

// Constructor for uninitialized block.

template< typename T >
inline C3::Block< T >::Block( const size_type size ) noexcept :
    _size(         size   ),
    _data( new T [ size ] )
{}

// Deep copy constructor.

template< typename T >
inline C3::Block< T >::Block( const C3::Block< T >& block ) noexcept :
    _size(         block._size   ),
    _data( new T [ block._size ] )
{
    std::copy( _data, _data + _size, block._data );
}

// Move constructor.

template< typename T >
inline C3::Block< T >::Block( C3::Block< T >&& block ) noexcept :
    _size(       0 ),
    _data( nullptr )
{
    *this = std::move( block );
}

// Deep copy assignment operator.

template< typename T >
inline C3::Block< T >& C3::Block< T >::operator = ( const C3::Block< T >& block ) noexcept 
{
    if( this != &block )
    {
        T* data = new T [ block._size ];
        std::copy( data, data + block._size, block._data );
        delete [] data;
        _size = block._size;
        _data = data;
    }
    return *this;
}

// Move assignment operator.

template< typename T >
inline C3::Block< T >& C3::Block< T >::operator = ( C3::Block< T >&& block ) noexcept
{
    if( this != &block )
    {
        delete [] _data;
        _size = block._size;
        _data = block._data;
        block._size = 0;
        block._data = nullptr;
    }
    return *this;
}

// Destructor.

template< typename T >
inline C3::Block< T >::~Block()
{
    delete [] _data;
}
