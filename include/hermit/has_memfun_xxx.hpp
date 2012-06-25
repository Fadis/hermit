#ifndef HERMIT_HAS_MEMFUN_XXX_HPP
#define HERMIT_HAS_MEMFUN_XXX_HPP

#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/components.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>

#include <hermit/to_member_function.hpp>

#define HPP_MEMFUN_XXX( name ) \
  namespace detail { \
    template< typename Func, typename T > \
    auto BOOST_PP_CAT( detect_, name ) BOOST_PP_LPAREN() T* t ) -> decltype( \
      static_cast< Func >( &T:: name ), boost::mpl::bool_< true >() \
    ); \
    template< typename Func > \
    boost::mpl::bool_< false > BOOST_PP_CAT( detect_, name ) BOOST_PP_LPAREN() ... ); \
  } \
  template< typename T, typename Func, typename Result = \
    decltype( \
      detail:: BOOST_PP_CAT( detect_, name ) \
      < typename to_member_function< Func, T >::type > \
      BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
> \
  struct BOOST_PP_CAT( detect_, name ) : public boost::mpl::bool_< Result::value > {};

#endif

