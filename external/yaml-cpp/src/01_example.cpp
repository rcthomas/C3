
#include <fstream>
#include <iostream>
#include <string>

#include <mpi.h>

#include "yaml-cpp/anchor.h"
#include "yaml-cpp/emitterstyle.h"
#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/yaml.h"
#include "nodebuilder.h"

namespace YAML
{
    struct Mark;
}

class DocumentCounter : public YAML::EventHandler 
{

    public :

        DocumentCounter( const size_t count = 0 ) :
            _count( count ) {}

        virtual ~DocumentCounter() {}

        virtual void OnDocumentStart(const YAML::Mark& mark) {}
        virtual void OnDocumentEnd() { ++ _count; }
        
        virtual void OnNull(const YAML::Mark& mark, YAML::anchor_t anchor) {}
        virtual void OnAlias(const YAML::Mark& mark, YAML::anchor_t anchor) {}
        virtual void OnScalar(const YAML::Mark& mark, const std::string& tag,
                              YAML::anchor_t anchor, const std::string& value) {}
        
        virtual void OnSequenceStart(const YAML::Mark& mark, const std::string& tag,
                                     YAML::anchor_t anchor, YAML::EmitterStyle::value style) {}
        virtual void OnSequenceEnd() {}
        
        virtual void OnMapStart(const YAML::Mark& mark, const std::string& tag,
                                YAML::anchor_t anchor, YAML::EmitterStyle::value style) {}
        virtual void OnMapEnd() {}

        size_t count() const { return _count; }

    private :

        size_t _count;

};


int main( int argc, char* argv[] )
{

    int status = MPI_Init( &argc, &argv );

    int rank = 0;
    status = MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if( rank == 0 )  std::cout << "slurping ......" << std::endl;

    char* content;

    std::ifstream stream( argv[ 1 ], std::ios::in | std::ios::binary);
    if( stream )
    {
        stream.seekg( 0, std::ios::end );
        size_t size = stream.tellg();
        content = new char [ size ];
        stream.seekg( 0, std::ios::beg );
        stream.read( content, size );
        stream.close();
    }

    if( rank == 0 ) std::cout << "slurping [done]" << std::endl;
//  if( rank == 0 ) std::cout << "parsing ......" << std::endl;

//  YAML::Parser parser( input );
//  DocumentCounter counter;
//  while( true ) 
//  {
//      if( ! parser.HandleNextDocument( counter ) ) break;
//      std::cout << counter.count() << std::endl;
//  }
//  std::cout << counter.count() << std::endl;
// 
//  std::istringstream ss( content ); 
//  YAML::Parser parser( ss );
//  while( true ) 
//  {
//      YAML::NodeBuilder builder;
//      if( ! parser.HandleNextDocument( builder ) ) break;
//      const YAML::Node& node = builder.Root();
//      if( rank == 0 ) std::cout << node[ "path" ] << std::endl;
//  }

//  if( rank == 0 ) std::cout << "parsing [done]" << std::endl;

    status = MPI_Finalize();
    return 0;

}
