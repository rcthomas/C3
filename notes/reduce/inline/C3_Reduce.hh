
#include "../C3_Column.hh"
#include "../C3_Frame.hh"
#include "../C3_Row.hh"
#include "../C3_Stack.hh"
#include "../C3_TypeTraits.hh"
#include "../C3_View.hh"

// Internal implementation declarations.

namespace C3
{

    // Creates requested container with right dimensions for reduction operation.

    template< class Destination >
    struct _CreateReduce;

    // Reduce Driver Declarations
    // --------------------------
    // Invoked directly by public reduce() functions, reduce drivers generally strip containers down to block
    // descriptions and pass these to the right reduce pattern along with the reduce operator, supplied by the caller.  

    // Pixel from Column, Row, Frame, or Stack.
    template< class Destination, class Source, class Operator >
    Destination& _reduce( Destination& dest, const Source& src, Operator op );
    
    // Pixel from View.
    template< class Destination, class T, class Operator >
    Destination& _reduce( Destination& dest, const View< T >& src, Operator op );

    // Column from Frame.
    template< class T, class U, class Operator >
    Column< T >& _reduce( Column< T >& dest, const Frame< U >& src, Operator op);
    
    // Column from View.
    template< class T, class U, class Operator >
    Column< T >& _reduce( Column< T >& dest, const View< U >& src, Operator op );
    
    // Row from Frame.
    template< class T, class U, class Operator >
    Row< T >& _reduce( Row< T >& dest, const Frame< U >& src, Operator op );
    
    // Row from View.
    template< class T, class U, class Operator >
    Row< T >& _reduce( Row< T >& dest, const View< U >& src, Operator op );
    
    // Frame from Stack.
    template< class T, class U, class Operator >
    Frame< T >& _reduce( Frame< T >& dest, const Stack< U >& src, Operator op );

    // View from Stack.
    template< class T, class U, class Operator >
    View< T >& _reduce( View< T >& dest, const Stack< U >& src, Operator op );

    // Reduce Pattern Declarations
    // ---------------------------
    // Reduce pattern functions handle iteration over pixels or blocks and apply reduce operators to pixel ranges.
    // Specialized or optimized versions may be developed, and driver routines could be modified to take a template
    // parameter enabling selection at compilation.  Not clear it is necessary.

    // Pixel from contiguous block.
    template< class T, class U, class Operator >
    void _reduce_pixel_contiguous( T& dest, const Block< U >& src, Operator op );

    // Contiguous block from strided block.
    template< class T, class U, class Operator >
    void _reduce_contiguous_strided( Block< T >& dest, const Block< U >& src, Operator op,
            const size_type offset, const size_type length, const size_type stride );

    // Row from Frame or View.
    template< class T, class Source, class Operator >
    void _reduce_row_2d( Row< T >& dest, const Source& src, Operator op );

    // Strided block from larger strided block.
    template< class T, class U, class Operator >
    void _reduce_strided_strided( Block< T >& dest, const Block< U >& src, Operator op,
            const size_type  src_offset, const size_type  src_length, const size_type  src_stride,
            const size_type dest_offset, const size_type dest_length, const size_type dest_stride, 
            const size_type dest_count );

}

namespace C3{

template< class Destination, class Source >
struct CanReduce
{

    static const bool  src_container  = C3::IsContainer< Source >::value;
    static const bool dest_pixel      = C3::IsPixel< Destination >::value;
    static const bool  src_frame_view = C3::IsFrame< Source >::value || C3::IsView< Source >::value;
    static const bool dest_column_row = C3::IsColumn< Destination >::value || C3::IsRow< Destination >::value;
    static const bool  src_stack      = C3::IsStack< Source >::value;
    static const bool dest_frame_view = C3::IsFrame< Destination >::value || C3::IsView< Destination >::value;

    static const bool value = ( src_container && dest_pixel )
        || ( src_frame_view && dest_column_row )
        || ( src_stack && dest_frame_view );
    
};

}

template< template< class > class Destination, template< class > class Source, class T, class Operator >
inline Destination< T > C3::reduce( const Source< T >& src, Operator op )
{
    static_assert( C3::CanReduce< Destination< T >, Source< T > >::value, "NO" );
    auto dest = C3::_CreateReduce< Destination< T > >::create( src );
    return C3::_reduce( dest, src, op );
}

template< template< class > class Source, class T, class Operator >
inline T C3::reduce( const Source< T >& src, Operator op )
{
    static_assert( C3::CanReduce< T, Source< T > >::value, "NO" );
    auto dest = T();
    return C3::_reduce( dest, src, op );
}

template< template< class > class Destination, class T, template< class > class Source, class U, class Operator >
inline Destination< T > C3::reduce( const Source< U >& src, Operator op )
{
    static_assert( C3::CanReduce< Destination< T >, Source< U > >::value, "NO" );
    auto dest = C3::_CreateReduce< Destination< T > >::create( src );
    return C3::_reduce( dest, src, op );
}

template< class T, template< class > class Source, class U, class Operator >
inline T C3::reduce( const Source< U >& src, Operator op )
{
    static_assert( C3::CanReduce< T, Source< U > >::value, "NO" );
    auto dest = T();
    return C3::_reduce( dest, src, op );
}

template< class Destination, class Source, class Operator >
inline Destination& C3::reduce( Destination& dest, const Source& src, Operator op )
{
    static_assert( C3::CanReduce< Destination, Source >::value, "NO" );
    assert( C3::congruent( dest, src ) );
    return C3::_reduce( dest, src, op );
}

// Reduce Destination Creator Definition
// -------------------------------------

// Primary template.

template< class Destination >
struct C3::_CreateReduce {};

// View or frame to column.

template< class T >
struct C3::_CreateReduce< C3::Column< T > >
{
    template< class Source >
    static C3::Column< T > create( const Source& src ) { return C3::Column< T >( src.nrows() ); }
};

template< class T >
struct C3::_CreateReduce< C3::Row< T > >
{
    template< class Source >
    static C3::Row< T > create( const Source& src ) { return C3::Row< T >( src.ncolumns() ); }
};

template< class T >
struct C3::_CreateReduce< C3::Frame< T > >
{
    template< class U >
    static C3::Frame< T > create( const C3::Stack< U >& src ) { return C3::Frame< T >( src.ncolumns(), src.nrows() ); }
};

// Reduce Pattern Definitions
// --------------------------

// Pixel from contiguous block.

template< class T, class U, class Operator >
inline void C3::_reduce_pixel_contiguous( T& dest, const C3::Block< U >& src, Operator op )
{
    op( dest, src.begin(), src.end() );
}

// Contiguous block from strided block.

template< class T, class U, class Operator >
inline void C3::_reduce_contiguous_strided( C3::Block< T >& dest, const C3::Block< U >& src, Operator op,
        const C3::size_type offset, const C3::size_type length, const C3::size_type stride )
{
    auto src_begin = src.begin() + offset;
    auto src_end   = src_begin   + length;
    for( auto pixel = dest.begin(); pixel != dest.end(); ++pixel )
    {
        op( *pixel, src_begin, src_end );
        src_begin += stride;
        src_end   += stride;
    }
}

// Row from Frame or View.  This routine mainly exists to avoid putting the exact same lines of code in two reduce
// pattern routines.

template< class T, class Source, class Operator >
inline void C3::_reduce_row_2d( C3::Row< T >& dest, const Source& src, Operator op )
{
    C3::Block< T > tmp( src.nrows() );
    for( auto j = 0; j < src.ncolumns(); ++j )
    {
        for( auto k = 0; k < src.nrows(); ++k ) tmp[ k ] = src( j, k );
        C3::reduce( dest( j ), tmp, op );
    }
}

// Strided block from larger strided block.

template< class T, class U, class Operator >
inline void C3::_reduce_strided_strided( C3::Block< T >& dest, const C3::Block< U >& src, Operator op,
    const C3::size_type  src_offset, const C3::size_type  src_length, const C3::size_type  src_stride,
    const C3::size_type dest_offset, const C3::size_type dest_length, const C3::size_type dest_stride, 
    const C3::size_type dest_count )
{
    auto src_begin  = src.begin()  + src_offset;
    auto src_end    = src_begin    + src_length;
    auto dest_begin = dest.begin() + dest_offset;
    auto dest_end   = dest_begin   + dest_length;
    for( auto i = 0; i < dest_count; ++i )
    {
        for( auto pixel = dest_begin; pixel != dest_end; ++pixel )
        {
            op( *pixel, src_begin, src_end );
            src_begin += src_stride;
            src_end   += src_stride;
        }
        dest_begin += dest_stride;
        dest_end   += dest_stride;
    }
}

// Reduce Driver Definitions
// -------------------------

// Pixel from Column, Row, Frame, or Stack.  These source containers are contiguous and we use all their contents in the
// course of reduction.  Compile-time overload resolution makes this function be the catch-all that detects reduction
// calls having Column, Row, Frame, or Stack source containers and non-pixel destination containers that are not defined
// elsewhere.
        
template< class Destination, class Source, class Operator >
inline Destination& C3::_reduce( Destination& dest, const Source& src, Operator op )
{
    C3::_reduce_pixel_contiguous( dest, src.block(), op );
    return dest;
}
   
// Pixel from View using referenced portion of underlying pixel block.  Since all referenced pixels are needed to
// perform reduce, we copy them into a Frame container then reduce that.  Compile-time overload resolution makes this
// function be the catch-all that detects reduction calls having View source containers and non-pixel destination
// containers that are not defined elsewhere.

template< class Destination, class T, class Operator >
inline Destination& C3::_reduce( Destination& dest, const C3::View< T >& src, Operator op )
{
    C3::Frame< T > tmp( src.ncolumns(), src.nrows() );
    C3::assign( tmp, src );     // FIXME change to tmp = src;
    return C3::reduce( dest, tmp, op );
}
        
// Column from Frame.

template< class T, class U, class Operator >
inline C3::Column< T >& C3::_reduce( C3::Column< T >& dest, const C3::Frame< U >& src, Operator op )
{
    C3::_reduce_contiguous_strided( dest.block(), src.block(), op, 0, src.ncolumns(), src.ncolumns() );
    return dest;
}

// Column from View.

template< class T, class U, class Operator >
inline C3::Column< T >& C3::_reduce( C3::Column< T >& dest, const C3::View< U >& src, Operator op )
{
    C3::_reduce_contiguous_strided( dest.block(), src.block(), op, src.offset(), src.ncolumns(), src.stride() );
    return dest;
}

// Row from Frame.

template< class T, class U, class Operator >
inline C3::Row< T >& C3::_reduce( C3::Row< T >& dest, const C3::Frame< U >& src, Operator op )
{
    C3::_reduce_row_2d( dest, src, op );
    return dest;
}

// Row from View.

template< class T, class U, class Operator >
inline C3::Row< T >& C3::_reduce( C3::Row< T >& dest, const C3::View< U >& src, Operator op )
{
    C3::_reduce_row_2d( dest, src, op );
    return dest;
}

// Frame from Stack.

template< class T, class U, class Operator >
inline C3::Frame< T >& C3::_reduce( C3::Frame< T >& dest, const C3::Stack< U >& src, Operator op )
{
    C3::_reduce_contiguous_strided( dest.block(), src.block(), op, 0, src.nframes(), src.nframes() );
    return dest;
}

// View from Stack.

template< class T, class U, class Operator >
inline C3::View< T >& C3::_reduce( C3::View< T >& dest, const C3::Stack< U >& src, Operator op )
{
    C3::_reduce_strided_strided( dest.block(), src.block(), op, 
                        0,   src.nframes(), src.nframes(), 
            dest.offset(), dest.ncolumns(), dest.stride(), dest.nrows() );
    return dest;
}
