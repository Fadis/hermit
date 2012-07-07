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

#define HPP_HAS_MEMBER_FUNCTION_XXX_DETECTOR_ARGUMENT( z, index, unused ) \
  std::declval< typename boost::mpl::at_c< Parameters, index >::type >()

#define HPP_HAS_MEMBER_FUNCTION_XXX_DETECTOR( z, index, args ) \
  template< \
    typename T, \
    typename ExpectedType, \
    typename Parameters, \
    typename ReturnType \
  > \
  struct BOOST_PP_TUPLE_ELEM( 4, 0, args ) < T, ExpectedType, Parameters, ReturnType, boost::mpl::long_< index > > { \
    template< \
      typename U, \
      typename Result = decltype( \
        std::declval< U >() . BOOST_PP_TUPLE_ELEM( 4, 1, args ) ( \
          BOOST_PP_ENUM( index, HPP_HAS_MEMBER_FUNCTION_XXX_DETECTOR_ARGUMENT,  ) \
        ) \
      ) \
    > \
    static typename BOOST_PP_TUPLE_ELEM( 4, 3, args ) :: BOOST_PP_TUPLE_ELEM( 4, 2, args ) < Result, ReturnType >::type check( U&& ); \
    static typename BOOST_PP_TUPLE_ELEM( 4, 3, args ) :: false_type check( ... ); \
    typedef decltype( check( std::declval< T >() ) ) type; \
    static constexpr bool value = type::value; \
  };

#define HPP_HAS_MEMBER_FUNCTION_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  namespace detail { \
    namespace detail { \
      template< \
        typename T, \
        typename ExpectedType, \
        typename Parameters, \
        typename ReturnType, \
        typename ArgumentCount \
      > \
      struct metafunc_name : public library :: false_type {}; \
      BOOST_PP_REPEAT( \
        HPP_HAS_MEMBER_FUNCTION_XXX_MAX_ARGUMENTS_COUNT, \
        HPP_HAS_MEMBER_FUNCTION_XXX_DETECTOR, \
        ( metafunc_name, target_name, judge, library ) \
      ) \
    } \
    template< \
      typename ExpectedType, \
      typename T, \
      typename Parameters = typename boost::function_types::parameter_types< ExpectedType >::type, \
      typename ReturnType = typename boost::function_types::result_type< ExpectedType >::type, \
      typename ArgumentCount = typename boost::mpl::size< Parameters >::type \
    > \
    typename detail :: metafunc_name < \
      T, ExpectedType, Parameters, ReturnType, ArgumentCount \
    >::type metafunc_name ( T&& ); \
    template< typename ExpectedType > \
    library :: false_type metafunc_name ( ... ); \
  } \
  template< \
    typename T, \
    typename ExpectedType, \
    typename Result = decltype( detail :: metafunc_name< ExpectedType > ( std::declval< T >() ) ) \
  > struct metafunc_name : public Result {};

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
        std::declval< U >() . target_name ( std::declval< Args >()... ) \
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

