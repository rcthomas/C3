
#include "../C3_MpiException.hh"

// Constructor, wraps an MPI opaque communicator.

inline C3::Communicator::Communicator( MPI_Comm comm ) :
    _comm( comm )
{

    // Count of ranks in the communicator.

    int status = MPI_Comm_size( comm, &_size );
    C3::assert_mpi_status( status );

    // Rank of this process.

    status = MPI_Comm_rank( comm, &_rank );
    C3::assert_mpi_status( status );

    // Process is root if its rank is zero.

    _root = _rank == 0;

}
