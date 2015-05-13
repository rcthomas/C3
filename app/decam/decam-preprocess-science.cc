#include "yaml-cpp/yaml.h"

#include "C3.hh"
#include "DECam.hh"

using Application = C3::Application< DECam::DECam, YAML::Node, DECam::Preprocess >;

int main( int argc, char* argv[] )
{
    return Application::run( argc, argv );
}
