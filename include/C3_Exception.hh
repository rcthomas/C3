#ifndef C3_EXCEPTION_HH
#define C3_EXCEPTION_HH

#include <stdexcept>

namespace C3
{

    /// @class Exception
    /// @brief Base exception class.
    
    class Exception : public std::runtime_error
    {   
        
        public :    // Public methods.
            
            /// Named constructor for list of arguments to format into message string.
            template< class... T > static Exception create( const T&... args );
            
            /// Constructor.
            explicit Exception( const std::string& message ) :
                std::runtime_error( message ) {}
        
        protected : // Protected methods.
            
            /// Format list of arguments into message string. 
            template< class... T > static std::string format( const T&... args );
        
        private :   // Private methods.
            
            /// Format argument of any type into string stream.
            template< class T > 
            static std::stringstream& _format( std::stringstream& stream, const T& arg );
            
            /// Recursively format arguments of any type into string stream.
            template< class T, class... U >
            static std::stringstream& _format( std::stringstream& stream, const T& head, const U&... tail );
    
    };

}

#include "inline/C3_Exception.hh"

#endif
