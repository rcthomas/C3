
#include "gtest/gtest.h"

#include "C3_Column.hh"

// Test constructing the container.

TEST( ColumnTest, Construct ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

    EXPECT_EQ( nrows, container.nrows() );

}

// Test block access.

TEST( ColumnTest, BlockAccess )
{

    C3::size_type nrows = 5;
    C3::Column< unsigned long int > container( nrows );

    // Settem ...
    container( 0 ) = 1u;
    container( 1 ) = 11u;
    container( 2 ) = 21u;
    container( 3 ) = 1211u;
    container( 4 ) = 111221u;

    C3::OwnedBlock< unsigned long int > block( 5 );
    block[ 0 ] = 312211u;
    block[ 1 ] = 13112221u;
    block[ 2 ] = 1113213211u;
    block[ 3 ] = 31131211131221u;
    block[ 4 ] = 13211311123113112211u;
    container.block() = block;

    EXPECT_EQ( 312211u              , container( 0 ) );
    EXPECT_EQ( 13112221u            , container( 1 ) );
    EXPECT_EQ( 1113213211u          , container( 2 ) );
    EXPECT_EQ( 31131211131221u      , container( 3 ) );
    EXPECT_EQ( 13211311123113112211u, container( 4 ) );

}

// Test that we can get and set by coordinate subscript.

TEST( ColumnTest, GetAndSetCoordinateSubscript ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

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

// Test we can construct a container from another one.

TEST( ColumnTest, CopyConstruct )
{

    C3::size_type nrows = 5;
    C3::Column< int > original( nrows );
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

TEST( ColumnTest, MoveConstruct )
{

    C3::size_type nrows = 5;
    C3::Column< int > source( nrows );
    int* data = source.block().data();

    C3::Column< int > container = std::move( source );

    EXPECT_EQ(    data, container.block().data() );
    EXPECT_EQ( nullptr,    source.block().data() );

}

// Test copy assignment operator.

TEST( ColumnTest, CopyAssign )
{

    C3::size_type nrows = 5;
    C3::Column< int > original( nrows );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;

    C3::Column< int > container( nrows ); 
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

    C3::size_type nrows = 5;
    C3::Column< int > original( nrows );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;
    int* original_data = original.block().data();

    C3::Column< int > container( nrows + 1 );
    container = std::move( original );

    EXPECT_EQ( nrows         , container.nrows()        );
    EXPECT_EQ( original_data , container.block().data() );
    EXPECT_EQ( 0             , original.nrows()         );
    EXPECT_EQ( nullptr       , original.block().data()  );

}

// Test destructor fires when we want it to.

struct column_cheat { static int count; ~column_cheat() { ++count; } };
int column_cheat::count = 0;

TEST( ColumnTest, Destruct )
{

    C3::size_type size = 5;
    {
        C3::Column< column_cheat > original( size );
    }

    EXPECT_EQ( size, column_cheat::count );

}

