
#include "DECam_Overscan.hh"

int main( int argc, char* argv[] )
{
    return C3::Application< C3::Serial, DECam::Traits, DECam::Overscan >::run( argc, argv );
}
