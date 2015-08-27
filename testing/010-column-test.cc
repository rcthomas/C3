
#include "gtest/gtest.h"

#include "C3_Column.hh"

TEST( ColumnTest, Construct ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

    EXPECT_EQ( nrows, container.nrows() );

}

TEST( ColumnTest, ConstructWithInitialization ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows, 3 );

    EXPECT_EQ( nrows, container.nrows() );
    for( auto pixel : container ) EXPECT_EQ( 3, pixel );

}

TEST( ColumnTest, AccessByCoordinates ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

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

TEST( ColumnTest, AssignPixel ) 
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

    auto const pixel = 312211;

    container = pixel;

    EXPECT_EQ( pixel, container( 0 ) );
    EXPECT_EQ( pixel, container( 1 ) );
    EXPECT_EQ( pixel, container( 2 ) );
    EXPECT_EQ( pixel, container( 3 ) );
    EXPECT_EQ( pixel, container( 4 ) );

}

TEST( ColumnTest, ConvertValueType ) 
{

    C3::size_type nrows = 5;
    C3::Column< double > container( nrows );

    container( 0 ) = 1.1;
    container( 1 ) = 11.2;
    container( 2 ) = 21.3;
    container( 3 ) = 1211.4;
    container( 4 ) = 111221.5;

    auto other = C3::Column< int >( container );

    EXPECT_EQ( 1     , other( 0 ) );
    EXPECT_EQ( 11    , other( 1 ) );
    EXPECT_EQ( 21    , other( 2 ) );
    EXPECT_EQ( 1211  , other( 3 ) );
    EXPECT_EQ( 111221, other( 4 ) );

}
