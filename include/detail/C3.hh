#ifndef C3_HH
#define C3_HH

#include <cstdlib>

namespace C3
{

    // Array subscript and container size type.

    using size_type = size_t;

    // Undefined type.

    struct UndefinedType {};

    // Container forward declarations.

    template< class T > class Block;
    template< class T > class OwnedBlock;
    template< class T > class Column;
    template< class T > class Row;
    template< class T > class Frame;
    template< class T > class Stack;
    template< class T > class View;

}

#endif
