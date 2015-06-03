
#include "gtest/gtest.h"

#include "C3_Stack.hh"

// Test constructing the container.

TEST( StackTest, Construct ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    EXPECT_EQ( nframes  , container.nframes()   );
    EXPECT_EQ( ncolumns , container.ncolumns()  );
    EXPECT_EQ( nrows    , container.nrows()     );
    EXPECT_EQ( nframes * ncolumns * nrows, container.size() );

}

// Test that we can get and set by linear array subscript.

TEST( StackTest, GetAndSetArraySubscript ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    // Settem ...
    container[ 0  ] = 1;
    container[ 1  ] = 11;
    container[ 3  ] = 21;
    container[ 6  ] = 1211;
    container[ 12 ] = 111221;
    container[ 23 ] = 312211;

    // ... ngettem.
    EXPECT_EQ( 1     , container[ 0  ] );
    EXPECT_EQ( 11    , container[ 1  ] );
    EXPECT_EQ( 21    , container[ 3  ] );
    EXPECT_EQ( 1211  , container[ 6  ] );
    EXPECT_EQ( 111221, container[ 12 ] );
    EXPECT_EQ( 312211, container[ 23 ] );

}

// Test that we can get and set by coordinate subscript.

TEST( StackTest, GetAndSetCoordinateSubscript ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    // Settem ...
    container( 0, 0, 0 ) = 1;
    container( 1, 0, 0 ) = 11;
    container( 1, 1, 0 ) = 21;
    container( 0, 0, 1 ) = 1211;
    container( 0, 0, 2 ) = 111221;
    container( 1, 2, 3 ) = 312211;

    // ... ngettem.
    EXPECT_EQ( 1     , container( 0, 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0, 0 ) );
    EXPECT_EQ( 21    , container( 1, 1, 0 ) );
    EXPECT_EQ( 1211  , container( 0, 0, 1 ) );
    EXPECT_EQ( 111221, container( 0, 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2, 3 ) );

    // ... ngettem other way.
    EXPECT_EQ( 1     , container[ 0  ] );
    EXPECT_EQ( 11    , container[ 1  ] );
    EXPECT_EQ( 21    , container[ 3  ] );
    EXPECT_EQ( 1211  , container[ 6  ] );
    EXPECT_EQ( 111221, container[ 12 ] );
    EXPECT_EQ( 312211, container[ 23 ] );

}

// Test end is where it should be relative to beginning.  More extensive tests
// of use of begin() and end() happen with operators like assign, apply, etc.

TEST( StackTest, RawAccess )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    EXPECT_EQ( container.data(), container.begin() );
    EXPECT_EQ( container.data() + nframes * ncolumns * nrows, container.end() );

}

// Test we can construct a container from another one.

TEST( StackTest, CopyConstruct )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > original( nframes, ncolumns, nrows );
    original( 0, 0, 0 ) = 1;
    original( 1, 0, 0 ) = 11;
    original( 1, 1, 0 ) = 21;
    original( 0, 0, 1 ) = 1211;
    original( 0, 0, 2 ) = 111221;
    original( 1, 2, 3 ) = 312211;

    C3::Stack< int > container( original );
    EXPECT_EQ( 1     , container( 0, 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0, 0 ) );
    EXPECT_EQ( 21    , container( 1, 1, 0 ) );
    EXPECT_EQ( 1211  , container( 0, 0, 1 ) );
    EXPECT_EQ( 111221, container( 0, 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2, 3 ) );

}

// Test move constructor.

C3::Stack< int > func( const C3::size_type nframes, const C3::size_type ncolumns, const C3::size_type nrows, int*& data ) 
{ 
    C3::Stack< int > tmp( nframes, ncolumns, nrows );
    data = tmp.data();
    return tmp;
}

TEST( StackTest, MoveConstruct )
{

    int* ptr;
    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container = func( nframes, ncolumns, nrows, ptr );

    EXPECT_EQ( nframes * ncolumns * nrows, container.size() );
    EXPECT_EQ( ptr, container.data() );

}

// Test copy assignment operator.

TEST( StackTest, CopyAssign )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > original( nframes, ncolumns, nrows );
    original( 0, 0, 0 ) = 1;
    original( 1, 0, 0 ) = 11;
    original( 1, 1, 0 ) = 21;
    original( 0, 0, 1 ) = 1211;
    original( 0, 0, 2 ) = 111221;
    original( 1, 2, 3 ) = 312211;

    C3::Stack< int > container( nframes, ncolumns, nrows );
    container = original;
    EXPECT_EQ( 1     , container( 0, 0, 0 ) );
    EXPECT_EQ( 11    , container( 1, 0, 0 ) );
    EXPECT_EQ( 21    , container( 1, 1, 0 ) );
    EXPECT_EQ( 1211  , container( 0, 0, 1 ) );
    EXPECT_EQ( 111221, container( 0, 0, 2 ) );
    EXPECT_EQ( 312211, container( 1, 2, 3 ) );

}

// Test move assignment operator.

TEST( StackTest, MoveAssign )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > original( nframes, ncolumns, nrows );
    original( 0, 0, 0 ) = 1;
    original( 1, 0, 0 ) = 11;
    original( 1, 1, 0 ) = 21;
    original( 0, 0, 1 ) = 1211;
    original( 0, 0, 2 ) = 111221;
    original( 1, 2, 3 ) = 312211;
    int* original_data = original.data();

    C3::Stack< int > container( nframes + 13, ncolumns + 1, nrows + 3 );
    container = std::move( original );

    EXPECT_EQ( nframes * ncolumns * nrows, container.size() );
    EXPECT_EQ( nframes         , container.nframes() );
    EXPECT_EQ( ncolumns        , container.ncolumns() );
    EXPECT_EQ( nrows           , container.nrows() );
    EXPECT_EQ( original_data   , container.data() );
    EXPECT_EQ( 1               , container( 0, 0, 0 ) );
    EXPECT_EQ( 11              , container( 1, 0, 0 ) );
    EXPECT_EQ( 21              , container( 1, 1, 0 ) );
    EXPECT_EQ( 1211            , container( 0, 0, 1 ) );
    EXPECT_EQ( 111221          , container( 0, 0, 2 ) );
    EXPECT_EQ( 312211          , container( 1, 2, 3 ) );
    EXPECT_EQ( 0               , original.size() );
    EXPECT_EQ( nullptr         , original.data() );

}

// Test destructor fires when we want it to.

struct cheat { static int count; ~cheat() { ++count; } };
int cheat::count = 0;

TEST( StackTest, Destruct )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    {
        C3::Stack< cheat > original( nframes, ncolumns, nrows );
    }

    EXPECT_EQ( nframes * ncolumns * nrows, cheat::count );

}

// Run the tests.

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
