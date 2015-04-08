
#include <iostream>
#include <string>

#include "yaml-cpp/yaml.h"

int main( int argc, char* argv[] )
{

    YAML::Node config = YAML::LoadFile( argv[ 1 ] );

    const std::string first_name = config[ "first_name" ].as< std::string >();
    const std::string  last_name = config[  "last_name" ].as< std::string >();

    std::cout << first_name << " " << last_name << std::endl;

    return 0;

}
