
#include "gtest/gtest.h"

#include "C3_Column.hh"

// Test constructing the container.

TEST( ColumnTest, Construct ) 
{

    C3::size_type size = 5;
    C3::Column< int > container( size );

    EXPECT_EQ( size, container.nrows() );
    EXPECT_EQ( size, container.size()  );

}

// Test that we can get and set by linear array subscript.

TEST( ColumnTest, GetAndSetArraySubscript ) 
{

    C3::size_type size = 5;
    C3::Column< int > container( size );

    // Settem ...
    container[ 0 ] = 1;
    container[ 1 ] = 11;
    container[ 2 ] = 21;
    container[ 3 ] = 1211;
    container[ 4 ] = 111221;

    // ... ngettem.
    EXPECT_EQ( 1        , container[ 0 ] );
    EXPECT_EQ( 11       , container[ 1 ] );
    EXPECT_EQ( 21       , container[ 2 ] );
    EXPECT_EQ( 1211     , container[ 3 ] );
    EXPECT_EQ( 111221   , container[ 4 ] );

}

// Test that we can get and set by coordinate subscript.

TEST( ColumnTest, GetAndSetCoordinateSubscript ) 
{

    C3::size_type size = 5;
    C3::Column< int > container( size );

    // Settem ...
    container( 0 ) = 1;
    container( 1 ) = 11;
    container( 2 ) = 21;
    container( 3 ) = 1211;
    container( 4 ) = 111221;

    // ... ngettem.
    EXPECT_EQ( 1        , container( 0 ) );
    EXPECT_EQ( 11       , container( 1 ) );
    EXPECT_EQ( 21       , container( 2 ) );
    EXPECT_EQ( 1211     , container( 3 ) );
    EXPECT_EQ( 111221   , container( 4 ) );

}

// Text end is where it should be relative to beginning.  More extensive tests
// of use of begin() and end() happen with operators like assign, apply, etc.

TEST( ColumnTest, RawAccess )
{

    C3::size_type size = 5;
    C3::Column< int > container( size );

    EXPECT_EQ( container.data()       , container.begin() );
    EXPECT_EQ( container.data() + size, container.end()   );

}

// Test we can construct a container from another one.

TEST( ColumnTest, CopyConstruct )
{

    C3::size_type size = 5;
    C3::Column< int > original( size );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;

    C3::Column< int > container( original );
    EXPECT_EQ( 1        , container( 0 ) );
    EXPECT_EQ( 11       , container( 1 ) );
    EXPECT_EQ( 21       , container( 2 ) );
    EXPECT_EQ( 1211     , container( 3 ) );
    EXPECT_EQ( 111221   , container( 4 ) );

}

// Test move constructor.

C3::Column< int > func( const C3::size_type size, int*& data ) 
{ 
    C3::Column< int > tmp( size );
    data = tmp.data();
    return tmp;
}

TEST( ColumnTest, MoveConstruct )
{

    C3::size_type size = 5;
    int* ptr;
    C3::Column< int > container = func( size, ptr );

    EXPECT_EQ( size, container.size() );
    EXPECT_EQ( ptr , container.data() );

}

// Test copy assignment operator.

TEST( ColumnTest, CopyAssign )
{

    C3::size_type size = 5;
    C3::Column< int > original( size );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;

    C3::Column< int > container( size ); 
    container = original;

    EXPECT_EQ( 1        , container( 0 ) );
    EXPECT_EQ( 11       , container( 1 ) );
    EXPECT_EQ( 21       , container( 2 ) );
    EXPECT_EQ( 1211     , container( 3 ) );
    EXPECT_EQ( 111221   , container( 4 ) );

}

// Test move assignment operator.

TEST( ColumnTest, MoveAssign )
{

    C3::size_type size = 5;
    C3::Column< int > original( size );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;
    int* original_data = original.data();

    C3::Column< int > container( size + 1 );
    container = std::move( original );

    EXPECT_EQ( size         , container.size() );
    EXPECT_EQ( original_data, container.data() );
    EXPECT_EQ( 0            , original.size() );
    EXPECT_EQ( nullptr      , original.data() );

}

// Test destructor fires when we want it to.

struct cheat { static int count; ~cheat() { ++count; } };
int cheat::count = 0;

TEST( ColumnTest, Destruct )
{

    C3::size_type size = 5;
    {
        C3::Column< cheat > original( size );
    }

    EXPECT_EQ( size, cheat::count );

}

// Run the tests.

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
