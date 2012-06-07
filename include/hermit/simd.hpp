#ifndef HERMIT_SIMD_HPP
#define HERMIT_SIMD_HPP

#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE

#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/shift_left.hpp>

namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, typename U = boost::mpl::int_< 1 > >
        struct smallest_pot
        : public boost::mpl::if_<
          boost::mpl::bool_< T::value <= U::value >,
          U,
          smallest_pot< T, boost::mpl::shift_left< U, boost::mpl::int_< 1 > > >
          >::type {};

      template< typename T, unsigned int size >
        class vector {
          typedef T type __attribute( ( vector_size( smallest_pot< boost::mpl::int_< size > >::value * sizeof( T ) ) ) );
        };
    }
  }
}

# endif
#endif

#ifdef __clang__
# if __clang_major__ >= 3

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE

namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, unsigned int size >
        class vector {
          public:
            typedef T type __attribute__( ( ext_vector_type( size ) ) );
        };
    }
  }
}

# endif
#endif

#ifndef HERMIT_VECTOR_TYPE_IS_AVAILABLE

#include <boost/array.hpp>

namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, unsigned int size >
        class vector {
          public:
            typedef boost::array< T, size > raw_type;
        };
    }
  }
}

#endif

#endif
