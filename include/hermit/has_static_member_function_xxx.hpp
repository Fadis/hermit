#ifndef HERMIT_HAS_STATIC_MEMBER_FUNCTION_XXX_HPP
#define HERMIT_HAS_STATIC_MEMBER_FUNCTION_XXX_HPP

#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/components.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>

#define HPP_HAS_STATIC_MEMBER_FUNCTION_XXX( metafunc_name, name ) \
  namespace detail { \
    namespace detail { \
      namespace detail { \
        namespace detail { \
          namespace detail { \
            template< typename Func, typename T > \
            auto metafunc_name BOOST_PP_LPAREN() T* t ) -> decltype( \
              boost::is_same< decltype( T:: name ), Func >() \
            ); \
            template< typename Func > \
            boost::mpl::bool_< false > metafunc_name BOOST_PP_LPAREN() ... ); \
          } \
          template< typename T, typename Func, typename Result = \
            decltype( \
              detail:: metafunc_name \
              < Func > \
              BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
          > \
          struct metafunc_name : public boost::mpl::bool_< Result::value > {}; \
        } \
        template< typename T, typename Func > \
        struct metafunc_name : \
          public boost::mpl::bool_< \
            detail:: metafunc_name < T, Func >::value \
          > { \
        }; \
      } \
      template< typename Func, typename T > \
      detail:: metafunc_name < T, Func > metafunc_name BOOST_PP_LPAREN() T* t, typename boost::disable_if< typename boost::is_pod< T >::type >::type* = 0 ); \
      template< typename Func > \
      boost::mpl::bool_< false > metafunc_name BOOST_PP_LPAREN() ... ); \
    } \
    template< typename T, typename Func, typename Result = \
      decltype( \
        detail:: metafunc_name \
          < Func > \
          BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
        > \
    struct metafunc_name : public boost::mpl::bool_< Result::value > {}; \
  } \
  template< typename T, typename Func > \
  struct metafunc_name : \
    public boost::mpl::bool_< \
      detail:: metafunc_name < T, Func >::value \
    > { \
  }; \

#endif

