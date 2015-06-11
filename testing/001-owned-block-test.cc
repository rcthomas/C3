
#include "gtest/gtest.h"

#include "C3_OwnedBlock.hh"

// Constructor.

TEST( OwnedBlockTest, Construct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > block( size );

    EXPECT_EQ( size, block.size() );

}

TEST( OwnedBlockTest, ArraySubscriptAccess )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > block( size );
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

// Native array acess.

TEST( OwnedBlockTest, NativeArrayAccess )
{

    C3::size_type size = 5;
    C3::OwnedBlock< unsigned long int > block( size );
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

// Begin and end.

TEST( OwnedBlockTest, BeginAndEnd )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > block( size );

    EXPECT_EQ( block.data(), block.begin()               );
    EXPECT_EQ( size        , block.end() - block.begin() );

}

// Copy constructor.

TEST( OwnedBlockTest, CopyConstruct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > original( size );
    original[ 0 ] = 1;
    original[ 1 ] = 11;
    original[ 2 ] = 21;
    original[ 3 ] = 1211;
    original[ 4 ] = 111221;

    C3::OwnedBlock< int > other( original );
    EXPECT_EQ( 1     , other[ 0 ] );
    EXPECT_EQ( 11    , other[ 1 ] );
    EXPECT_EQ( 21    , other[ 2 ] );
    EXPECT_EQ( 1211  , other[ 3 ] );
    EXPECT_EQ( 111221, other[ 4 ] );

}

// Move constructor.

TEST( OwnedBlockTest, MoveConstruct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > original( size );
    int* data = original.data();

    C3::OwnedBlock< int > other = std::move( original );

    EXPECT_EQ( data   ,    other.data() );
    EXPECT_EQ( nullptr, original.data() );

}

// Copy assignment.

TEST( OwnedBlockTest, CopyAssign )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > original( size );
    original[ 0 ] = 1;
    original[ 1 ] = 11;
    original[ 2 ] = 21;
    original[ 3 ] = 1211;
    original[ 4 ] = 111221;

    C3::OwnedBlock< int > other( size );
    other = original;

    EXPECT_EQ( 1     , other[ 0 ] );
    EXPECT_EQ( 11    , other[ 1 ] );
    EXPECT_EQ( 21    , other[ 2 ] );
    EXPECT_EQ( 1211  , other[ 3 ] );
    EXPECT_EQ( 111221, other[ 4 ] );

}

// Move assignment.

TEST( OwnedBlockTest, MoveAssign )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > original( size );
    int* data = original.data();

    C3::OwnedBlock< int > other( size + 30 );
    other = std::move( original );

    EXPECT_EQ( data   ,    other.data() );
    EXPECT_EQ( nullptr, original.data() );


}
// Test destructor fires when we want it to.

struct block_cheat { static int count; ~block_cheat() { ++count; } };
int block_cheat::count = 0;

TEST( OwnedBlockTest, Destruct )
{

    C3::size_type size = 5;
    {
        C3::OwnedBlock< block_cheat > original( size );
    }

    EXPECT_EQ( size, block_cheat::count );

}

