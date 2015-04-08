#ifndef C3__HEADER
#define C3__HEADER

#include <cstring>
#include <vector>
#include <string>

#include "fitsio.h"

namespace C3
{

    /// Raise exception if CFITSIO status passed is not 0.
    void assert_cfitsio_status( const int cfitsio_status );

    /// @class Header
    /// @brief Use this for debugging and development, not production.

    class Header
    {

        public :

            /// Create by loading from a FITS HDU.
            static Header create( fitsfile* fits );

            /// Constructor, copy.
            Header( const Header& header );

            /// Destructor.
            ~Header();

            /// Number of records.
            size_t size() const { return _size; }

            /// True if there are no records stored.
            bool empty() const { return _size == 0; }

            /// Remove all records.
            void clear();

            /// Return/update keyword of selected record.
            ///@{
            std::string keyword( const size_t pos ) const;
            void        keyword( const size_t pos, const std::string& update );
            ///@}

            /// Get or set typed value.  Tries to quote strings.
            ///@{
            template< typename T > T    get( const size_t pos ) const;
            template< typename T > void set( const size_t pos, const T typed_value );
            ///@}

            /// Return/update value (as string) of selected record (won't quote strings!).
            ///@{
            std::string value( const size_t pos ) const;
            void        value( const size_t pos, const std::string& update );
            ///@}

            /// Return/update comment of selected record.
            ///@{
            std::string comment( const size_t pos ) const;
            void        comment( const size_t pos, const std::string& update );
            ///@}

            /// Get or set typed value by keyword.  Tries to quote strings.
            ///@{
            template< typename T > T    get( const std::string& keyword, const size_t occurrence = 0 ) const;
            template< typename T > void set( const std::string& keyword, const T typed_value, const size_t occurrence = 0 );
            ///@}

            /// Return/update value by keyword (as string) of selected record (won't quote strings!).
            ///@{
            std::string value( const std::string& keyword, const size_t occurrence = 0 ) const;
            void        value( const std::string& keyword, const std::string& update, const size_t occurrence = 0 );
            ///@}

            /// Return/update comment of selected record by keyword.
            ///@{
            std::string comment( const std::string&, const size_t occurrence = 0 ) const;
            void        comment( const std::string&, const std::string& update, const size_t occurrence = 0 );
            ///@}

            /// Return record index matching keyword.
            size_t find( const std::string& keyword, const size_t occurrence = 0 ) const;

            /// Return record as keyword, value, comment strings.
            void extract( const size_t pos, std::string& keyword, std::string& value, std::string& comment ) const;

            /// Update a record at some position given keyword, value, comment strings.
            void replace( const size_t pos, const std::string& keyword, const std::string& value, const std::string& comment );

            /// Insert a record at some position given keyword, value, comment strings.
            void insert( const size_t pos, const std::string& keyword, const std::string& value, const std::string& comment );

            /// Remove a record at some position.
            void remove( const size_t pos );

            /// Assignment.
            Header& operator = ( const Header& header );

        private :

            /// Constructor.
            Header( const size_t size, char* text );

            size_t  _size;  ///< Number of records.
            char*   _text;  ///< Block of text.

    };

    /// Get or set std::string value (specialized method).
    ///@{
    template<> std::string Header::get< std::string >( const size_t pos ) const;
    template<> void Header::set< std::string >( const size_t pos, const std::string typed_value );
    ///@}

    /// Stream output.
    std::ostream& operator << ( std::ostream& stream, const Header& header );

}

#include "C3_Header.inl.hh"

#endif
