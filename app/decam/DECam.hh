#ifndef DECAM__HH
#define DECAM__HH

#include <string>
#include <vector>

namespace DECam
{

    /// @class Traits
    /// @brief Type information for the Dark Energy Camera.

    struct Traits
    {

        static const std::vector< std::string > frames;

        static bool frame_exists( const std::string& frame );
    
    };

}

const std::vector< std::string > DECam::Traits::frames( { "S1", "S2", "S3", "N1", "N2", "N3", "S8", "S9", "S14", "S15", "S20", "S25", "N8", "N9", "N14", "N15",
        "N20", "N25", "S10", "S11", "S12", "S13", "S18", "S19", "S16", "S17", "S21", "S22", "S23", "S24", "S26", "S27", "S28", "S29", "S30", "S31", "N4", "N5",
        "N6", "N7", "S4", "S5", "S6", "S7", "N10", "N11", "N12", "N13", "N18", "N19", "N16", "N17", "N21", "N22", "N23", "N24", "N26", "N27", "N28", "N29",
        "N30", "N31" } ); // , "FS1", "FS2", "FS3", "FS4", "FN1", "FN2", "FN3", "FN4" } );

inline bool DECam::Traits::frame_exists( const std::string& frame )
{
    return std::find( frames.begin(), frames.end(), frame ) != frames.end();
}

#endif

// HDU  usually
//      chip
//  2   S1
//  3   S2
//  4   S3
//  5   N1
//  6   N2
//  7   N3
//  8   S8
//  9   S9
// 10   S14
// 11   S15
// 12   S20
// 13   S25
// 14   N8
// 15   N9
// 16   N14
// 17   N15
// 18   N20
// 19   N25
// 20   S10
// 21   S11
// 22   S12
// 23   S13
// 24   S18
// 25   S19
// 26   S16
// 27   S17
// 28   S21
// 29   S22
// 30   S23
// 31   S24
// 32   S26
// 33   S27
// 34   S28
// 35   S29
// 36   S30
// 37   S31
// 38   N4
// 39   N5
// 40   N6
// 41   N7
// 42   S4
// 43   S5
// 44   S6
// 45   S7
// 46   N10
// 47   N11
// 48   N12
// 49   N13
// 50   N18
// 51   N19
// 52   N16
// 53   N17
// 54   N21
// 55   N22
// 56   N23
// 57   N24
// 58   N26
// 59   N27
// 60   N28
// 61   N29
// 62   N30
// 63   N31
// 64   FS1
// 65   FS2
// 66   FS3
// 67   FS4
// 68   FN1
// 69   FN2
// 70   FN3
// 71   FN4
