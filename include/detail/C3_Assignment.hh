#ifndef C3_ASSIGNMENT_HH
#define C3_ASSIGNMENT_HH

#include <algorithm>
#include <cassert>

namespace C3
{

    namespace detail
    {

        template< class T >
        struct Identity {};

        // Assignment Kernel.

        template< template< class > class Operator >
        struct AssignmentKernel
        {

            template< class T, class U >
            static void assign( T* dest_begin, T* dest_end, const U src )
            {
                std::transform( dest_begin, dest_end, dest_begin, std::bind( Operator< T >(), std::placeholders::_1, src ) );
            }

            template< class T, class U >
            static void assign( const U* src_begin, const U* src_end, T* dest_begin )
            {
                std::transform( src_begin, src_end, dest_begin, dest_begin, Operator< T >() );
            }

        };

        template<>
        struct AssignmentKernel< Identity >
        {

            template< class T, class U >
            static void assign( T* dest_begin, T* dest_end, const U src )
            {
                std::fill( dest_begin, dest_end, src );
            }

            template< class T, class U >
            static void assign( const U* src_begin, const U* src_end, T* dest_begin )
            {
                std::copy( src_begin, src_end, dest_begin );
            }

        };

        // Pixel-fill assignment algorithm.

        template< template< class > class Operator, class T, class U >
        inline void pixel_fill_assign( Block< T >& dest, const U src, 
                const size_type offset, const size_type length, const size_type count, const size_type stride )
        {
            auto dest_begin = dest.begin() + offset;
            auto dest_end   = dest_begin   + length;
            for( auto i = 0; i < count; ++i )
            {
                AssignmentKernel< Operator >::assign( dest_begin, dest_end, src );
                dest_begin += stride;
                dest_end   += stride;
            }
        }

        // Block-fill assignment algorithm.

        template< template< class > class Operator, class T, class U >
        inline void block_fill_assign( Block< T >& dest, const Block< U >& src, 
                const size_type offset, const size_type length, const size_type stride )
        {
            auto dest_begin = dest.begin() + offset;
            auto dest_end   = dest_begin   + length;
            for( auto pixel = src.begin(); pixel != src.end(); ++pixel )
            {
                AssignmentKernel< Operator >::assign( dest_begin, dest_end, *pixel );
                dest_begin += stride;
                dest_end   += stride;
            }
        }

        // Block-copy assignment algorithm.

        template< template< class > class Operator, class T, class U >
        inline void block_copy_assign( Block< T >& dest, const Block< U >& src,
                const size_type  src_offset, const size_type src_length, const size_type src_stride,
                const size_type dest_offset, const size_type dest_count, const size_type dest_stride )
        {
            auto src_begin  = src.begin()  + src_offset;
            auto src_end    = src_begin    + src_length;
            auto dest_begin = dest.begin() + dest_offset;
            assert(  src_offset + dest_count * src_length <=  src.size() );
            assert( dest_offset + dest_count * src_length <= dest.size() );
            for( auto i = 0; i < dest_count; ++i )
            {
                AssignmentKernel< Operator >::assign( src_begin, src_end, dest_begin );
                src_begin  += src_stride;
                src_end    += src_stride;
                dest_begin += dest_stride;
            }
        }

        // Nested (fill within copy, no temporary) assignment.

        template< template< class > class Operator, class T, class U >
        inline void nested_assign( Block< T >& dest, const Block< U >& src,
                const size_type  src_offset, const size_type  src_length, const size_type  src_stride,
                const size_type dest_offset, const size_type dest_length, const size_type dest_stride, const size_type dest_count )
        {
            auto src_begin  = src.begin()  + src_offset;
            auto src_end    = src_begin    + src_length;
            auto dest_begin = dest.begin() + dest_offset;
            auto dest_end   = dest_begin   + dest_length;
            for( auto i = 0; i < dest_count; ++i )
            {
                for( auto pixel = src_begin; pixel != src_end; ++pixel )
                {
                    AssignmentKernel< Operator >::assign( dest_begin, dest_end, *pixel );
                    dest_begin += dest_stride;
                    dest_end   += dest_stride;
                }
                src_begin += src_stride;
                src_end   += src_stride;
            }
        }

        // The assignment schema could be implemented with tag dispatch, but the
        // multiplexing is not compelling enough.  There are twenty overloaded
        // function calls between various container and pixel types.  Using tag
        // dispatch would get this down to fourteen, a reduction of only 30% in
        // terms of number of functions.  This reduction would come at the cost of
        // having to manage the type traits as well, which might obviate the 
        // reduction in terms of code.

        template< template< class > class Operator, class T, class U >
        inline T& assign( T& dest, const U& src )
        {
            static_assert( true, "I have no idea what you want to do." );
        }

        template< template< class > class Operator, class T, class U >
        inline Column< T >& assign( Column< T >& dest, const U src )
        {
            pixel_fill_assign< Operator >( dest.block(), src, 0, dest.block().size(), 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Row< T >& assign( Row< T >& dest, const U src )
        {
            pixel_fill_assign< Operator >( dest.block(), src, 0, dest.block().size(), 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Frame< T >& assign( Frame< T >& dest, const U src )
        {
            pixel_fill_assign< Operator >( dest.block(), src, 0, dest.block().size(), 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const U src )
        {
            pixel_fill_assign< Operator >( dest.block(), src, 0, dest.block().size(), 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline View< T >& assign( View< T >& dest, const U src )
        {
            pixel_fill_assign< Operator >( dest.block(), src, dest.offset(), dest.ncolumns(), dest.nrows(), dest.stride() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Frame< T >& assign( Frame< T >& dest, const Column< U >& src )
        {
            block_fill_assign< Operator >( dest.block(), src.block(), 0, dest.nrows(), dest.nrows() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline View< T >& assign( View< T >& dest, const Column< U >& src )
        {
            block_fill_assign< Operator >( dest.block(), src.block(), dest.offset, dest.nrows(), dest.stride() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const Column< U >& src )
        {
            auto length = dest.nframes() * dest.nrows();
            block_fill_assign< Operator >( dest.block(), src.block(), 0, length, length );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const Frame< U >& src )
        {
            block_fill_assign< Operator >( dest.block(), src.block(), 0, dest.nframes(), dest.nframes() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline OwnedBlock< T >& assign( OwnedBlock< T >& dest, const OwnedBlock< U >& src )
        {
            block_copy_assign< Operator >( dest, src, 0, src.size(), 0, 0, 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Column< T >& assign( Column< T >& dest, const Column< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.block().size(), 0, 0, 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Row< T >& assign( Row< T >& dest, const Row< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.block().size(), 0, 0, 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Frame< T >& assign( Frame< T >& dest, const Frame< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.block().size(), 0, 0, 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const Stack< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.block().size(), 0, 0, 1, 0 );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline View< T >& assign( View< T >& dest, const View< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), src.offset(), src.ncolumns(), src.stride(), dest.offset(), dest.nrows(), dest.stride() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline View< T >& assign( View< T >& dest, const Frame< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.ncolumns(), src.ncolumns(), dest.offset(), dest.nrows(), dest.stride() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Frame< T >& assign( Frame< T >& dest, const View< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), src.offset(), src.ncolumns(), src.stride(), 0, dest.nrows(), dest.ncolumns() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Frame< T >& assign( Frame< T >& dest, const Row< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.ncolumns(), 0, 0, dest.nrows(), dest.ncolumns() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline View< T >& assign( View< T >& dest, const Row< U >& src )
        {
            block_copy_assign< Operator >( dest.block(), src.block(), 0, src.ncolumns(), 0, dest.offset(), dest.nrows(), dest.stride() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const View< U >& src )
        {
            nested_assign< Operator >( dest.block(), src.block(), src.offset(), src.ncolumns(), src.stride(), 0, dest.nframes(), dest.nframes(), dest.nrows() );
            return dest;
        }

        template< template< class > class Operator, class T, class U >
        inline Stack< T >& assign( Stack< T >& dest, const Row< U >& src )
        {
            nested_assign< Operator >( dest.block(), src.block(), 0, src.ncolumns(), 0, 0, dest.nframes(), dest.nframes(), dest.nrows() );
            return dest;
        }

    }

}

#endif
