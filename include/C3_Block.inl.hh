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

// Cast to block of another type.

template< typename T >
template< typename U >
inline C3::Block< T >::operator C3::Block< U > () const
{
    C3::Block< U > block( _size );
    std::copy( _data, _data + _size, block.data() );
    return block;
}

// Assign a scalar to the entire block.

template< typename T >
inline void C3::Block< T >::assign( const T& source )
{
    std::fill( _data, _data + _size, source );
}

// Assign a scalar to a chunk of the block.

template< typename T >
inline void C3::Block< T >::assign( const T& source, const size_type offset, const size_type size )
{
    T* begin = _data + offset;
    T* end   = begin + size;
    assert( begin < this->end() );
    assert( end <= this->end()  );
    std::fill( begin, end, source );
}

// Assign a scalar to regularly spaced chunks of the block.

template< typename T >
inline void C3::Block< T >::assign( const T& source, const size_type offset, const size_type size, 
        const size_type stride, const size_type repeat )
{
    assert( size <= stride );
    T* begin = _data + offset;
    T* end   = begin + size;
    assert( begin < this->end() );
    assert( end + ( repeat - 1 ) * stride <= this->end() );
    for( size_type i = 0; i < repeat; ++ i )
    {
        std::fill( begin, end, source );
        begin += stride;
        end   += stride;
    }
}
