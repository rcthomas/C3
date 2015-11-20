#ifndef C3_TYPE_TRAITS_HH
#define C3_TYPE_TRAITS_HH

#include <type_traits>

#include "C3.hh"

namespace C3
{

    template< class T > struct IsPixel 
    { 
        static const bool value = std::is_arithmetic< T >::value; 
    };

    template< class T > struct ValueType 
    {
        using type = typename std::conditional< IsPixel< T >::value, T, UndefinedType >::type;
    };

    template< class T > struct ValueType< Block < T > > { using type = T; };
    template< class T > struct ValueType< Column< T > > { using type = T; };
    template< class T > struct ValueType< Row   < T > > { using type = T; };
    template< class T > struct ValueType< Frame < T > > { using type = T; };
    template< class T > struct ValueType< View  < T > > { using type = T; };
    template< class T > struct ValueType< Stack < T > > { using type = T; };

    template< class T > struct IsBlock                 { static const bool value = false; };
    template< class T > struct IsBlock< Block< T > >   { static const bool value =  true; };

    template< class T > struct IsColumn                 { static const bool value = false; };
    template< class T > struct IsColumn< Column< T > >  { static const bool value =  true; };

    template< class T > struct IsRow                    { static const bool value = false; };
    template< class T > struct IsRow< Row< T > >        { static const bool value =  true; };

    template< class T > struct IsFrame                  { static const bool value = false; };
    template< class T > struct IsFrame< Frame< T > >    { static const bool value =  true; };

    template< class T > struct IsStack                  { static const bool value = false; };
    template< class T > struct IsStack< Stack< T > >    { static const bool value =  true; };

    template< class T > struct IsView                   { static const bool value = false; };
    template< class T > struct IsView< View< T > >      { static const bool value =  true; };

    template< class T > struct IsFrameOrView                { static const bool value = false; };
    template< class T > struct IsFrameOrView< Frame< T > >  { static const bool value =  true; };
    template< class T > struct IsFrameOrView<  View< T > >  { static const bool value =  true; };

    template< class T > struct IsContainer
    {
        static const bool value = IsBlock< T >::value
            || IsColumn< T >::value
            || IsRow< T >::value
            || IsFrame< T >::value
            || IsStack< T >::value
            || IsView< T >::value;
    };

    template< class Head, class Tail >
    struct ContainerPairTraits
    {

        static const bool head_is_pixel     = IsPixel< Head >::value;
        static const bool head_is_container = IsContainer< Head >::value;

        static const bool tail_is_pixel     = IsPixel< Tail >::value;
        static const bool tail_is_container = IsContainer< Tail >::value;

        static const bool head_pixel_tail_container = head_is_pixel     && tail_is_container;
        static const bool head_container_tail_pixel = head_is_container && tail_is_pixel;

        static const bool container_and_pixel = head_pixel_tail_container || head_container_tail_pixel;

    };

}

#endif
