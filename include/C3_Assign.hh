#ifndef C3_ASSIGN_HH
#define C3_ASSIGN_HH

#include "C3.hh"
#include "C3_TypeTraits.hh"

namespace C3
{

    /// Assigns a source container (or pixel value) to destination.
    template< class Destination, class Source > Destination& assign( Destination& dest, const Source& source );

    // Details, details.

    namespace Detail
    {

        // Tag-dispatch details.

        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, FillAssignment        );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, CopyAssignment        );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, ColumnFrameAssignment );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, ColumnStackAssignment );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, RowFrameAssignment    );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, RowStackAssignment    );
        template< class Destination, class Source > Destination& assign( Destination& dest, const Source& src, FrameStackAssignment  );

        // Implementation details.

        namespace Kernel
        {
            template< class Destination, class Source > Destination& fill_assign( Destination& dest, const Source& src );
            template< class Destination, class Source > Destination& fill_assign( Destination& dest, const Source& src, const size_type count );
            template< class Destination, class Source > Destination& copy_assign( Destination& dest, const Source& src, const size_type count );
        }

    }

}

#include "C3_Assign.inl.hh"

#endif
