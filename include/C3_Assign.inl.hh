
#include <algorithm>
#include <cassert>

#include "C3_TypeTraits.hh"

// Assign source to destination, optimized on argument types.

template< class Destination, class Source >
inline Destination& C3::assign( Destination& dest, const Source& src )
{
    static_assert( C3::IsAssignable< Destination, Source >::value, "assignment between incompatible types" );
    return C3::Detail::Assign< Destination, Source, C3::IsCompatibleScalar< typename Destination::value_type, Source >::value >::assign( dest, src );
}

// Primary assignment template.  Covers Column->Frame, Column->Stack,
// and Frame->Stack.

template< class Destination, class Source, bool ScalarSource >
struct C3::Detail::Assign
{
    static Destination& assign( Destination& dest, const Source& src )
    { 
        assert( ! ( dest.size() % src.size() ) );
        return C3::Detail::fill_assign( dest, src, dest.size() / src.size() ); 
    }
};

// Scalar assignment template.  Covers Scalar->Column, Scalar->Row,
// Scalar->Frame, Scalar->Stack.

template< class Destination, class Source >
struct C3::Detail::Assign< Destination, Source, true >
{
    static Destination& assign( Destination& dest, const Source& src )
    { 
        return C3::Detail::fill_assign( dest, src );
    }
};

// Matching container types template.  Covers Column, Row, Frame, and Stack.

template< class Destination >
struct C3::Detail::Assign< Destination, Destination >
{
    static Destination& assign( Destination& dest, const Destination& src )
    { 
        assert( dest.size() == src.size() );
        return C3::Detail::copy_assign( dest, src, 1 ); 
    }
};

// Covers Row->Frame.

template< typename T >
struct C3::Detail::Assign< C3::Frame< T >, C3::Row< T > >
{
    static C3::Frame< T >& assign( C3::Frame< T >& dest, const C3::Row< T >& src )
    { 
        assert( dest.ncolumns() == src.ncolumns() );
        return C3::Detail::copy_assign( dest, src, dest.ncolumns() ); 
    }
};

// Covers Row->Stack.

template< typename T >
struct C3::Detail::Assign< C3::Stack< T >, C3::Row< T > >
{
    static C3::Stack< T >& assign( C3::Stack< T >& dest, const C3::Row< T >& src )
    { 
        assert( dest.ncolumns() == src.ncolumns() );
        C3::Block< T > tmp( dest.nframes() * dest.ncolumns() );
        return C3::Detail::copy_assign( dest, C3::Detail::fill_assign( tmp, src, dest.nframes() ), dest.nrows() );
    }
};

// --- All of the above functions should trickle down to one of the compute
// --- kernels below.

// Simple fill-assignment kernel.

template< class Destination >
inline Destination& C3::Detail::fill_assign( Destination& dest, const typename Destination::value_type src )
{
    std::fill( dest.begin(), dest.end(), src );
    return dest;
}

// Chunk fill-assignment kernel.  Copy source to destination by repeating each
// element of source multiple times.  Size of destination must equal size of
// soruce times number of copies.

template< class Destination, class Source >
inline Destination& C3::Detail::fill_assign( Destination& dest, const Source& src, const C3::size_type count )
{
    typename Destination::value_type* begin = dest.begin();
    typename Destination::value_type* end   = begin + count;
    for( auto element = src.begin(); element != src.end(); ++ element )
    {
        std::fill( begin, end, *element );
        begin += count;
        end   += count;
    }
    return dest;
}

// Chunk copy-assignment kernel.  Copy source to destination by repeating the
// source multiple times.  Size of destination must equal size of source times
// number of copies.

template< class Destination, class Source >
inline Destination& C3::Detail::copy_assign( Destination& dest, const Source& src, const C3::size_type count )
{
    typename Destination::value_type* begin = dest.begin();
    for( auto i = 0; i < count; ++ i )
    {
        std::copy( src.begin(), src.end(), begin );
        begin += src.size();
    }
    return dest;
}
