
#include <cmath>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "C3_Application.hh"
#include "C3_Column.hh"
#include "C3_Context.hh"
#include "C3_Parallel.hh"
#include "C3_Serial.hh"
#include "C3_View.hh"

#include "DECam.hh"

namespace DECam
{

    /// @class Overscan
    /// @brief Performs overscan subtraction.
    ///
    /// Simple example of a processing engine that does overscan subtraction on
    /// DECam images.  Developers make templates like this one, that at a
    /// minimum exposes a public void "process()" method accepting a single
    /// YAML::Node document.  If the engine needs any configuration, this can
    /// be done by defining a default constructor that initializes the engine
    /// using configuration data provided by a Context.  The "Context" template
    /// argument is the thing that gives developers access to big deal
    /// resources like file system access (that may need to be coordinated in
    /// parallel) and MPI communicators (in parallel).  In serial the context
    /// doesn't do much, but people should use the context interface if they
    /// want to scale their engine up and do at least collective I/O.

    template< class Context >
    struct Overscan
    {

        /// Process a task.
        void process( const YAML::Node& task );

    };

}

// Definition of the process method.  It can be more or less elaborate.  The GSL
// stuff is temporary.

template< class Context >
inline void DECam::Overscan< Context >::process( const YAML::Node& task )
{

    using data_type = double;
    using flag_type = unsigned short int;

    // Context handle and frame task parameters.

    Context&     context = Context::instance();
    C3::Logger&  logger  = context.logger();

    const YAML::Node& config  = context.config();
    const YAML::Node& my_task = task[ "meta" ][ context.frame() ];

    // Input frame.
    
    auto input_path = config[ "input_root" ].as< std::string >() + task[ "relpath" ].as< std::string >();
    auto ncolumns   = my_task[ "naxis1" ].as< int >();
    auto nrows      = my_task[ "naxis2" ].as< int >();

    logger.info( "Loading from input exposure", input_path );

    C3::Frame< data_type > input( ncolumns, nrows );
    context.load( input, input_path );

    // Output data, inverse variance, and flag frames.

    auto datasec          = my_task[ "datasec" ].as< std::vector< int > >();
    auto datasec_ncolumns = datasec[ 1 ] - datasec[ 0 ] + 1;
    auto datasec_nrows    = datasec[ 3 ] - datasec[ 2 ] + 1;

    C3::Frame< data_type > output( datasec_ncolumns, datasec_nrows );
    C3::Frame< data_type > invvar( datasec_ncolumns, datasec_nrows );
    C3::Frame< flag_type > flags ( datasec_ncolumns, datasec_nrows );

    // Iterate over amplifier.

    for( std::string amp : { "a", "b" } )
    {

        logger.debug( "Amplifier:", amp );

        // Data sections.

        auto section = my_task[ "datasec" + amp ].as< std::vector< int > >();
        C3::View< data_type >  input_data = C3::View< data_type >::iraf_style(  input, section[ 0 ], section[ 1 ], section[ 2 ], section[ 3 ] );

        C3::View< data_type > output_data( output, input_data.ncolumns(), input_data.nrows(), 
                section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );
        C3::View< data_type > invvar_data( invvar, input_data.ncolumns(), input_data.nrows(), 
                section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );
        C3::View< flag_type >  flags_data(  flags, input_data.ncolumns(), input_data.nrows(), 
                section[ 0 ] - datasec[ 0 ], section[ 2 ] - datasec[ 2 ]  );

        // Input overscan section.

        section.clear();
        section = my_task[ "biassec" + amp ].as< std::vector< int > >();
        C3::View< data_type > overscan = C3::View< data_type >::iraf_style( input, section[ 0 ], section[ 1 ], section[ 2 ], section[ 3 ] );

        // Estimate and subtract overscan, multiply by gain.

        auto gain  = my_task[ "gain" + amp ].as< data_type >();
        auto gain2 = gain * gain;

        auto rdnoise  = my_task[ "rdnoise" + amp ].as< data_type >();
        auto rdnoise2 = rdnoise * rdnoise;

        logger.debug( "Using [ gain =", gain,"] and [ rdnoise =", rdnoise, "]." );

        C3::Block< data_type > weights( overscan.ncolumns() );
        C3::Block< data_type > buffer ( overscan.ncolumns() );

        for( auto k = 0; k < overscan.nrows(); ++ k )
        {

            // Copy overscan row into buffer.

            for( auto j = 0; j < overscan.ncolumns(); ++ j ) buffer[ j ] = overscan( j, k );

            // Median value.

            std::sort( buffer.begin(), buffer.end() );
            auto median = buffer[ buffer.size() / 2 ];

            // Normalized median absolute deviation.

            for( auto j = 0; j < overscan.ncolumns(); ++ j ) buffer[ j ] = std::abs( buffer[ j ] - median );
            std::sort( buffer.begin(), buffer.end() );
            auto nmad = 1.4826 * buffer[ buffer.size() / 2 ];

            // Mask outliers.

            for( auto j = 0; j < overscan.ncolumns(); ++ j ) weights[ j ] = std::abs( overscan( j, k ) - median ) < 3.0 * nmad;

            // Weighted mean and variance of overscan row.

            auto wmean = 0.0;
            auto wsum  = 0.0;
            for( auto j = 0; j < overscan.ncolumns(); ++ j ) 
            {
                auto weight = weights[ j ];
                wmean += weight * overscan( j, k );
                wsum  += weight;
            }
            wmean /= wsum;

            auto wvar  = 0.0;
            auto w2sum = 0.0;
            for( auto j = 0; j < overscan.ncolumns(); ++ j ) 
            {
                auto weight = weights[ j ];
                auto diff   = overscan( j, k ) - wmean;
                wvar  += weight * diff * diff;
                w2sum += weight * weight;
            }
            wvar = wvar / ( wsum - w2sum / wsum );

            // Commit subtracted bias and multiply by gain.

            for( auto j = 0; j < output_data.ncolumns(); ++ j ) 
            {
                output_data( j, k ) = gain * ( input_data( j, k ) - wmean );
                invvar_data( j, k ) = 1.0 / ( rdnoise2 + gain2 * wvar );
                flags_data ( j, k ) = 0;
            }

        }

    }

    // Write the output.

    auto output_path = config[ "output_root" ].as< std::string >() + task[ "output" ].as< std::string >();
    context.template save< float >( output, invvar, flags, output_path );

}
