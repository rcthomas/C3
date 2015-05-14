
#include "gtest/gtest.h"

#include "C3_Block.hh"

TEST( BlockTest, ConstructorWorks ) 
{

    size_t size = 5;
    C3::Block< size_t > block( size );

    EXPECT_EQ( size, block.size() );

}

TEST( BlockTest, CopyConstructorWorks ) 
{

    size_t size = 5;
    C3::Block< size_t > block( size );
    for( auto i = 0; i < block.size(); ++ i ) block[ i ] = i;

    C3::Block< size_t > copy( block );

    EXPECT_EQ( size, copy.size() );
    for( auto i = 0; i < copy.size(); ++ i ) EXPECT_EQ( block[ i ], copy[ i ] );

}

C3::Block< size_t > create( const size_t size, size_t** ptr )
{
    C3::Block< size_t > tmp( size );
    *ptr = tmp.data();
    return tmp;
}

TEST( BlockTest, MoveConstructorWorks ) 
{

    size_t size = 5;
    size_t* block_data = nullptr;
    C3::Block< size_t > block = create( size, &block_data );

    EXPECT_EQ( size      , block.size() );
    EXPECT_EQ( block_data, block.data() );

}

TEST( BlockTest, CopyAssignmentOperatorWorks ) 
{

    size_t size = 5;
    C3::Block< size_t > block( size );
    for( auto i = 0; i < block.size(); ++ i ) block[ i ] = i;

    C3::Block< size_t > copy = block;

    EXPECT_EQ( size, copy.size() );
    for( auto i = 0; i < copy.size(); ++ i ) EXPECT_EQ( block[ i ], copy[ i ] );

}

TEST( BlockTest, MoveAssignmentOperatorWorks ) 
{

    size_t size = 5;
    C3::Block< size_t > block( size );
    size_t* block_data = block.data(); 

    C3::Block< size_t > copy( 1 );
    copy = std::move( block );

    EXPECT_EQ( size      , copy.size() );
    EXPECT_EQ( block_data, copy.data() );
    EXPECT_EQ( 0         , block.size() );
    EXPECT_EQ( nullptr   , block.data() );

}

struct DestructCounter
{
    static unsigned int num;
    ~DestructCounter() { ++ num; }
};

unsigned int DestructCounter::num = 0;

TEST( BlockTest, DestructorWorks ) 
{
    size_t size = 5;
    {
        C3::Block< DestructCounter > block( size );
    }
    EXPECT_EQ( size, DestructCounter::num );
}

TEST( BlockTest, SizeWorks )
{
    for( size_t size = 1; size < 100; size *= 2 )
    {
        C3::Block< size_t > block( size );
        EXPECT_EQ( size, block.size() );
    }
}

TEST( BlockTest, ElementAccessWorks )
{
    size_t size = 5;
    C3::Block< size_t > block( size );

    block[ 0 ] = 99; EXPECT_EQ( 99, block[ 0 ] );
    block[ 1 ] = 79; EXPECT_EQ( 79, block[ 1 ] );
    block[ 2 ] = 49; EXPECT_EQ( 49, block[ 2 ] );
    block[ 3 ] = 39; EXPECT_EQ( 39, block[ 3 ] );
    block[ 4 ] =  9; EXPECT_EQ(  9, block[ 4 ] );
}

TEST( BlockTest, DataAccessWorks )
{

    size_t size = 5;
    C3::Block< size_t > block( size );

    size_t* data = block.data();
    data[ 0 ] = 1; 
    data[ 1 ] = 11; 
    data[ 2 ] = 21; 
    data[ 3 ] = 1211;
    data[ 4 ] = 111221;

    const size_t* const_data = block.data();
    EXPECT_EQ( 1     , const_data[ 0 ] );
    EXPECT_EQ( 11    , const_data[ 1 ] );
    EXPECT_EQ( 21    , const_data[ 2 ] );
    EXPECT_EQ( 1211  , const_data[ 3 ] );
    EXPECT_EQ( 111221, const_data[ 4 ] );

    size_t* offset_data = block.data( 2 );
    offset_data[ 0 ] = 312211;
    offset_data[ 1 ] = 13112221;
    offset_data[ 2 ] = 1113213211;

    const size_t* const_offset_data = block.data( 1 );
    EXPECT_EQ( 11        , const_offset_data[ 0 ] );
    EXPECT_EQ( 312211    , const_offset_data[ 1 ] );
    EXPECT_EQ( 13112221  , const_offset_data[ 2 ] );
    EXPECT_EQ( 1113213211, const_offset_data[ 3 ] );

}

TEST( BlockTest, CastWorks )
{

    size_t size = 5;
    C3::Block< unsigned char > block( size );
    block[ 0 ] = 255;

    C3::Block< signed char > other( block );
    EXPECT_EQ( -1, other[ 0 ] );

}

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
