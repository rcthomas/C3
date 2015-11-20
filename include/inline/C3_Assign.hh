
#include <algorithm>
#include <cassert>

#include "../C3_Congruent.hh"

// Internal declarations

namespace C3
{

    // Assignment Kernel Driver Declarations
    // -------------------------------------
   
    template< class Destination, class T, class BinaryOperator >
    Destination& _assign( Destination& dest, const T src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    View< T >& _assign( View< T >& dest, const U src, BinaryOperator op );
    
    template< template< class > class Container, class T, class U, class BinaryOperator >
    Container< T >& _assign( Container< T >& dest, const Container< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    View< T >& _assign( View< T >& dest, const View< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    Frame< T >& _assign( Frame< T >& dest, const Column< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    Frame< T >& _assign( Frame< T >& dest, const Row< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    Frame< T >& _assign( Frame< T >& dest, const View< U >& src, BinaryOperator op );

    template< class T, class U, class BinaryOperator >
    View< T >& _assign( View< T >& dest, const Column< U >& src, BinaryOperator op );

    template< class T, class U, class BinaryOperator >
    View< T >& _assign( View< T >& dest, const Row< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    View< T >& _assign( View< T >& dest, const Frame< U >& src, BinaryOperator op );

    template< class T, class U, class BinaryOperator >
    Stack< T >& _assign( Stack< T >& dest, const Frame< U >& src, BinaryOperator op );
    
    template< class T, class U, class BinaryOperator >
    Stack< T >& _assign( Stack< T >& dest, const View< U >& src, BinaryOperator op );

    // Identity (Binary Pass-Through)
    // ------------------------------

    struct Identity {};

    // Assignment Kernel Declarations
    // ------------------------------

    template< class T, class U >
    void _assign_kernel_1( T* begin, T* end, const U src, Identity );
    
    template< class T, class U, class BinaryOperator >
    void _assign_kernel_1( T* begin, T* end, const U src, BinaryOperator op );

    template< class T, class U >
    void _assign_kernel_2( T* dest_begin, const U* src_begin, const U* src_end, Identity );
    
    template< class T, class U, class BinaryOperator >
    void _assign_kernel_2( T* dest_begin, const U* src_begin, const U* src_end, BinaryOperator op );

    template< class Destination, class Source, class BinaryOperator >
    Destination& _assign_kernel_3( Destination& dest, const Source& src, BinaryOperator op,
        const size_type dest_stride, const size_type src_stride = 0 );

    template< class Destination, class Source, class BinaryOperator >
    Destination& _assign_kernel_4( Destination& dest, const Source& src, BinaryOperator op,
        const size_type length, const size_type stride );

}

// 

template< class Destination, class Source >
inline Destination& C3::assign( Destination& dest, const Source& src )
{
    return C3::_assign( dest, src, C3::Identity() );
}

template< class Destination, class Source, class BinaryOperator >
inline Destination& C3::assign( Destination& dest, const Source& src, BinaryOperator op )
{
    return C3::_assign( dest, src, op );
}

// Container from Pixel
// --------------------

template< class Destination, class T, class BinaryOperator >
inline Destination& C3::_assign( Destination& dest, const T src, BinaryOperator op )
{
    C3::_assign_kernel_1( dest.begin(), dest.end(), src, op );
    return dest;
}

template< class T, class U, class BinaryOperator >
inline C3::View< T >& C3::_assign( C3::View< T >& dest, const U src, BinaryOperator op )
{
    auto begin = dest.begin();
    auto end   = dest.begin + dest.ncolumns();
    for( auto k = 0; k < dest.nrows(); ++k )
    {
        C3::_assign_kernel_1( begin, end, src, op );
        begin += dest.stride();
        src   += dest.stride();
    }
    return dest;
}

// Container from Same Container
// -----------------------------

template< template< class > class Container, class T, class U, class BinaryOperator >
inline Container< T >& C3::_assign( Container< T >& dest, const Container< U >& src, BinaryOperator op )
{
    assert( C3::congruent( dest, src ) );
    C3::_assign_kernel_2( dest.begin(), src.begin(), src.end(), op );
    return dest;
}

template< class T, class U, class BinaryOperator >
inline C3::View< T >& C3::_assign( C3::View< T >& dest, const C3::View< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_3( dest, src, op, dest.stride(), src.stride() );
}

// Container from Different Container
// ----------------------------------

// Frame from Column.

template< class T, class U, class BinaryOperator >
inline C3::Frame< T >& C3::_assign( C3::Frame< T >& dest, const C3::Column< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_4( dest, src, op, dest.ncolumns(), dest.ncolumns() );
}

// Frame from Row.

template< class T, class U, class BinaryOperator >
inline C3::Frame< T >& C3::_assign( C3::Frame< T >& dest, const C3::Row< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_3( dest, src, op, dest.ncolumns() );
}

// Frame from View.

template< class T, class U, class BinaryOperator >
inline C3::Frame< T >& C3::_assign( C3::Frame< T >& dest, const C3::View< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_3( dest, src, op, dest.ncolumns(), src.stride() );
}

// View from Column.

template< class T, class U, class BinaryOperator >
inline C3::View< T >& C3::_assign( C3::View< T >& dest, const C3::Column< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_4( dest, src, op, dest.ncolumns(), dest.stride() );
}

// View from Row.

template< class T, class U, class BinaryOperator >
inline C3::View< T >& C3::_assign( C3::View< T >& dest, const C3::Row< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_3( dest, src, op, dest.stride() );
}

// View from Frame.

template< class T, class U, class BinaryOperator >
inline C3::View< T >& C3::_assign( C3::View< T >& dest, const C3::Frame< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_3( dest, src, op, dest.stride(), src.ncolumns() );
}

// Stack from Frame.

template< class T, class U, class BinaryOperator >
inline C3::Stack< T >& C3::_assign( C3::Stack< T >& dest, const C3::Frame< U >& src, BinaryOperator op )
{
    return C3::_assign_kernel_4( dest, src, op, dest.nframes(), dest.nframes() );
}

// Stack from View.

template< class T, class U, class BinaryOperator >
inline C3::Stack< T >& C3::_assign( C3::Stack< T >& dest, const C3::View< U >& src, BinaryOperator op )
{
    assert( C3::congruent( dest, src ) );
    auto dest_begin = dest.begin();
    auto dest_end   = dest_begin + dest.nframes();
    const auto src_begin = src.begin();
    const auto src_end   = src_begin + src.ncolumns();
    for( auto k = 0; k < dest.nrows(); ++k )
    {
        for( auto pixel = src_begin; pixel != src_end; ++pixel )
        {
            C3::_assign_kernel_1( dest_begin, dest_end, *pixel );
            dest_begin += dest.nframes();
            dest_end   += dest.nframes();
        }
        src_begin += src.stride();
        src_end   += src.stride();
    }
    return dest;
}

// Assignment Kernel Definitions
// -----------------------------

template< class T, class U >
inline void C3::_assign_kernel_1( T* begin, T* end, const U src, C3::Identity )
{
    std::fill( begin, end, src );
}

template< class T, class U, class BinaryOperator >
inline void C3::_assign_kernel_1( T* begin, T* end, const U src, BinaryOperator op )
{
    for( ; begin != end; ++begin ) *begin = op( *begin, src ); // Ensure order (see std::transform).
}

template< class T, class U >
inline void C3::_assign_kernel_2( T* dest_begin, const U* src_begin, const U* src_end, C3::Identity )
{
    std::copy( src_begin, src_end, dest_begin );
}

template< class T, class U, class BinaryOperator >
inline void C3::_assign_kernel_2( T* dest_begin, const U* src_begin, const U* src_end, BinaryOperator op )
{
    std::copy( src_begin, src_end, dest_begin );
    for( ; src_begin != src_end; ++src_begin, ++dest_begin ) *dest_begin = op( *dest_begin, src_begin ); 
    // Ensure order (see std::transform).
}

template< class Destination, class Source, class BinaryOperator >
inline Destination& C3::_assign_kernel_3( Destination& dest, const Source& src, BinaryOperator op,
        const C3::size_type dest_stride, const C3::size_type src_stride )
{
    assert( C3::congruent( dest, src ) );
    auto dest_begin = dest.begin();
    const auto src_begin = src.begin();
    const auto src_end   = src_begin + src.ncolumns();
    for( auto k = 0; k < dest.nrows(); ++k )
    {
        C3::_assign_kernel_2( dest_begin, src_begin, src_end, op );
        dest_begin += dest_stride;
        src_begin  += src_stride;
        src_end    += src_stride;
    }
    return dest;
}

template< class Destination, class Source, class BinaryOperator >
inline Destination& C3::_assign_kernel_4( Destination& dest, const Source& src, BinaryOperator op,
        const C3::size_type length, const C3::size_type stride )
{
    assert( C3::congruent( dest, src ) );
    auto begin = dest.begin();
    auto end   = begin + length;
    for( auto pixel : src )
    {
        C3::_assign_kernel_1( begin, end, pixel, op );
        begin += stride;
        end   += stride;
    }
    return dest;
}
