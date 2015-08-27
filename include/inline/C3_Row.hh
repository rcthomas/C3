
// Constructor.

template< class T >
inline C3::Row< T >::Row( const C3::size_type ncolumns ) noexcept : 
    C3::Block< T >( ncolumns ) 
{}

// Initializing constructor.

template< class T >
inline C3::Row< T >::Row( const C3::size_type ncolumns, const T pixel ) noexcept : 
    C3::Block< T >( ncolumns, pixel ) 
{}

// Pixel assignment.

template< class T >
inline C3::Row< T >& C3::Row< T >::operator = ( const T pixel ) noexcept
{
    return C3::assign( *this, pixel );
}

// Value type conversion.

template< class T >
template< class U >
inline C3::Row< T >::operator C3::Row< U >() const noexcept
{
    C3::Row< U > row( ncolumns() );
    return C3::assign( row, *this );
}
