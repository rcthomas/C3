
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
    EXPECT_EQ( nframes * ncolumns * nrows, container.block().size() );

}

// Test block access.

TEST( StackTest, BlockAccess )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< unsigned long int > container( nframes, ncolumns, nrows );

    container( 0, 0, 0 ) = 1u;
    container( 1, 0, 0 ) = 11u;
    container( 1, 1, 0 ) = 21u;
    container( 0, 0, 1 ) = 1211u;
    container( 0, 0, 2 ) = 111221u;
    container( 1, 2, 3 ) = 312211u;

    C3::OwnedBlock< unsigned long int > block( 24 );
    block [ 0  ] = 312211u;
    block [ 1  ] = 13112221u;
    block [ 3  ] = 1113213211u;
    block [ 6  ] = 31131211131221u;
    block [ 12 ] = 13211311123113112211u;
    block [ 23 ] = 11221131132111311231u;
    container.block() = block;

    EXPECT_EQ( 312211u                  , container( 0, 0, 0 ) );
    EXPECT_EQ( 13112221u                , container( 1, 0, 0 ) );
    EXPECT_EQ( 1113213211u              , container( 1, 1, 0 ) );
    EXPECT_EQ( 31131211131221u          , container( 0, 0, 1 ) );
    EXPECT_EQ( 13211311123113112211u    , container( 0, 0, 2 ) );
    EXPECT_EQ( 11221131132111311231u    , container( 1, 2, 3 ) );

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

C3::Stack< int > stack_func( const C3::size_type nframes, const C3::size_type ncolumns, const C3::size_type nrows, int*& data )
{ 
    C3::Stack< int > tmp( nframes, ncolumns, nrows );
    data = tmp.block().data();
    return tmp;
}

TEST( StackTest, MoveConstruct )
{

    int* data;
    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container = stack_func( nframes, ncolumns, nrows, data );

    EXPECT_EQ( nframes , container.nframes()        );
    EXPECT_EQ( ncolumns, container.ncolumns()       );
    EXPECT_EQ( nrows   , container.nrows()          );
    EXPECT_EQ( data    , container.block().data()   );

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
    int* original_data = original.block().data();

    C3::Stack< int > container( nframes + 13, ncolumns + 1, nrows + 3 );
    container = std::move( original );

    EXPECT_EQ( nframes * ncolumns * nrows, container.block().size() );
    EXPECT_EQ( nframes         , container.nframes() );
    EXPECT_EQ( ncolumns        , container.ncolumns() );
    EXPECT_EQ( nrows           , container.nrows() );
    EXPECT_EQ( original_data   , container.block().data() );
    EXPECT_EQ( 1               , container( 0, 0, 0 ) );
    EXPECT_EQ( 11              , container( 1, 0, 0 ) );
    EXPECT_EQ( 21              , container( 1, 1, 0 ) );
    EXPECT_EQ( 1211            , container( 0, 0, 1 ) );
    EXPECT_EQ( 111221          , container( 0, 0, 2 ) );
    EXPECT_EQ( 312211          , container( 1, 2, 3 ) );
    EXPECT_EQ( 0               , original.block().size() );
    EXPECT_EQ( nullptr         , original.block().data() );

}

// Test destructor fires when we want it to.

struct stack_cheat { static int count; ~stack_cheat() { ++count; } };
int stack_cheat::count = 0;

TEST( StackTest, Destruct )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    {
        C3::Stack< stack_cheat > original( nframes, ncolumns, nrows );
    }

    EXPECT_EQ( nframes * ncolumns * nrows, stack_cheat::count );

}
