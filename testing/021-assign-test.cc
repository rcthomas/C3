
#include "gtest/gtest.h"

#include "C3_Block.hh"
#include "C3_Column.hh"
#include "C3_Frame.hh"
#include "C3_Row.hh"
#include "C3_Stack.hh"

// Scalar to block.

TEST( AssignTest, ScalarToBlock )
{

    C3::size_type size = 5;
    C3::Block< int > container( size );

    int value = 3;
    C3::assign( container, value );

    for( auto k = 0; k < size; ++k ) EXPECT_EQ( value, container[ k ] );

}

// Scalar to column.

TEST( AssignTest, ScalarToColumn )
{

    C3::size_type nrows = 5;
    C3::Column< int > container( nrows );

    int value = 3;
    C3::assign( container, value );

    for( auto k = 0; k < nrows; ++k ) EXPECT_EQ( value, container( k ) );

}

// Scalar to row.

TEST( AssignTest, ScalarToRow )
{

    C3::size_type ncolumns = 5;
    C3::Row< int > container( ncolumns );

    int value = 2;
    C3::assign( container, value );

    for( auto j = 0; j < ncolumns; ++j ) EXPECT_EQ( value, container( j ) );

}

// Scalar to frame.

TEST( AssignTest, ScalarToFrame )
{

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container( ncolumns, nrows );

    int value = 1;
    C3::assign( container, value );

    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j ) 
        {
            EXPECT_EQ( value, container( j, k ) );
        }
    }

}

// Scalar to stack.

TEST( AssignTest, ScalarToStack )
{

    C3::size_type nframes  = 2;
    C3::size_type ncolumns = 3;
    C3::size_type nrows    = 4;
    C3::Stack< int > container( nframes, ncolumns, nrows );

    int value = 9;
    C3::assign( container, value );

    for( auto k = 0; k < nrows; ++k )
    {
        for( auto j = 0; j < ncolumns; ++j ) 
        {
            for( auto i = 0; i < nframes; ++i ) 
            {
                EXPECT_EQ( value, container( i, j, k ) );
            }
        }
    }

}

// Column to column.

TEST( AssignTest, ColumnToColumn )
{

    C3::size_type size = 5;
    C3::Column< int > container1( size );
    C3::Column< int > container2( size );
    container1( 0 ) = 1;
    container1( 1 ) = 11;
    container1( 2 ) = 21;
    container1( 3 ) = 1211;
    container1( 4 ) = 111221;;

    C3::assign( container2, container1 );
    EXPECT_EQ( container1( 0 ), container2( 0 ) );
    EXPECT_EQ( container1( 1 ), container2( 1 ) );
    EXPECT_EQ( container1( 2 ), container2( 2 ) );
    EXPECT_EQ( container1( 3 ), container2( 3 ) );
    EXPECT_EQ( container1( 4 ), container2( 4 ) );

}

// Column to column wrong size.

TEST( AssignDeathTest, ColumnToColumnMismatchSize )
{

    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    C3::size_type size = 5;
    C3::Column< int > container1( size );
    C3::Column< int > container2( size + 1 );

    EXPECT_DEATH( C3::assign( container2, container1 ), "" );

}

// Row to row wrong size.
 
TEST( AssignDeathTest, RowToRowMismatchSize )
{

    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    C3::size_type size = 5;
    C3::Row< int > container1( size );
    C3::Row< int > container2( size + 1 );

    EXPECT_DEATH( C3::assign( container2, container1 ), "" );

}
 
// Frame to frame wrong size.

TEST( AssignDeathTest, FrameToFrameMismatchSize )
{

    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container1( ncolumns, nrows     );
    C3::Frame< int > container2( ncolumns, nrows + 1 );

    EXPECT_DEATH( C3::assign( container2, container1 ), "" );

}

// Frame to frame wrong shape.

TEST( AssignDeathTest, FrameToFrameMismatchShape )
{

    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    C3::size_type ncolumns = 2;
    C3::size_type nrows    = 3;
    C3::Frame< int > container1( ncolumns, nrows );
    C3::Frame< int > container2( nrows, ncolumns );

    EXPECT_DEATH( C3::assign( container2, container1 ), "" );

}
