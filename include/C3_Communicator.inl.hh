
// Constructor, wraps an MPI opaque communicator.

inline C3::Communicator::Communicator( MPI_Comm comm ) :
    _comm( comm )
{

    // Count of ranks in the communicator.

    int status = MPI_Comm_size( comm, &_size );
    C3::MPI::assert_status( status );

    // Rank of this process.

    status = MPI_Comm_rank( comm, &_rank );
    C3::MPI::assert_status( status );

    // Process is root if its rank is zero.

    _root = _rank == 0;

}

// Copy constructor.

inline C3::Communicator::Communicator( const C3::Communicator& comm ) :
        _comm( comm._comm ),
        _size( comm._size ),
        _rank( comm._rank ),
        _root( comm._root )
{}

// Destructor.

inline C3::Communicator::~Communicator()
{}

// Assignment.

inline C3::Communicator& C3::Communicator::operator = ( const C3::Communicator& comm )
{
    if( this != &comm )
    {
        _comm = comm._comm;
        _size = comm._size;
        _rank = comm._rank;
        _root = comm._root;
    }
    return *this;
}
