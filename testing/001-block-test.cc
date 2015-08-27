
#include "gtest/gtest.h"

#include "C3_Block.hh"

TEST( BlockTest, Construct )
{

    C3::size_type size = 5;
    C3::Block< int > container( size );

    EXPECT_EQ( size, container.size() );

}

TEST( BlockTest, ConstructWithInitialization )
{

    C3::size_type size = 5;
    C3::Block< int > container( size, 9 );

    EXPECT_EQ( size, container.size() );
    for( auto pixel : container ) EXPECT_EQ( 9, pixel );

}

TEST( BlockTest, CopyConstruct )
{

    C3::size_type size = 5;
    C3::Block< int > original( size );
    original[ 0 ] = 1;
    original[ 1 ] = 11;
    original[ 2 ] = 21;
    original[ 3 ] = 1211;
    original[ 4 ] = 111221;

    C3::Block< int > other( original );
    EXPECT_EQ( 1     , other[ 0 ] );
    EXPECT_EQ( 11    , other[ 1 ] );
    EXPECT_EQ( 21    , other[ 2 ] );
    EXPECT_EQ( 1211  , other[ 3 ] );
    EXPECT_EQ( 111221, other[ 4 ] );

}

TEST( BlockTest, MoveConstruct )
{

    C3::size_type size = 5;
    C3::Block< int > original( size );
    int* data = original.data();

    C3::Block< int > other = std::move( original );

    EXPECT_EQ( data   ,    other.data() );
    EXPECT_EQ( nullptr, original.data() );

}

TEST( BlockTest, CopyAssign )
{

    C3::size_type size = 5;
    C3::Block< int > original( size );
    original[ 0 ] = 1;
    original[ 1 ] = 11;
    original[ 2 ] = 21;
    original[ 3 ] = 1211;
    original[ 4 ] = 111221;

    C3::Block< int > other( size );
    other = original;

    EXPECT_EQ( 1     , other[ 0 ] );
    EXPECT_EQ( 11    , other[ 1 ] );
    EXPECT_EQ( 21    , other[ 2 ] );
    EXPECT_EQ( 1211  , other[ 3 ] );
    EXPECT_EQ( 111221, other[ 4 ] );

}

TEST( BlockTest, MoveAssign )
{

    C3::size_type size = 5;
    C3::Block< int > original( size );
    int* data = original.data();

    C3::Block< int > other( size + 30 );
    other = std::move( original );

    EXPECT_EQ( data   ,    other.data() );
    EXPECT_EQ( nullptr, original.data() );

}

struct block_cheat { static int count; ~block_cheat() { ++count; } };
int block_cheat::count = 0;

TEST( BlockTest, Destruct )
{

    C3::size_type size = 5;
    {
        C3::Block< block_cheat > original( size );
    }

    EXPECT_EQ( size, block_cheat::count );

}

TEST( BlockTest, AccessNativeArray )
{

    C3::size_type size = 5;
    C3::Block< unsigned long int > block( size );
    block[ 0 ] = 1u;
    block[ 1 ] = 11u;
    block[ 2 ] = 21u;
    block[ 3 ] = 1211u;
    block[ 4 ] = 111221u;

    EXPECT_EQ( 1u     , block.data()[ 0 ] );
    EXPECT_EQ( 11u    , block.data()[ 1 ] );
    EXPECT_EQ( 21u    , block.data()[ 2 ] );
    EXPECT_EQ( 1211u  , block.data()[ 3 ] );
    EXPECT_EQ( 111221u, block.data()[ 4 ] );

    block.data()[ 0 ] = 312211u;
    block.data()[ 1 ] = 13112221u;
    block.data()[ 2 ] = 1113213211u;
    block.data()[ 3 ] = 31131211131221u;
    block.data()[ 4 ] = 13211311123113112211u;

    EXPECT_EQ( 312211u              , block[ 0 ] );
    EXPECT_EQ( 13112221u            , block[ 1 ] );
    EXPECT_EQ( 1113213211u          , block[ 2 ] );
    EXPECT_EQ( 31131211131221u      , block[ 3 ] );
    EXPECT_EQ( 13211311123113112211u, block[ 4 ] );

}

TEST( BlockTest, BeginAndEnd )
{

    C3::size_type size = 5;
    C3::Block< int > block( size );

    EXPECT_EQ( block.data(), block.begin()               );
    EXPECT_EQ( size        , block.end() - block.begin() );

    auto pos = 1923;
    for( auto& entry : block ) entry = pos++;

    pos = 1923;
    for( auto entry : block ) EXPECT_EQ( pos++, entry );

}

TEST( BlockTest, AssignPixel ) 
{

    C3::size_type size = 5;
    C3::Block< int > container( size );

    auto const pixel = 312211;

    container = pixel;

    EXPECT_EQ( pixel, container[ 0 ] );
    EXPECT_EQ( pixel, container[ 1 ] );
    EXPECT_EQ( pixel, container[ 2 ] );
    EXPECT_EQ( pixel, container[ 3 ] );
    EXPECT_EQ( pixel, container[ 4 ] );

}

TEST( BlockTest, ConvertValueType ) 
{

    C3::size_type size = 5;
    C3::Block< double > container( size );

    container[ 0 ] = 1.1;
    container[ 1 ] = 11.2;
    container[ 2 ] = 21.3;
    container[ 3 ] = 1211.4;
    container[ 4 ] = 111221.5;

    auto other = C3::Block< int >( container );

    EXPECT_EQ( 1     , other[ 0 ] );
    EXPECT_EQ( 11    , other[ 1 ] );
    EXPECT_EQ( 21    , other[ 2 ] );
    EXPECT_EQ( 1211  , other[ 3 ] );
    EXPECT_EQ( 111221, other[ 4 ] );

}

TEST( BlockTest, ArraySubscriptAccess )
{

    C3::size_type size = 5;
    C3::Block< int > block( size );
    block[ 0 ] = 1;
    block[ 1 ] = 11;
    block[ 2 ] = 21;
    block[ 3 ] = 1211;
    block[ 4 ] = 111221;

    EXPECT_EQ( 1     , block[ 0 ] );
    EXPECT_EQ( 11    , block[ 1 ] );
    EXPECT_EQ( 21    , block[ 2 ] );
    EXPECT_EQ( 1211  , block[ 3 ] );
    EXPECT_EQ( 111221, block[ 4 ] );

}
