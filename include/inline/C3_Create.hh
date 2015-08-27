
// Internal implementation declarations.

namespace C3
{
    template< class T, class U >  Block< T > _create( const  Block< U >& src );
    template< class T, class U > Column< T > _create( const Column< U >& src );
    template< class T, class U >    Row< T > _create( const    Row< U >& src );
    template< class T, class U >  Frame< T > _create( const  Frame< U >& src );
    template< class T, class U >  Frame< T > _create( const   View< U >& src );
    template< class T, class U >  Stack< T > _create( const  Stack< U >& src );
}

// From prototype instance with same value type.

template< class Container >
inline Container C3::create( const Container& src )
{
    using value_type = typename C3::ValueType< Container >::type;
    return C3::_create< value_type >( src );
}

// From prototype instance with possibly different value type.

template< class T, template< class > class Container, class U >
inline Container< T > C3::create( const Container< U >& src )
{
    return C3::_create< T >( src );
}

// Block.

template< class T, class U >
inline C3::Block< T > C3::_create( const C3::Block< U >& src )
{
    return C3::Block< T >( src.size() );
}

// Column.

template< class T, class U >
inline C3::Column< T > C3::_create( const C3::Column< U >& src )
{
    return C3::Column< T >( src.size() );
}

// Row.

template< class T, class U >
inline C3::Row< T > C3::_create( const C3::Row< U >& src )
{
    return C3::Row< T >( src.size() );
}

// Frame from frame.

template< class T, class U >
inline C3::Frame< T > C3::_create( const C3::Frame< U >& src )
{
    return C3::Frame< T >( src.ncolumns(), src.nrows() );
}

// Frame from view.

template< class T, class U >
inline C3::Frame< T > C3::_create( const C3::View< U >& src )
{
    return C3::Frame< T >( src.ncolumns(), src.nrows() );
}

// Stack.

template< class T, class U >
inline C3::Stack< T > C3::_create( const C3::Stack< U >& src )
{
    return C3::Stack< T >( src.nframes(), src.ncolumns(), src.nrows() );
}
