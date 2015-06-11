
#include "gtest/gtest.h"

#include "C3_Frame.hh"

// Test constructing the container.

TEST( FrameTest, Construct ) 
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    EXPECT_EQ( ncolumns         , container.ncolumns()      );
    EXPECT_EQ( nrows            , container.nrows()         );
    EXPECT_EQ( ncolumns * nrows , container.block().size()  );

}

// Test block access.

TEST( FrameTest, BlockAccess )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< unsigned long int > container( ncolumns, nrows );

    // Settem ...
    container( 0, 0 ) = 1u;
    container( 1, 0 ) = 11u;
    container( 0, 1 ) = 21u;
    container( 1, 1 ) = 1211u;
    container( 0, 2 ) = 111221u;
    container( 1, 2 ) = 312211u;

    C3::OwnedBlock< unsigned long int > block( 6 );
    block[ 0 ] = 312211u;
    block[ 1 ] = 13112221u;
    block[ 2 ] = 1113213211u;
    block[ 3 ] = 31131211131221u;
    block[ 4 ] = 13211311123113112211u;
    block[ 5 ] = 11221131132111311231u;
    container.block() = block;

    EXPECT_EQ( 312211u                  , container( 0, 0 ) );
    EXPECT_EQ( 13112221u                , container( 1, 0 ) );
    EXPECT_EQ( 1113213211u              , container( 0, 1 ) );
    EXPECT_EQ( 31131211131221u          , container( 1, 1 ) );
    EXPECT_EQ( 13211311123113112211u    , container( 0, 2 ) );
    EXPECT_EQ( 11221131132111311231u    , container( 1, 2 ) );

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

TEST( FrameTest, MoveConstruct )
{

    C3::size_type ncolumns = 4;
    C3::size_type nrows    = 5;
    C3::Frame< int > source( ncolumns, nrows );
    int* data = source.block().data();

    C3::Frame< int > container = std::move( source );

    EXPECT_EQ(     data, container.block().data() );
    EXPECT_EQ(  nullptr,    source.block().data() );

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
    int* original_data = original.block().data();

    C3::Frame< int > container( ncolumns + 1, nrows + 3 );
    container = std::move( original );

    EXPECT_EQ( ncolumns * nrows, container.block().size()   );
    EXPECT_EQ( ncolumns        , container.ncolumns()       );
    EXPECT_EQ( nrows           , container.nrows()          );
    EXPECT_EQ( original_data   , container.block().data()   );
    EXPECT_EQ( 1               , container( 0, 0 )          );
    EXPECT_EQ( 11              , container( 1, 0 )          );
    EXPECT_EQ( 21              , container( 0, 1 )          );
    EXPECT_EQ( 1211            , container( 1, 1 )          );
    EXPECT_EQ( 111221          , container( 0, 2 )          );
    EXPECT_EQ( 312211          , container( 1, 2 )          );
    EXPECT_EQ( 0               , original.block().size()    );
    EXPECT_EQ( nullptr         , original.block().data()    );

}

// Test destructor fires when we want it to.

struct frame_cheat { static int count; ~frame_cheat() { ++count; } };
int frame_cheat::count = 0;

TEST( FrameTest, Destruct )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    {
        C3::Frame< frame_cheat > original( ncolumns, nrows );
    }

    EXPECT_EQ( ncolumns * nrows, frame_cheat::count );

}
