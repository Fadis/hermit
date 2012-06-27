#ifndef HERMIT_HAS_MEMFUN_XXX_HPP
#define HERMIT_HAS_MEMFUN_XXX_HPP

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

#include <hermit/to_member_function.hpp>

#define HPP_MEMFUN_XXX( name ) \
  namespace detail { \
    namespace detail { \
      namespace detail { \
        namespace detail { \
          namespace detail { \
            template< typename Func, typename T > \
            auto BOOST_PP_CAT( detect_memfun_, name ) BOOST_PP_LPAREN() T* t ) -> decltype( \
              static_cast< Func >( &T:: name ), void(), boost::mpl::bool_< true >() \
            ); \
            template< typename Func > \
            boost::mpl::bool_< false > BOOST_PP_CAT( detect_memfun_, name ) BOOST_PP_LPAREN() ... ); \
          } \
          template< typename T, typename Func, typename Result = \
            decltype( \
              detail:: BOOST_PP_CAT( detect_memfun_, name ) \
              < typename to_member_function< Func, T >::type > \
              BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
          > \
          struct BOOST_PP_CAT( detect_memfun_, name ) : public boost::mpl::bool_< Result::value > {}; \
        } \
        template< typename T, typename Func > \
        struct BOOST_PP_CAT( detect_memfun_, name ) : \
          public boost::mpl::bool_< \
            detail:: BOOST_PP_CAT( detect_memfun_, name ) < T, Func >::value \
          > { \
        }; \
      } \
      template< typename Func, typename T > \
      BOOST_PP_CAT( detail::detect_memfun_, name ) < T, Func > BOOST_PP_CAT( detect_memfun_, name ) BOOST_PP_LPAREN() T* t, typename boost::disable_if< typename boost::is_pod< T >::type >::type* = 0 ); \
      template< typename Func > \
      boost::mpl::bool_< false > BOOST_PP_CAT( detect_memfun_, name ) BOOST_PP_LPAREN() ... ); \
    } \
    template< typename T, typename Func, typename Result = \
      decltype( \
        detail:: BOOST_PP_CAT( detect_memfun_, name ) \
          < Func > \
          BOOST_PP_LPAREN() static_cast< T* >( nullptr ) ) ) \
        > \
    struct BOOST_PP_CAT( detect_memfun_, name ) : public boost::mpl::bool_< Result::value > {}; \
  } \
  template< typename T, typename Func > \
  struct BOOST_PP_CAT( detect_memfun_, name ) : \
    public boost::mpl::bool_< \
      detail:: BOOST_PP_CAT( detect_memfun_, name ) < T, Func >::value \
    > { \
  }; \

#endif

