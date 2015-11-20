
#include <mpi.h>

template<>
struct C3::MpiType< char >
{
    static const MPI_Datatype datatype = MPI_CHAR;
};

template<>
struct C3::MpiType< unsigned char >
{
    static const MPI_Datatype datatype = MPI_UNSIGNED_CHAR;
};

template<>
struct C3::MpiType< signed char >
{
    static const MPI_Datatype datatype = MPI_SIGNED_CHAR;
};

//template<>
//struct C3::MpiType< bool >
//{
//    static const MPI_Datatype datatype = MPI_C_BOOL;      /// BOOL?
//};

template<>
struct C3::MpiType< unsigned short int >
{
    static const MPI_Datatype datatype = MPI_UNSIGNED_SHORT;
};

template<>
struct C3::MpiType< signed short int >
{
    static const MPI_Datatype datatype = MPI_SHORT;
};

template<>
struct C3::MpiType< unsigned int >
{
    static const MPI_Datatype datatype = MPI_UNSIGNED;
};

template<>
struct C3::MpiType< signed int >
{
    static const MPI_Datatype datatype = MPI_INT;
};

template<>
struct C3::MpiType< unsigned long int >
{
    static const MPI_Datatype datatype = MPI_UNSIGNED_LONG;
};

template<>
struct C3::MpiType< signed long int >
{
    static const MPI_Datatype datatype = MPI_LONG;
};

template<>
struct C3::MpiType< float >
{
    static const MPI_Datatype datatype = MPI_FLOAT;
};

template<>
struct C3::MpiType< signed long long int >
{
    static const MPI_Datatype datatype = MPI_LONG_LONG_INT;
};

template<>
struct C3::MpiType< double >
{
    static const MPI_Datatype datatype = MPI_DOUBLE;
};
