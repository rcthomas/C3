
#include <cmath>
#include <iostream>

#include <gsl/gsl_statistics.h>

#include <yaml-cpp/yaml.h>

#include "C3_Application.hh"
#include "C3_Column.hh"
#include "C3_Context.hh"
#include "C3_Parallel.hh"
#include "C3_View.hh"

#include "DECam.hh"

// The developer makes a class like this, with a "process()" method taking a YAML::Node.
// The "Context" template argument is the thing that gives developers access to big deal
// resources like file system access (that may need to be coordinated in parallel) and 
// MPI communicators (in parallel).  In serial the context doesn't do much, but people
// should use the context interface if they want to scale their engine up and do 
// at least collective I/O.

namespace DECam
{

    template< class Context >
    class TestEngine
    {

        public :

            /// Constructor.
            TestEngine();

            /// Destructor.
            ~TestEngine();

            /// Process a task.
            void process( const YAML::Node& task );

        private :

            int _counter;

    };

}

template< class Context >
inline DECam::TestEngine< Context >::TestEngine() :
    _counter( 0 )
{
    std::cout << Context::instance().frame() << " : test engine start-up" << std::endl;
}

template< class Context >
inline DECam::TestEngine< Context >::~TestEngine()
{
    std::cout << Context::instance().frame() << " : test engine shut-down, processed " << _counter << " tasks)" << std::endl;
}

template< class Context >
inline void DECam::TestEngine< Context >::process( const YAML::Node& task )
{

//  using data_type = double;
//  using flag_type = unsigned short int;

//  // Context handle and frame task parameters.

//  Context&          context = Context::instance();
//  const YAML::Node& config  = context.config();
//  const YAML::Node& my_task = task[ "meta" ][ context.frame() ];

//  // Input frame.
//  
//  auto input_path = config[ "rootdir" ].as< std::string >() + task[ "path" ].as< std::string >();
//  auto ncolumns   = my_task[ "naxis1" ].as< int >();
//  auto nrows      = my_task[ "naxis2" ].as< int >();

//  C3::Frame< data_type > input( ncolumns, nrows );
//  context.load( input, input_path );

//  // Output data, inverse variance, and flag frames.

//  auto datasec          = my_task[ "datasec" ].as< std::vector< int > >();
//  auto datasec_ncolumns = datasec[ 1 ] - datasec[ 0 ] + 1;
//  auto datasec_nrows    = datasec[ 3 ] - datasec[ 2 ] + 1;

//  C3::Frame< data_type > output( datasec_ncolumns, datasec_nrows );
//  C3::Frame< data_type > invvar( datasec_ncolumns, datasec_nrows );
//  C3::Frame< flag_type > flags ( datasec_ncolumns, datasec_nrows );

//  // Iterate over amplifier.

//  for( std::string amp : { "a", "b" } )
//  {

//      // Data sections.

//      auto section = my_task[ "datasec" + amp ].as< std::vector< int > >();
//      C3::View< data_type >  input_data = C3::View< data_type >::iraf_style(  input, section[ 0 ], section[ 1 ], section[ 2 ], section[ 3 ] );

//      C3::View< data_type > output_data( output, input_data.ncolumns(), input_data.nrows(), 
//              section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );
//      C3::View< data_type > invvar_data( invvar, input_data.ncolumns(), input_data.nrows(), 
//              section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );
//      C3::View< flag_type >  flags_data(  flags, input_data.ncolumns(), input_data.nrows(), 
//              section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );

//      // Input overscan section.

//      section.clear();
//      section = my_task[ "biassec" + amp ].as< std::vector< int > >();
//      C3::View< data_type > overscan = C3::View< data_type >::iraf_style( input, section[ 0 ], section[ 1 ], section[ 2 ], section[ 3 ] );

//      // Estimate and subtract overscan, multiply by gain.

//      auto gain  = my_task[ "gain" + amp ].as< data_type >();
//      auto gain2 = gain * gain;

//      auto rdnoise  = my_task[ "rdnoise" + amp ].as< data_type >();
//      auto rdnoise2 = rdnoise * rdnoise;

//      C3::OwnedBlock< data_type > weights( overscan.ncolumns() );
//      C3::OwnedBlock< data_type > buffer ( overscan.ncolumns() );

//      for( auto k = 0; k < overscan.nrows(); ++ k )
//      {

//          // Copy overscan row into buffer.  FIXME: make C3::assign( buffer, overscan.row( k ) ) or buffer = overscan.row( k );

//          for( auto j = 0; j < overscan.ncolumns(); ++ j ) buffer[ j ] = overscan( j, k );

//          // Median value.

//          std::sort( buffer.begin(), buffer.end() );
//          auto median = gsl_stats_median_from_sorted_data( buffer.data(), 1, buffer.size() );

//          // Normalized median absolute deviation.

//          for( auto j = 0; j < overscan.ncolumns(); ++ j ) buffer[ j ] = std::abs( buffer[ j ] - median );
//          std::sort( buffer.begin(), buffer.end() );
//          auto inv_nmad = 1.0 / ( 1.4826 * gsl_stats_median_from_sorted_data( buffer.data(), 1, buffer.size() ) );

//          // Mask outliers.

//          for( auto j = 0; j < overscan.ncolumns(); ++ j ) weights[ j ] = inv_nmad * std::abs( overscan( j, k ) - median ) < 3.0;

//          // Weighted mean and variance of overscan row.

//          for( auto j = 0; j < overscan.ncolumns(); ++ j ) buffer[ j ] = overscan( j, k );    // derh.
//          auto wmean = gsl_stats_wmean( weights.data(), 1, buffer.data(), 1, buffer.size() );
//          auto wvar  = gsl_stats_wvariance_m( weights.data(), 1, buffer.data(), 1, buffer.size(), wmean );

//          // Commit subtracted bias and multiply by gain.

//          for( auto j = 0; j < output_data.ncolumns(); ++ j ) 
//          {
//              output_data( j, k ) = gain * ( input_data( j, k ) - wmean );
//              invvar_data( j, k ) = 1.0 / ( rdnoise2 + gain2 * wvar );
//              flags_data ( j, k ) = 0;
//          }

//      }

//  }

//  // Write the output.

//  auto output_path = task[ "output" ].as< std::string >();
//  context.save( output, invvar, flags, output_path );

//  // Increment task counter.

//  ++ _counter;

}

// Run the application.

int main( int argc, char* argv[] )
{
    return C3::Application< C3::Parallel, DECam::Traits, DECam::TestEngine >::run( argc, argv );
}
