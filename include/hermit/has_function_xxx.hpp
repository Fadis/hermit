#ifndef HERMIT_HAS_FUNCTION_XXX_HPP
#define HERMIT_HAS_FUNCTION_XXX_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_DECLTYPE
#else

#include <hermit/none_type.hpp>

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#include <boost/type_traits.hpp>
#else
#include <type_traits>
#endif

#if BOOST_NO_VARIADIC_TEMPLATES
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/tuple.hpp>

#define HPP_HAS_FUNCTION_XXX_MAX_ARGUMENTS_COUNT 20
#define HPP_HAS_FUNCTION_XXX_DETECTOR_ARGUMENTS( z, index, prefix ) \
  BOOST_PP_CAT( prefix, index )
#define HPP_HAS_FUNCTION_XXX_DETECTOR_DECLVAL( z, index, prefix ) \
  std::declval< BOOST_PP_CAT( prefix, index ) >()
#define HPP_HAS_FUNCTION_XXX_DETECTOR( z, index, args ) \
  template< typename ReturnType BOOST_PP_COMMA_IF( BOOST_PP_NOT_EQUAL( index, 0 ) ) BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_ARGUMENTS, typename Arg ) > \
  struct BOOST_PP_TUPLE_ELEM( 4, 0, args ) < ReturnType ( BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_ARGUMENTS, Arg ) ) > { \
    template< BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_ARGUMENTS, typename Arg_ ) > \
    static decltype( BOOST_PP_TUPLE_ELEM( 4, 1, args ) ( BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_DECLVAL, Arg_ ) ) ) \
      check( BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_ARGUMENTS, Arg_ ) ); \
    typedef typename BOOST_PP_TUPLE_ELEM( 4, 3, args ) :: BOOST_PP_TUPLE_ELEM( 4, 2, args ) < \
      decltype( check( BOOST_PP_ENUM( index, HPP_HAS_FUNCTION_XXX_DETECTOR_DECLVAL, Arg ) ) ), \
      ReturnType \
    >::type type; \
    static constexpr bool value = type::value; \
  };
#define HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  namespace detail { \
    hermit::none_type target_name (...); \
    template< typename T > \
    struct metafunc_name : public std::false_type {}; \
    BOOST_PP_REPEAT_FROM_TO( 1, HPP_HAS_FUNCTION_XXX_MAX_ARGUMENTS_COUNT, HPP_HAS_FUNCTION_XXX_DETECTOR, ( metafunc_name, target_name, judge, library ) ) \
  } \
  template < typename T > \
  struct metafunc_name : detail:: metafunc_name < T > {};
#else
#define HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  namespace detail { \
    hermit::none_type target_name (...); \
    template< typename T > \
    struct metafunc_name : public std::false_type {}; \
    template< typename ReturnType, typename ...Args > \
    struct metafunc_name < ReturnType ( Args... ) > { \
      template< typename ...Args_ > \
      static auto check( Args_... ) -> decltype( target_name ( std::declval< Args >()... ) ); \
      typedef typename library :: judge< \
        decltype( check< Args... >( std::declval< Args >()... ) ), \
        ReturnType \
      >::type type; \
      static constexpr bool value = type::value; \
    }; \
  } \
  template < typename T > \
  struct metafunc_name : detail:: metafunc_name < T > {};
#endif

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#define HPP_HAS_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, boost )
#define HPP_HAS_CONVERTIBLE_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, boost )
#else
#define HPP_HAS_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, std )
#define HPP_HAS_CONVERTIBLE_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, std )
#endif

#endif

#endif

