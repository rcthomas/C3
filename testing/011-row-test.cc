
#include "gtest/gtest.h"

#include "C3_Row.hh"

// Test constructing the container.

TEST( RowTest, Construct ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

    EXPECT_EQ( ncolumns, container.ncolumns() );

}

// Test block access.

TEST( RowTest, BlockAccess )
{

    C3::size_type ncolumns = 5;
    C3::Row< unsigned long int > container( ncolumns );

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

TEST( RowTest, GetAndSetCoordinateSubscript ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

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

TEST( RowTest, CopyConstruct )
{

    C3::size_type ncolumns = 5;
    C3::Row< int > original( ncolumns );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;

    C3::Row< int > container( original );
    EXPECT_EQ( 1        , container( 0 ) );
    EXPECT_EQ( 11       , container( 1 ) );
    EXPECT_EQ( 21       , container( 2 ) );
    EXPECT_EQ( 1211     , container( 3 ) );
    EXPECT_EQ( 111221   , container( 4 ) );

}

// Test move constructor.

C3::Row< int > row_func( const C3::size_type ncolumns, int*& data ) 
{ 
    C3::Row< int > tmp( ncolumns );
    data = tmp.block().data();
    return tmp;
}

TEST( RowTest, MoveConstruct )
{

    int* data;
    C3::size_type ncolumns = 5;
    C3::Row< int > container = row_func( ncolumns, data );

    EXPECT_EQ( ncolumns, container.ncolumns()       );
    EXPECT_EQ( data    , container.block().data()   );

}

// Test copy assignment operator.

TEST( RowTest, CopyAssign )
{

    C3::size_type ncolumns = 5;
    C3::Row< int > original( ncolumns );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;

    C3::Row< int > container( ncolumns ); 
    container = original;

    EXPECT_EQ( 1        , container( 0 ) );
    EXPECT_EQ( 11       , container( 1 ) );
    EXPECT_EQ( 21       , container( 2 ) );
    EXPECT_EQ( 1211     , container( 3 ) );
    EXPECT_EQ( 111221   , container( 4 ) );

}

// Test move assignment operator.

TEST( RowTest, MoveAssign )
{

    C3::size_type ncolumns = 5;
    C3::Row< int > original( ncolumns );
    original( 0 ) = 1;
    original( 1 ) = 11;
    original( 2 ) = 21;
    original( 3 ) = 1211;
    original( 4 ) = 111221;
    int* original_data = original.block().data();

    C3::Row< int > container( ncolumns + 1 );
    container = std::move( original );

    EXPECT_EQ( ncolumns      , container.ncolumns()     );
    EXPECT_EQ( original_data , container.block().data() );
    EXPECT_EQ( 0             , original.ncolumns()      );
    EXPECT_EQ( nullptr       , original.block().data()  );

}

// Test destructor fires when we want it to.

struct row_cheat { static int count; ~row_cheat() { ++count; } };
int row_cheat::count = 0;

TEST( RowTest, Destruct )
{

    C3::size_type size = 5;
    {
        C3::Row< row_cheat > original( size );
    }

    EXPECT_EQ( size, row_cheat::count );

}
