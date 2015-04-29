#ifndef C3__MPI
#define C3__MPI

namespace C3
{

    namespace MPI
    {

        /// Raise exception if MPI status passed is not MPI_SUCCESS.
        void assert_status( const int status );

    }

}

#include "C3_MPI.inl.hh"

#endif
