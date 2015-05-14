
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


int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
