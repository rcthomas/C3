
// Constructor.

template< class T >
inline C3::Stack< T >::Stack( const C3::size_type nframes, const C3::size_type ncolumns, 
        const C3::size_type nrows ) noexcept : 
    C3::Block< T >( nframes * ncolumns * nrows ), _nframes( nframes ), _ncolumns( ncolumns ), _nrows( nrows ) 
{}

// Initializing constructor.

template< class T >
inline C3::Stack< T >::Stack( const C3::size_type nframes, const C3::size_type ncolumns, 
        const C3::size_type nrows, const T pixel ) noexcept : 
    C3::Block< T >( nframes * ncolumns * nrows, pixel ), _nframes( nframes ), _ncolumns( ncolumns ), _nrows( nrows ) 
{}

// Pixel assignment.

template< class T >
inline C3::Stack< T >& C3::Stack< T >::operator = ( const T pixel ) noexcept
{
    return C3::assign( *this, pixel );
}

// Value type conversion.

template< class T >
template< class U >
inline C3::Stack< T >::operator C3::Stack< U >() const noexcept
{
    C3::Stack< U > stack( nframes(), ncolumns(), nrows() );
    return C3::assign( stack, *this );
}
