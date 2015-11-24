
#ifdef PARALLEL
#include "C3_Parallel.hh"
#else
#include "C3_Serial.hh"
#endif

#include "DECam_Overscan.hh"

int main( int argc, char* argv[] )
{
#ifdef PARALLEL
    return C3::Application< C3::Parallel, DECam::Traits, DECam::Overscan >::run( argc, argv );
#else
    return C3::Application< C3::Serial, DECam::Traits, DECam::Overscan >::run( argc, argv );
#endif
}
