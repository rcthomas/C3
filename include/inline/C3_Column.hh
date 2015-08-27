
// Construction.

template< class T >
inline C3::Column< T >::Column( const C3::size_type nrows ) noexcept : 
    C3::Block< T >( nrows ) 
{}

// Initializing constructor.

template< class T >
inline C3::Column< T >::Column( const C3::size_type nrows, const T pixel ) noexcept : 
    C3::Block< T >( nrows, pixel ) 
{}

// Pixel assignment.

template< class T >
inline C3::Column< T >& C3::Column< T >::operator = ( const T pixel ) noexcept
{
    return C3::assign( *this, pixel );
}

// Value type conversion.

template< class T >
template< class U >
inline C3::Column< T >::operator C3::Column< U >() const noexcept
{
    C3::Column< U > column( nrows() );
    return C3::assign( column, *this );
}
