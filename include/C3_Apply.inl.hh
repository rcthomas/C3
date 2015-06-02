
#include <algorithm>
#include <cassert>

template< template< class > class Operator, class Block >
Block& C3::apply( Block& dest, const typename Block::value_type src )
{
    std::transform( dest.begin(), dest.end(), dest.begin(), std::bind( Operator< typename Block::value_type >(), std::placeholders::_1, src ) );
    return dest;
}

template< template< class > class Operator, class Block >
Block& C3::apply( Block& dest, const Block& src )
{
    assert( dest.size() == src.size() );
    std::transform( dest.begin(), dest.end(), src.begin(), dest.begin(), Operator< typename Block::value_type >() );
    return dest;
}

// col to frame, col to stack, frame to stack

template< template< class > class Operator, class Block >
Block& C3::apply( Block& dest, const Block& src, const typename Block::size_type count )
{
    assert( dest.size() == src.size() * count );
    typename Block::value_type* begin = dest.begin();
    typename Block::value_type* end   = begin + count;
    for( auto element = src.begin(); element != src.end(); ++ element )
    {
        std::transform( begin, end, begin, std::bind( Operator< typename Block::value_type >(), std::placeholders::_1, *element ) );
        begin += count;
        end   += count;
    }
    return dest;
}

// row to frame, 

template< template< class > class Operator, class Block >
Block& C3::apply( Block& dest, const Block& src, const typename Block::size_type count )
{
    assert( dest.size() == src.size() * count );
    typename Block::value_type* begin = dest.begin();
    typename Block::value_type* end   = begin + src.size();
    for( auto i = 0; i < count; ++ i )
    {
        std::transform( begin, end, src.begin(), begin, Operator< typename Block::value_type >() );
        begin += src.size();
        end   += src.size();
    }
    return dest;
}

