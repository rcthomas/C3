#include <iostream>
#include <sstream>

#include "C3_Exception.hh"

// Create by loading from a FITS HDU.

inline C3::Header C3::Header::create( fitsfile* fits )
{
    int   size = 0;
    char* text = 0;
    int status = 0;
    fits_convert_hdr2str( fits, 0, 0, 0, &text, &size, &status );
    C3::FitsException::assert_status( status );
    return C3::Header( size, text );
}

// Constructor, copy.

inline C3::Header::Header( const C3::Header& header ) :
    _size(            header._size            ),
    _text( new char [ header._size * 80 + 1 ] )
{
    strcpy( _text, header._text );
}

// Destructor.

inline C3::Header::~Header()
{
    delete [] _text;
}

// Remove all records.

inline void C3::Header::clear()
{
    char* tmp = new char [ 0 ];
    delete [] _text;
    _size = 0;
    _text = tmp;
}

// Return keyword of the selected record.

inline std::string C3::Header::keyword( const size_t pos ) const
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );
    return keyword;

}

// Update keyword of selected record.

inline void C3::Header::keyword( const size_t pos, const std::string& update )
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );
    replace( pos,  update, value, comment );

}

// Get typed value.

template< typename T >
inline T C3::Header::get( const size_t pos ) const
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );

    T result;
    std::stringstream ss( value );
    ss >> result;

    return result;

}

template<>
inline std::string C3::Header::get( const size_t pos ) const
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );

    size_t begin = value.find_first_not_of( "' " );
    if( begin == std::string::npos ) return "";

    size_t end = value.find_last_not_of( "' " );
    size_t length = end - begin + 1;

    return value.substr( begin, length );

}

// Set typed value.

template< typename T >
inline void C3::Header::set( const size_t pos, const T typed_value )
{

    std::stringstream ss;
    ss << typed_value;

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword,    value, comment );
    replace( pos, keyword, ss.str(), comment );

}

template<>
inline void C3::Header::set( const size_t pos, const std::string typed_value ) 
{
    std::string tmp( typed_value );
    if( *(tmp.begin()) != '\'' && *(tmp.rbegin()) != '\'' ) tmp = "'" + tmp + "'";
    value( pos, tmp );
}


// Return value (string) of the selected record.

inline std::string C3::Header::value( const size_t pos ) const
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );
    return value;

}

// Update value (string) of selected record.

inline void C3::Header::value( const size_t pos, const std::string& update )
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword,  value, comment );
    replace( pos, keyword, update, comment );

}

// Return comment of the selected record.

inline std::string C3::Header::comment( const size_t pos ) const
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );
    return comment;

}

// Update comment of selected record.

inline void C3::Header::comment( const size_t pos, const std::string& update )
{

    std::string keyword;
    std::string value;
    std::string comment;
    extract( pos, keyword, value, comment );
    replace( pos, keyword, value,  update );

}

// Get typed value by keyword.

template< typename T > 
inline T C3::Header::get( const std::string& keyword, const size_t occurrence ) const
{
    return get< T >( find( keyword, occurrence ) );
}

// Set typed value by keyword.

template< typename T >
inline void C3::Header::set( const std::string& keyword, const T typed_value, const size_t occurrence )
{
    set< T >( find( keyword, occurrence ), typed_value );
}

// Return value by keyword as string.

inline std::string C3::Header::value( const std::string& keyword, const size_t occurrence ) const 
{
    return value( find( keyword, occurrence ) );
}

// Update value by keyword as string.

inline void C3::Header::value( const std::string& keyword, const std::string& update, const size_t occurrence )
{
    value( find( keyword, occurrence ), update );
}

// Return comment by keyword as string.

inline std::string C3::Header::comment( const std::string& keyword, const size_t occurrence ) const 
{
    return comment( find( keyword, occurrence ) );
}

// Update comment by keyword as string.

inline void C3::Header::comment( const std::string& keyword, const std::string& update, const size_t occurrence )
{
    comment( find( keyword, occurrence ), update );
}

// Return record index matching keyword.

inline size_t C3::Header::find( const std::string& keyword, const size_t occurrence ) const
{

    size_t count = 0;
    for( size_t pos = 0; pos < _size; ++ pos )
    {

        char card[ FLEN_CARD ];
        strncpy( card, _text + pos * 80, 80 );
        card[ 80 ] = '\0';

        char this_keyword[ FLEN_KEYWORD ];
        int keylength = 0;
        int status = 0;
        fits_get_keyname( card, this_keyword, &keylength, &status );
        C3::FitsException::assert_status( status );

        if( std::string( this_keyword ) != keyword ) continue;
        if( count == occurrence ) return pos;
        ++ count;

    }

    std::stringstream ss;
    ss << occurrence;
    throw C3::Exception( "no keyword/occurrence in header: " + keyword + "/" + ss.str() );

}

// Return record as keyword, value, comment strings.

inline void C3::Header::extract( const size_t pos, std::string& keyword, std::string& value, std::string& comment ) const
{

    char card[ FLEN_CARD ];
    strncpy( card, _text + pos * 80, 80 );
    card[ 80 ] = '\0';

    char this_keyword[ FLEN_KEYWORD ];
    int keylength = 0;
    int status = 0;
    fits_get_keyname( card, this_keyword, &keylength, &status );
    C3::FitsException::assert_status( status );

    char this_value[ FLEN_VALUE ]; 
    char this_comment[ FLEN_COMMENT ];
    fits_parse_value( card, this_value, this_comment, &status );
    C3::FitsException::assert_status( status );

    keyword = this_keyword;
    value   = this_value;
    comment = this_comment;

}

// Update a record at some position given keyword, value, comment strings.

inline void C3::Header::replace( const size_t pos, const std::string& keyword, const std::string& value, const std::string& comment )
{
    
    std::string str_card = keyword + " " + value + " /" + comment;
    char templt[ FLEN_CARD ];
    strcpy( templt, str_card.c_str() );

    char card[ FLEN_CARD ];
    int keytype = 0;
    int status = 0;
    fits_parse_template( templt, card, &keytype, &status );
    C3::FitsException::assert_status( status );

    memset( _text + pos * 80, ' ', 80 );
    strncpy( _text + pos * 80, card, strlen( card ) );

}

// Insert a record at some position given keyword, value, comment strings.

inline void C3::Header::insert( const size_t pos, const std::string& keyword, const std::string& value, const std::string& comment )
{

    char* tmp = new char [ ( _size + 1 ) * 80 + 1 ];

    // Copy first part.

    strncpy( tmp, _text, pos * 80 );

    // Insert the new card.

    std::string str_card = keyword + " " + value + " /" + comment;
    char templt[ FLEN_CARD ];
    strcpy( templt, str_card.c_str() );

    char card[ FLEN_CARD ];
    int keytype = 0;
    int status = 0;
    fits_parse_template( templt, card, &keytype, &status );
    C3::FitsException::assert_status( status );

    memset ( tmp + pos * 80,  ' ', 80 );
    strncpy( tmp + pos * 80, card, strlen( card ) );

    // Copy the rest.

    strncpy( tmp + ( pos + 1 ) * 80, _text + pos * 80, ( _size - pos ) * 80 );

    // Replace.

    delete [] _text;
    _size += 1;
    _text = tmp;

}

// Remove a record at some position.

inline void C3::Header::remove( const size_t pos )
{

    char* tmp = new char [ ( _size - 1 ) * 80 + 1 ];

    // Copy first part.

    strncpy( tmp, _text, pos * 80 );

    // Skip the record to delete and copy the rest.

    strncpy( tmp + pos * 80, _text + ( pos + 1 ) * 80, ( _size - pos - 1 ) * 80 );

    // Replace.

    delete [] _text;
    _size -= 1;
    _text = tmp;

}

// Assignment.

inline C3::Header& C3::Header::operator = ( const C3::Header& header )
{
    if( this != &header )
    {
        char* tmp = new char [ header._size * 80 + 1 ];
        strcpy( tmp, _text );
        delete [] _text;
        _size = header._size;
        _text = tmp;
    }
    return *this;
}

// Constructor.

inline C3::Header::Header( const size_t size, char* text ) :
    _size( size ),
    _text( text )
{}

// Stream output.

inline std::ostream& C3::operator << ( std::ostream& stream, const C3::Header& header )
{
    for( size_t pos = 0; pos < header.size(); ++ pos )
    {
        char card[ FLEN_CARD ];
        strncpy( card, header._text + pos * 80, 80 );
        card[ 80 ] = '\0';
        stream << card << std::endl;
    }
    return stream;
}
