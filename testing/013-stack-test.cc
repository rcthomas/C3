
#include "gtest/gtest.h"

#include "C3_Stack.hh"

TEST( StackTest, Construct ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    EXPECT_EQ( nframes , container.nframes()  );
    EXPECT_EQ( ncolumns, container.ncolumns() );
    EXPECT_EQ( nrows   , container.nrows()    );

}

TEST( StackTest, ConstructWithInitialization ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows, 37 );

    EXPECT_EQ( nframes , container.nframes()  );
    EXPECT_EQ( ncolumns, container.ncolumns() );
    EXPECT_EQ( nrows   , container.nrows()    );
    for( auto pixel : container ) EXPECT_EQ( 37, pixel );

}

TEST( StackTest, AccessByCoordinates ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    auto pos = 0;
    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j ) 
        {
            for( auto i = 0; i < nframes; ++i ) container( i, j, k ) = pos++;
        }
    }

    pos = 0;
    for( auto pixel : container ) EXPECT_EQ( pos++, pixel );

    pos = 531;
    for( auto& pixel : container ) pixel = pos++;

    pos = 531;
    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j )
        {
            for( auto i = 0; i < nframes; ++i ) EXPECT_EQ( pos++, container( i, j, k ) );
        }
    }

}

TEST( StackTest, AssignPixel ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    auto const number = 312211;

    container = number;
    for( auto pixel : container ) EXPECT_EQ( number, pixel );

}

TEST( StackTest, ConvertValueType ) 
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Stack< double > container( nframes, ncolumns, nrows );

    container( 0, 0, 0 ) = 1.1;
    container( 1, 0, 0 ) = 11.2;
    container( 0, 1, 0 ) = 21.3;
    container( 1, 1, 0 ) = 1211.4;
    container( 0, 0, 1 ) = 111221.5;
    container( 1, 0, 1 ) = 312211.5;
    container( 0, 1, 1 ) = 21.1;
    container( 1, 1, 1 ) = 211.2;
    container( 0, 0, 2 ) = 221.3;
    container( 1, 0, 2 ) = 21211.4;
    container( 0, 1, 2 ) = 2111221.5;
    container( 1, 1, 2 ) = 2312211.5;

    auto other = C3::Stack< int >( container );

    EXPECT_EQ( 1      , other( 0, 0, 0 ) );
    EXPECT_EQ( 11     , other( 1, 0, 0 ) );
    EXPECT_EQ( 21     , other( 0, 1, 0 ) );
    EXPECT_EQ( 1211   , other( 1, 1, 0 ) );
    EXPECT_EQ( 111221 , other( 0, 0, 1 ) );
    EXPECT_EQ( 312211 , other( 1, 0, 1 ) );
    EXPECT_EQ( 21     , other( 0, 1, 1 ) );
    EXPECT_EQ( 211    , other( 1, 1, 1 ) );
    EXPECT_EQ( 221    , other( 0, 0, 2 ) );
    EXPECT_EQ( 21211  , other( 1, 0, 2 ) );
    EXPECT_EQ( 2111221, other( 0, 1, 2 ) );
    EXPECT_EQ( 2312211, other( 1, 1, 2 ) );

}
