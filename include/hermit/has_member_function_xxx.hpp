#ifndef HERMIT_HAS_MEMBER_FUNCTION_XXX_HPP
#define HERMIT_HAS_MEMBER_FUNCTION_XXX_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_DECLTYPE
#else
#include <utility>

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#include <boost/type_traits.hpp>
#else
#include <type_traits>
#endif

#if BOOST_NO_VARIADIC_TEMPLATES 

#include <boost/preprocessor.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/at.hpp>

#define HPP_HAS_MEMBER_FUNCTION_XXX_MAX_ARGUMENTS_COUNT 20

#define HPP_HAS_MEMBER_FUNCTION_XXX_ARGS( z, index, prefix ) \
  prefix BOOST_PP_CAT( Arg, index )
#define HPP_HAS_MEMBER_FUNCTION_XXX_DECLVALS( z, index, unused ) \
  std::declval< BOOST_PP_CAT( Arg, index ) >()
#define HPP_HAS_MEMBER_FUNCTION_XXX_DETECTORS( z, index, args ) \
  template< \
    typename T, \
    typename ReturnType BOOST_PP_COMMA_IF( BOOST_PP_NOT_EQUAL( index, 0 ) ) \
    BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_ARGS, typename ) \
  > \
  class BOOST_PP_TUPLE_ELEM( 4, 0, args ) < T, ReturnType ( BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_ARGS, ) ) > { \
  private: \
    template< typename U > \
    static typename BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: BOOST_PP_TUPLE_ELEM( 4, 2, args )  < \
      decltype( \
        std::declval< typename BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: remove_const< U >::type >() . BOOST_PP_TUPLE_ELEM( 4, 1, args )  ( BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_DECLVALS, ) ) \
      ), \
      ReturnType \
    >::type check( U&& ); \
    static BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: false_type check( ... ); \
  public: \
    typedef decltype( check( std::declval< T >() ) ) type; \
    static constexpr bool value = type::value; \
  }; \
  template< \
    typename T, \
    typename ReturnType BOOST_PP_COMMA_IF( BOOST_PP_NOT_EQUAL( index, 0 ) ) \
    BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_ARGS, typename ) \
  > \
  class BOOST_PP_TUPLE_ELEM( 4, 0, args )  < T, ReturnType ( BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_ARGS, ) ) const > { \
  private: \
    template< typename U > \
    static typename BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: BOOST_PP_TUPLE_ELEM( 4, 2, args )  < \
      decltype( \
        std::declval< typename BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: add_const< U >::type >() . BOOST_PP_TUPLE_ELEM( 4, 1, args )  ( BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_DECLVALS, ) ) \
      ), \
      ReturnType \
    >::type check( U&& ); \
    static BOOST_PP_TUPLE_ELEM( 4, 3, args )  :: false_type check( ... ); \
  public: \
    typedef decltype( check( std::declval< T >() ) ) type; \
    static constexpr bool value = type::value; \
  };

#define HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  template< \
    typename T, \
    typename ExpectedType \
  > \
  class metafunc_name : public library :: false_type {}; \
  BOOST_PP_REPEAT( HPP_HAS_MEMBER_FUNCTION_XXX_MAX_ARGUMENTS_COUNT, HPP_HAS_MEMBER_FUNCTION_XXX_DETECTORS, ( metafunc_name, target_name, judge, library ) )


#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#define HPP_HAS_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, boost )
#define HPP_HAS_CONVERTIBLE_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, boost )
#else
#define HPP_HAS_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, std )
#define HPP_HAS_CONVERTIBLE_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, std )
#endif

#else

#define HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  template< \
    typename T, \
    typename ExpectedType \
  > \
  class metafunc_name : public library :: false_type {}; \
  template< \
    typename T, \
    typename ReturnType, \
    typename ...Args \
  > \
  class metafunc_name < T, ReturnType ( Args... ) > { \
  private: \
    template< typename U > \
    static typename library :: judge < \
      decltype( \
        std::declval< typename library :: remove_const< U >::type >() . target_name ( std::declval< Args >()... ) \
      ), \
      ReturnType \
    >::type check( U&& ); \
    static library :: false_type check( ... ); \
  public: \
    typedef decltype( check( std::declval< T >() ) ) type; \
    static constexpr bool value = type::value; \
  }; \
  template< \
    typename T, \
    typename ReturnType, \
    typename ...Args \
  > \
  class metafunc_name < T, ReturnType ( Args... ) const > { \
  private: \
    template< typename U > \
    static typename library :: judge < \
      decltype( \
        std::declval< typename library :: add_const< U >::type >() . target_name ( std::declval< Args >()... ) \
      ), \
      ReturnType \
    >::type check( U&& ); \
    static library :: false_type check( ... ); \
  public: \
    typedef decltype( check( std::declval< T >() ) ) type; \
    static constexpr bool value = type::value; \
  };

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#define HPP_HAS_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, boost )
#define HPP_HAS_CONVERTIBLE_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, boost )
#else
#define HPP_HAS_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_same, std )
#define HPP_HAS_CONVERTIBLE_MEMBER_FUNCTION_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, is_convertible, std )
#endif

#endif

#endif

#endif

