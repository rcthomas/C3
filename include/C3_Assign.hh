#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

namespace C3
{

    /// Assign source to destination, optimized by argument types.
    template< class Destination, class Source > 
    Destination& assign( Destination& dest, const Source& src );

    namespace Detail
    {

        /// Handles assignment between various types.
        template< class Destination, class Source, bool SourceIsCompatibleScalar = false > 
        struct Assign;

        /// Assign repeated copies of scalar source to destination.
        template< class Destination >
        Destination& fill_assign( Destination& dest, const typename Destination::value_type src );

        /// Assign repeated copies of source entries to destination.
        template< class Destination, class Source > 
        Destination& fill_assign( Destination& dest, const Source& src, const size_type count );

        /// Assign repeated copies of source to destination.
        template< class Destination, class Source > 
        Destination& copy_assign( Destination& dest, const Source& src, const size_type count );

    }

}

#include "C3_Assign.inl.hh"

#endif
