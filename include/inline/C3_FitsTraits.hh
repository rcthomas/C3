
// FIXME verify these --- make sure that there is no special thing for BSCALE and BZERO though it would be easy to add for all of them.

template<>
struct C3::FitsType< unsigned char >
{
    static const int value = TBYTE;
};

template<>
struct C3::FitsType< signed char >
{
    static const int value = TSBYTE;
};

template<>
struct C3::FitsType< bool >
{
    static const int value = TLOGICAL;
};

template<>
struct C3::FitsType< unsigned short int >
{
    static const int value = TUSHORT;
};

template<>
struct C3::FitsType< signed short int >
{
    static const int value = TSHORT;
};

template<>
struct C3::FitsType< unsigned int >
{
    static const int value = TUINT;
};

template<>
struct C3::FitsType< signed int >
{
    static const int value = TINT;
};

template<>
struct C3::FitsType< unsigned long int >
{
    static const int value = TULONG;
};

template<>
struct C3::FitsType< signed long int >
{
    static const int value = TLONG;
};

template<>
struct C3::FitsType< float >
{
    static const int value = TFLOAT;
};

template<>
struct C3::FitsType< signed long long int >
{
    static const int value = TLONGLONG;
};

template<>
struct C3::FitsType< double >
{
    static const int value = TDOUBLE;
};
