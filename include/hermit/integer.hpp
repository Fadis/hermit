#ifndef HERMIT_INTEGER_HPP
#define HERMIT_INTEGER_HPP

#include <boost/integer.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>

#include <hermit/has_type_xxx.hpp>
#include <hermit/int128_t.hpp>
#include <hermit/mpint.hpp>

namespace hermit {
  namespace detail {
    FPP_HAS_TYPE_XXX( exact );

    template< unsigned int bits >
      struct uint_exact_is_available : public has_type_exact< boost::uint_t< bits > >::type {};

    template< unsigned int bits >
      struct int_exact_is_available : public has_type_exact< boost::int_t< bits > >::type {};

    template< unsigned int bits, bool is_available = uint_exact_is_available< bits >::value >
      struct uint_t_exact {};

    template< unsigned int bits >
      struct uint_t_exact< bits, true > {
        typedef typename boost::uint_t< bits >::exact exact;
      };

    template< unsigned int bits >
      struct uint_t_exact< bits, false > {};

    template< unsigned int bits, bool is_available = int_exact_is_available< bits >::value >
      struct int_t_exact {};

    template< unsigned int bits >
      struct int_t_exact< bits, true > {
        typedef typename boost::int_t< bits >::exact exact;
      };

    template< unsigned int bits >
      struct int_t_exact< bits, false > {};

  }

  template< unsigned int bits, bool le64 = ( bits <= 64 ), bool eq128 = ( bits == 128 ), bool le128 = ( bits <= 128 ) >
  struct uint_t {
    typedef mpint least;
    typedef mpint fast;
  };

  template< unsigned int bits >
    struct uint_t< bits, true, false, true > : public detail::uint_t_exact< bits > {
      typedef typename boost::uint_t< bits >::least least;
      typedef typename boost::uint_t< bits >::fast fast;
    };

#ifdef HAVE_INT128_T
  template< unsigned int bits >
    struct uint_t< bits, false, false, true > {
      typedef uint128_t least;
      typedef uint128_t fast;
    };
  
  template< unsigned int bits >
    struct uint_t< bits, false, true, true > {
      typedef uint128_t exact;
      typedef uint128_t least;
      typedef uint128_t fast;
    };
#endif

  template< unsigned int bits, bool le64 = ( bits <= 64 ), bool eq128 = ( bits == 128 ), bool le128 = ( bits <= 128 ) >
  struct int_t {
    typedef mpint least;
    typedef mpint fast;
  };

  template< unsigned int bits >
    struct int_t< bits, true, false, true > : public detail::int_t_exact< bits > { 
      typedef typename boost::int_t< bits >::least least;
      typedef typename boost::int_t< bits >::fast fast;
    };

#ifdef HAVE_INT128_T
  template< unsigned int bits >
    struct int_t< bits, false, false, true > {
      typedef int128_t least;
      typedef int128_t fast;
    };
  
  template< unsigned int bits >
    struct int_t< bits, false, true, true > {
      typedef int128_t exact;
      typedef int128_t least;
      typedef int128_t fast;
    };
#endif

}

#endif

