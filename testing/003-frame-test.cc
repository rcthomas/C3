
#include "gtest/gtest.h"

#include "C3_Frame.hh"

// Test constructing the container.

TEST( FrameTest, Construct ) 
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    EXPECT_EQ( ncolumns, container.ncolumns() );
    EXPECT_EQ( nrows, container.nrows() );
    EXPECT_EQ( ncolumns * nrows, container.size() );

}

// Test that we can get and set by linear array subscript.

TEST( FrameTest, GetAndSetArraySubscript ) 
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    // Settem ...
    container[ 0 ] = 1;
    container[ 1 ] = 11;
    container[ 2 ] = 21;
    container[ 3 ] = 1211;
    container[ 4 ] = 111221;
    container[ 5 ] = 312211;

    // ... ngettem.
    EXPECT_EQ( 1     , container[ 0 ] );
    EXPECT_EQ( 11    , container[ 1 ] );
    EXPECT_EQ( 21    , container[ 2 ] );
    EXPECT_EQ( 1211  , container[ 3 ] );
    EXPECT_EQ( 111221, container[ 4 ] );
    EXPECT_EQ( 312211, container[ 5 ] );

}

// Test that we can get and set by coordinate subscript.

TEST( FrameTest, GetAndSetCoordinateSubscript ) 
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    // Settem ...
    container( 0, 0 ) = 1;
    container( 1, 0 ) = 11;
    container( 0, 1 ) = 21;
    container( 1, 1 ) = 1211;
    container( 0, 2 ) = 111221;
    container( 1, 2 ) = 312211;

    // ... ngettem.
    EXPECT_EQ( 1     , container( 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0 ) );
    EXPECT_EQ( 21    , container( 0, 1 ) );
    EXPECT_EQ( 1211  , container( 1, 1 ) );
    EXPECT_EQ( 111221, container( 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2 ) );

    // ... ngettem other way.
    EXPECT_EQ( 1     , container[ 0 ] );
    EXPECT_EQ( 11    , container[ 1 ] );
    EXPECT_EQ( 21    , container[ 2 ] );
    EXPECT_EQ( 1211  , container[ 3 ] );
    EXPECT_EQ( 111221, container[ 4 ] );
    EXPECT_EQ( 312211, container[ 5 ] );

}

// Text end is where it should be relative to beginning.  More extensive tests
// of use of begin() and end() happen with operators like assign, apply, etc.

TEST( FrameTest, RawAccess )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    EXPECT_EQ( container.data(), container.begin() );
    EXPECT_EQ( container.data() + ncolumns * nrows, container.end() );

}

// Test we can construct a container from another one.

TEST( FrameTest, CopyConstruct )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > original( ncolumns, nrows );
    original( 0, 0 ) = 1;
    original( 1, 0 ) = 11;
    original( 0, 1 ) = 21;
    original( 1, 1 ) = 1211;
    original( 0, 2 ) = 111221;
    original( 1, 2 ) = 312211;

    C3::Frame< int > container( original );
    EXPECT_EQ( 1     , container( 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0 ) );
    EXPECT_EQ( 21    , container( 0, 1 ) );
    EXPECT_EQ( 1211  , container( 1, 1 ) );
    EXPECT_EQ( 111221, container( 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2 ) );

}

// Test move constructor.

C3::Frame< int > func( const C3::size_type ncolumns, const C3::size_type nrows, int*& data ) 
{ 
    C3::Frame< int > tmp( ncolumns, nrows );
    data = tmp.data();
    return tmp;
}

TEST( FrameTest, MoveConstruct )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    int* ptr;
    C3::Frame< int > container = func( ncolumns, nrows, ptr );

    EXPECT_EQ( ncolumns * nrows, container.size() );
    EXPECT_EQ( ptr, container.data() );

}

// Test copy assignment operator.

TEST( FrameTest, CopyAssign )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > original( ncolumns, nrows );
    original( 0, 0 ) = 1;
    original( 1, 0 ) = 11;
    original( 0, 1 ) = 21;
    original( 1, 1 ) = 1211;
    original( 0, 2 ) = 111221;
    original( 1, 2 ) = 312211;

    C3::Frame< int > container( ncolumns, nrows );
    container = original;
    EXPECT_EQ( 1     , container( 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0 ) );
    EXPECT_EQ( 21    , container( 0, 1 ) );
    EXPECT_EQ( 1211  , container( 1, 1 ) );
    EXPECT_EQ( 111221, container( 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2 ) );

}

// Test move assignment operator.

TEST( FrameTest, MoveAssign )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > original( ncolumns, nrows );
    original( 0, 0 ) = 1;
    original( 1, 0 ) = 11;
    original( 0, 1 ) = 21;
    original( 1, 1 ) = 1211;
    original( 0, 2 ) = 111221;
    original( 1, 2 ) = 312211;
    int* original_data = original.data();

    C3::Frame< int > container( ncolumns + 1, nrows + 3 );
    container = std::move( original );

    EXPECT_EQ( ncolumns * nrows, container.size() );
    EXPECT_EQ( ncolumns        , container.ncolumns() );
    EXPECT_EQ( nrows           , container.nrows() );
    EXPECT_EQ( original_data   , container.data() );
    EXPECT_EQ( 1               , container( 0, 0 ) );
    EXPECT_EQ( 11              , container( 1, 0 ) );
    EXPECT_EQ( 21              , container( 0, 1 ) );
    EXPECT_EQ( 1211            , container( 1, 1 ) );
    EXPECT_EQ( 111221          , container( 0, 2 ) );
    EXPECT_EQ( 312211          , container( 1, 2 ) );
    EXPECT_EQ( 0               , original.size() );
    EXPECT_EQ( nullptr         , original.data() );

}

// Test destructor fires when we want it to.

struct cheat { static int count; ~cheat() { ++count; } };
int cheat::count = 0;

TEST( FrameTest, Destruct )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    {
        C3::Frame< cheat > original( ncolumns, nrows );
    }

    EXPECT_EQ( ncolumns * nrows, cheat::count );

}

// Run the tests.

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
