
// Constructor.

template< class T >
inline C3::Frame< T >::Frame( const C3::size_type ncolumns, const C3::size_type nrows ) noexcept : 
    C3::Block< T >( ncolumns * nrows ), _ncolumns( ncolumns ), _nrows( nrows ) 
{}

// Initializing constructor.

template< class T >
inline C3::Frame< T >::Frame( const C3::size_type ncolumns, const C3::size_type nrows, const T pixel ) noexcept : 
    C3::Block< T >( ncolumns * nrows, pixel ), _ncolumns( ncolumns ), _nrows( nrows ) 
{}

// Pixel assignment.

template< class T >
inline C3::Frame< T >& C3::Frame< T >::operator = ( const T pixel ) noexcept
{
    return C3::assign( *this, pixel );
}

// Value type conversion.

template< class T >
template< class U >
inline C3::Frame< T >::operator C3::Frame< U >() const noexcept
{
    C3::Frame< U > frame( ncolumns(), nrows() );
    return C3::assign( frame, *this );
}
