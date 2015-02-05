
// Reduction template: primary version.

template< template< typename > class O, typename T, typename U >
inline U C3::Reduce< O, T, U >::compute( const T& input )
{
    size_t size = input.size();
    U* tmp = new U [ size ];
    std::copy( input._data, input._data + size, tmp );
    U result = O< U >::compute( size, tmp );
    delete [] tmp;
    return result;
}

// Reduction template: overload for View input, otherwise the primary template.

template< template< typename > class O, typename T, typename U >
inline U C3::Reduce< O, C3::View< T >, U >::compute( const C3::View< T >& input )
{
    size_t size = input.size();
    U* tmp = new U [ size ];
    for( size_t k = 0; k < input.nrows(); ++ k )
    {
        const T* begin = input._data + k * input._stride;
        const T* end   = begin + input._ncols;
        std::copy( begin, end, tmp + k * input._ncols );
    }
    U result = O< U >::compute( size, tmp );
    delete [] tmp;
    return result;
}

// Reduction template: overload for Image input, Column output.

template< template< typename > class O, typename T, typename U >
inline C3::Column< U > C3::Reduce< O, C3::Image< T >, C3::Column< U > >::compute( const C3::Image< T >& input )
{
    size_t size = input.ncols();
    U* tmp = new U [ size ];
    C3::Column< U > result = C3::Column< U >::create( input.nrows() );
    for( size_t k = 0; k < input.nrows(); ++ k )
    {
        const T* begin = input._data + k * input._ncols;
        const T* end   = begin + input._ncols;
        std::copy( begin, end, tmp );
        result( k ) = O< U >::compute( size, tmp );
    }
    delete [] tmp;
    return result;
}

// Reduction template: overload for View input, Column output.

template< template< typename > class O, typename T, typename U >
inline C3::Column< U > C3::Reduce< O, C3::View< T >, C3::Column< U > >::compute( const C3::View< T >& input )
{
    size_t size = input.ncols();
    U* tmp = new U [ size ];
    C3::Column< U > result = C3::Column< U >::create( input.nrows() );
    for( size_t k = 0; k < input.nrows(); ++ k )
    {
        const T* begin = input._data + k * input._stride;   // only diff w/rt image (could have less lines of code)
        const T* end   = begin + input._ncols;
        std::copy( begin, end, tmp );
        result( k ) = O< U >::compute( size, tmp );
    }
    delete [] tmp;
    return result;
}

// Reduction template: overload for Image input, Row output.

template< template< typename > class O, typename T, typename U >
inline C3::Row< U > C3::Reduce< O, C3::Image< T >, C3::Row< U > >::compute( const C3::Image< T >& input )
{
    size_t size = input.nrows();
    U* tmp = new U [ size ];
    C3::Row< U > result = C3::Row< U >::create( input.ncols() );
    for( size_t j = 0; j < input.ncols(); ++ j )
    {
        for( size_t k = 0; k < input.nrows(); ++ k ) tmp[ k ] = input( j, k );
        result( j ) = O< U >::compute( size, tmp );
    }
    delete [] tmp;
    return result;
}

// Reduction template: overload for View input, Row output.

template< template< typename > class O, typename T, typename U >
inline C3::Row< U > C3::Reduce< O, C3::View< T >, C3::Row< U > >::compute( const C3::View< T >& input )
{
    // body is same as image so could have less lines again
    size_t size = input.nrows();
    U* tmp = new U [ size ];
    C3::Row< U > result = C3::Row< U >::create( input.ncols() );
    for( size_t j = 0; j < input.ncols(); ++ j )
    {
        for( size_t k = 0; k < input.nrows(); ++ k ) tmp[ k ] = input( j, k );
        result( j ) = O< U >::compute( size, tmp );
    }
    delete [] tmp;
    return result;
}

// Reduction template: overload for Stack input, Image output.

template< template< typename > class O, typename T, typename U >
inline C3::Image< U > C3::Reduce< O, C3::Stack< T >, C3::Image< U > >::compute( const C3::Stack< T >& input )
{
    size_t size = input.nimgs();
    U* tmp = new U [ size ];
    C3::Image< U > result = C3::Image< U >::create( input.ncols(), input.nrows() );
    for( size_t k = 0; k < input.nrows(); ++ k ) 
    {
        for( size_t j = 0; j < input.ncols(); ++ j )
        {
            const T* begin = input._data + input._ncols * ( j + input._nrows * k );
            const T* end   = begin + input._nimgs;
            std::copy( begin, end, tmp );
            result( j, k ) = O< U >::compute( size, tmp );
        }
    }
    delete [] tmp;
    return result;
}
