#include <iostream>
#include <memory>
#include <boost/integer.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>

#include "mpint.hpp"

#define FPP_HAS_TYPE_XXX( name ) \
  namespace detail { \
    boost::mpl::bool_< false > BOOST_PP_CAT( detect_, name ) BOOST_PP_LPAREN() ... BOOST_PP_RPAREN(); \
    template< typename T > \
    boost::mpl::bool_< true > BOOST_PP_CAT( detect_, name ) BOOST_PP_LPAREN() T, typename T:: name * = 0 BOOST_PP_RPAREN(); \
  } \
template< typename T > \
struct BOOST_PP_CAT( has_type_, name ) { \
  typedef decltype BOOST_PP_LPAREN() detail:: BOOST_PP_CAT( detect_, name ) BOOST_PP_LPAREN() T BOOST_PP_LPAREN() BOOST_PP_RPAREN()  BOOST_PP_RPAREN() BOOST_PP_RPAREN() type; \
  static const bool value = type::value; \
}
namespace fadis {
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

  template< unsigned int bits, bool le64 = ( bits <= 64 ), bool le128 = ( bits <= 128 ) >
  struct uint_t {
    typedef mpint least;
    typedef mpint fast;
  };

  template< unsigned int bits >
    struct uint_t< bits, true, true > : public detail::uint_t_exact< bits > {
      typedef typename boost::uint_t< bits >::least least;
      typedef typename boost::uint_t< bits >::fast fast;
    };

/*
  template< unsigned int bits >
    struct uint_t< bits, false, true > : public detail::uint_t_exact< bits >, public detail::uint_t_least< bits >, public detail::uint_t_fast< bits > {};
*/


  template< unsigned int bits, bool le64 = ( bits <= 64 ), bool le128 = ( bits <= 128 ) >
  struct int_t {
    typedef mpint least;
    typedef mpint fast;
  };

  template< unsigned int bits >
    struct int_t< bits, true, true > : public detail::int_t_exact< bits > { 
      typedef typename boost::int_t< bits >::least least;
      typedef typename boost::int_t< bits >::fast fast;
    };

/*
  template< unsigned int bits >
    struct uint_t< bits, false, true > : public detail::uint_t_exact< bits >, public detail::uint_t_least< bits >, public detail::uint_t_fast< bits > {};
*/

}

int main() {
  fadis::int_t< 64 >::fast foo;
  fadis::int_t< 128 >::least bar;
}

