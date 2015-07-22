
// FIXME verify these --- make sure that there is no special thing for BSCALE and BZERO though it would be easy to add for all of them.

template<>
struct C3::FitsType< unsigned char >
{
    static const int datatype = TBYTE;
    static const int bitpix   = BYTE_IMG;
};

template<>
struct C3::FitsType< signed char >
{
    static const int datatype = TSBYTE;
    static const int bitpix   = SBYTE_IMG;
};

template<>
struct C3::FitsType< bool >
{
    static const int datatype = TLOGICAL;
    static const int bitpix   = BYTE_IMG;   // VERIFY
};

template<>
struct C3::FitsType< unsigned short int >
{
    static const int datatype = TUSHORT;
    static const int bitpix   = USHORT_IMG;
};

template<>
struct C3::FitsType< signed short int >
{
    static const int datatype = TSHORT;
    static const int bitpix   = SHORT_IMG;
};

template<>
struct C3::FitsType< unsigned int >
{
    static const int datatype = TUINT;
    static const int bitpix   = ULONG_IMG;
};

template<>
struct C3::FitsType< signed int >
{
    static const int datatype = TINT;
    static const int bitpix   = LONG_IMG;
};

template<>
struct C3::FitsType< unsigned long int >
{
    static const int datatype = TULONG;
    static const int bitpix   = ULONG_IMG;
};

template<>
struct C3::FitsType< signed long int >
{
    static const int datatype = TLONG;
    static const int bitpix   = LONG_IMG;
};

template<>
struct C3::FitsType< float >
{
    static const int datatype = TFLOAT;
    static const int bitpix   = FLOAT_IMG;
};

template<>
struct C3::FitsType< signed long long int >
{
    static const int datatype = TLONGLONG;
    static const int bitpix   = LONGLONG_IMG;
};

template<>
struct C3::FitsType< double >
{
    static const int datatype = TDOUBLE;
    static const int bitpix   = DOUBLE_IMG;
};
