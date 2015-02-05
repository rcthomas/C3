#include <cmath>
#include <numeric>

template< typename T >
inline T C3::Min< T >::compute( const size_t size, T* array )
{
    return *std::min_element( array, array + size );
}

template< typename T >
inline T C3::Max< T >::compute( const size_t size, T* array )
{
    return *std::max_element( array, array + size );
}

template< typename T >
inline T C3::Mean< T >::compute( const size_t size, T* array )
{
    return std::accumulate( array, array + size, static_cast< T >( 0 ) ) / double( size );
}

template< typename T >
inline T C3::Stdev< T >::compute( const size_t size, T* array )
{
    T mean = C3::Mean< T >::compute( size, array );
    std::transform( array, array + size, array, std::bind2nd( std::minus< T >(), mean ) );
    std::transform( array, array + size, array, array, std::multiplies< T >() );
    return sqrt( std::accumulate( array, array + size, static_cast< T >( 0 ) ) / double( size ) );
}

// FIXME This implementation is slower than I would like.
// Consider http://ndevilla.free.fr/median/median/index.html

template< typename T >
inline T C3::Median< T >::compute( const size_t size, T* array )
{
    std::sort( array, array + size );
    return array[ size / 2 ];
}

template< typename T >
inline T C3::Mad< T >::compute( const size_t size, T* array )
{
    T median = C3::Median< T >::compute( size, array );
    std::transform( array, array + size, array, std::bind2nd( std::minus< T >(), median ) );
    for( size_t i = 0; i < size; ++ i ) array[ i ] = fabs( array[ i ] );    // This seems to be really slow; so is transform version...?
    std::sort( array, array + size );
    return array[ size / 2 ];
}
