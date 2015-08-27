
#include "gtest/gtest.h"

#include "C3_Row.hh"

TEST( RowTest, Construct ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

    EXPECT_EQ( ncolumns, container.ncolumns() );

}

TEST( RowTest, ConstructWithInitialization ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns, 3 );

    EXPECT_EQ( ncolumns, container.ncolumns() );
    for( auto pixel : container ) EXPECT_EQ( 3, pixel );

}

TEST( RowTest, AccessByCoordinates ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

    container( 0 ) = 1;
    container( 1 ) = 11;
    container( 2 ) = 21;
    container( 3 ) = 1211;
    container( 4 ) = 111221;

    EXPECT_EQ( 1     , container( 0 ) );
    EXPECT_EQ( 11    , container( 1 ) );
    EXPECT_EQ( 21    , container( 2 ) );
    EXPECT_EQ( 1211  , container( 3 ) );
    EXPECT_EQ( 111221, container( 4 ) );

}

TEST( RowTest, AssignPixel ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

    auto const pixel = 312211;

    container = pixel;

    EXPECT_EQ( pixel, container( 0 ) );
    EXPECT_EQ( pixel, container( 1 ) );
    EXPECT_EQ( pixel, container( 2 ) );
    EXPECT_EQ( pixel, container( 3 ) );
    EXPECT_EQ( pixel, container( 4 ) );

}

TEST( RowTest, ConvertValueType ) 
{

    C3::size_type ncolumns = 5;
    C3::Row< double > container( ncolumns );

    container( 0 ) = 1.1;
    container( 1 ) = 11.2;
    container( 2 ) = 21.3;
    container( 3 ) = 1211.4;
    container( 4 ) = 111221.5;

    auto other = C3::Row< int >( container );

    EXPECT_EQ( 1     , other( 0 ) );
    EXPECT_EQ( 11    , other( 1 ) );
    EXPECT_EQ( 21    , other( 2 ) );
    EXPECT_EQ( 1211  , other( 3 ) );
    EXPECT_EQ( 111221, other( 4 ) );

}
