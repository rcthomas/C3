
#include "gtest/gtest.h"

#include "C3_Frame.hh"

TEST( FrameTest, Construct ) 
{

    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Frame< int > container( ncolumns, nrows );

    EXPECT_EQ( ncolumns, container.ncolumns() );
    EXPECT_EQ( nrows   , container.nrows()    );

}

TEST( FrameTest, ConstructWithInitialization ) 
{

    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Frame< int > container( ncolumns, nrows, 3 );

    EXPECT_EQ( ncolumns, container.ncolumns() );
    EXPECT_EQ( nrows   , container.nrows()    );
    for( auto pixel : container ) EXPECT_EQ( 3, pixel );

}

TEST( FrameTest, AccessByCoordinates ) 
{

    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Frame< int > container( ncolumns, nrows );

    auto pos = 0;
    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j ) container( j, k ) = pos++;
    }

    pos = 0;
    for( auto pixel : container ) EXPECT_EQ( pos++, pixel );

    pos = 100;
    for( auto& pixel : container ) pixel = pos++;

    pos = 100;
    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j ) EXPECT_EQ( pos++, container( j, k ) );
    }

}

TEST( FrameTest, AssignPixel ) 
{

    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Frame< int > container( ncolumns, nrows );

    auto const number = 312211;

    container = number;
    for( auto pixel : container ) EXPECT_EQ( number, pixel );

}

TEST( FrameTest, ConvertValueType ) 
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< double > container( ncolumns, nrows );

    container( 0, 0 ) = 1.1;
    container( 1, 0 ) = 11.2;
    container( 0, 1 ) = 21.3;
    container( 1, 1 ) = 1211.4;
    container( 0, 2 ) = 111221.5;
    container( 1, 2 ) = 312211.5;

    auto other = C3::Frame< int >( container );

    EXPECT_EQ( 1     , other( 0, 0 ) );
    EXPECT_EQ( 11    , other( 1, 0 ) );
    EXPECT_EQ( 21    , other( 0, 1 ) );
    EXPECT_EQ( 1211  , other( 1, 1 ) );
    EXPECT_EQ( 111221, other( 0, 2 ) );
    EXPECT_EQ( 312211, other( 1, 2 ) );

}
