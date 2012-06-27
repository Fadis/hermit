#ifndef HERMIT_HAS_TYPE_XXX_HPP
#define HERMIT_HAS_TYPE_XXX_HPP

#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>

#define FPP_HAS_TYPE_XXX( metafunc_name, name ) \
  namespace detail { \
    boost::mpl::bool_< false > metafunc_name BOOST_PP_LPAREN() ... BOOST_PP_RPAREN(); \
    template< typename T > \
    boost::mpl::bool_< true > metafunc_name BOOST_PP_LPAREN() T, typename T:: name * = 0 BOOST_PP_RPAREN(); \
  } \
template< typename T > \
struct metafunc_name { \
  typedef decltype BOOST_PP_LPAREN() detail:: metafunc_name BOOST_PP_LPAREN() T BOOST_PP_LPAREN() BOOST_PP_RPAREN()  BOOST_PP_RPAREN() BOOST_PP_RPAREN() type; \
  static const bool value = type::value; \
}

#endif

