
#include "gtest/gtest.h"

#include "C3_OwnedBlock.hh"
#include "C3_AliasBlock.hh"

// Constructor.

TEST( AliasBlockTest, Construct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > block( owned );

    EXPECT_EQ(         size, block.size() );
    EXPECT_EQ( owned.data(), block.data() );

}

TEST( AliasBlockTest, ArraySubscriptAccess )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > block( owned );
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

TEST( AliasBlockTest, NativeArrayAccess )
{

    C3::size_type size = 5;
    C3::OwnedBlock< unsigned long int > owned( size );
    C3::AliasBlock< unsigned long int > block( owned );
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

TEST( AliasBlockTest, BeginAndEnd )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > block( owned );

    EXPECT_EQ( block.data(), block.begin()               );
    EXPECT_EQ( size        , block.end() - block.begin() );

}

// Copy constructor.

TEST( AliasBlockTest, CopyConstruct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > original( owned );

    C3::AliasBlock< int > other( original );
    EXPECT_EQ(         size, other.size() );
    EXPECT_EQ( owned.data(), other.data() );

}

// Move constructor.

TEST( AliasBlockTest, MoveConstruct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > original( owned );

    C3::AliasBlock< int > other = std::move( original );

    EXPECT_EQ( owned.data(), other.data() );

}

// Copy assignment.

TEST( AliasBlockTest, CopyAssign )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    C3::AliasBlock< int > original( owned );

    C3::AliasBlock< int > other( original );
    other = original;

    EXPECT_EQ(         size, other.size() );
    EXPECT_EQ( owned.data(), other.data() );

}

// Move assignment.

TEST( AliasBlockTest, MoveAssign )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned1( size );
    C3::AliasBlock< int > original( owned1 );

    C3::OwnedBlock< int > owned2( size + 23 );
    C3::AliasBlock< int > other( owned2 );
    other = std::move( original );

    EXPECT_EQ(          size,    other.size() );
    EXPECT_EQ( owned1.data(),    other.data() );

}

// Test that we don't take responsibility for liberating the payload.

struct alias_block_cheat { static int count; ~alias_block_cheat() { ++count; } };
int alias_block_cheat::count = 0;

TEST( AliasBlockTest, Destruct )
{

    C3::size_type size = 5;
    C3::OwnedBlock< int > owned( size );
    {
        C3::AliasBlock< int > original( owned );
    }

    EXPECT_EQ( 0, alias_block_cheat::count );

}

