
#include <cassert>

#include "C3_Congruent.hh"

template< class Destination, class Source >
inline Destination& C3::assign( Destination& dest, const Source& src )
{
    assert( C3::congruent( dest, src ) );
    typename C3::OperationTraits< Destination, Source >::assignment_type assignment;
    return C3::Detail::assign( dest, src, assignment ); 
}

// Tag dispatch.

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, FillAssignment )
{
    return C3::Detail::Kernel::fill_assign( dest, src );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, CopyAssignment )
{
    return C3::Detail::Kernel::copy_assign( dest, src, 1 );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, ColumnFrameAssignment )
{
    return C3::Detail::Kernel::fill_assign( dest, src, dest.ncolumns() );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, ColumnStackAssignment )
{
    return C3::Detail::Kernel::fill_assign( dest, src, dest.nframes() * dest.ncolumns() );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, RowFrameAssignment )
{
    return C3::Detail::Kernel::copy_assign( dest, src, dest.nrows() );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, RowStackAssignment )
{
    using value_type = typename ContainerTraits< Source >::value_type;
    OwnedBlock< value_type > buffer( dest.nframes() * dest.ncolumns() );
    buffer = C3::Detail::Kernel::fill_assign( buffer, src, dest.nframes() );
    return C3::Detail::Kernel::copy_assign( dest, buffer, dest.nrows() );
}

template< class Destination, class Source >
inline Destination& C3::Detail::assign( Destination& dest, const Source& src, FrameStackAssignment )
{
    return C3::Detail::Kernel::fill_assign( dest, src, dest.nframes() );
}

// Simple fill-assignment kernel.  Fills a container by repeating a value.

template< class Destination, class Source >
inline Destination& C3::Detail::Kernel::fill_assign( Destination& dest, const Source& src )
{
    std::fill( dest.begin(), dest.end(), src );
    return dest;
}

// Chunk fill-assignment kernel.  Copy source to destination by repeating each
// element of source multiple times.  Size of destination must equal size of
// soruce times number of copies.

template< class Destination, class Source >
inline Destination& C3::Detail::Kernel::fill_assign( Destination& dest, const Source& src, const C3::size_type count )
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
inline Destination& C3::Detail::Kernel::copy_assign( Destination& dest, const Source& src, const C3::size_type count )
{
    typename Destination::value_type* begin = dest.begin();
    for( auto i = 0; i < count; ++ i )
    {
        std::copy( src.begin(), src.end(), begin );
        begin += src.size();
    }
    return dest;
}
