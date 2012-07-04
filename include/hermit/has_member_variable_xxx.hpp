#ifndef HERMIT_HAS_MEMBER_VARIABLE_XXX_HPP
#define HERMIT_HAS_MEMBER_VARIABLE_XXX_HPP

#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/components.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>

#define HPP_HAS_MEMBER_VARIABLE_XXX( metafunc_name, name ) \
  namespace detail { \
    namespace detail { \
      namespace detail { \
        namespace detail { \
          template< typename ExpectedType, typename T > \
          auto metafunc_name BOOST_PP_LPAREN() T* t ) -> decltype( \
            t -> name , void(), boost::is_same< ExpectedType, decltype( t -> name ) >() \
          ); \
          template< typename ExpectedType > \
          boost::mpl::bool_< false > metafunc_name BOOST_PP_LPAREN() ... ); \
        } \
        template< typename T, typename ExpectedType, typename Result = \
          decltype( \
            detail:: metafunc_name \
            < ExpectedType > \
            BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
        > \
        struct metafunc_name : public boost::mpl::bool_< Result::value > {}; \
      } \
      template< typename ExpectedType, typename T > \
      detail:: metafunc_name < T, ExpectedType > metafunc_name BOOST_PP_LPAREN() T* t, \
        typename boost::enable_if< typename boost::is_class< T >::type >::type* = 0 ); \
      template< typename ExpectedType > \
      boost::mpl::bool_< false > metafunc_name BOOST_PP_LPAREN() ... ); \
    } \
    template< typename T, typename ExpectedType, typename Result = \
      decltype( \
        detail:: metafunc_name \
        < ExpectedType > \
        BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) \
      ) \
    > \
    struct metafunc_name : public boost::mpl::bool_< Result::value > {}; \
  } \
  template< typename T, typename ExpectedType > \
  struct metafunc_name : \
    public boost::mpl::bool_< \
      detail:: metafunc_name < T, ExpectedType >::value \
    > { \
  }; \

#endif

