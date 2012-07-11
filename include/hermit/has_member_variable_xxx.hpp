#ifndef HERMIT_HAS_MEMBER_VARIABLE_XXX_HPP
#define HERMIT_HAS_MEMBER_VARIABLE_XXX_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_DECLTYPE
#else
#include <utility>

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#include <boost/type_traits.hpp>
#else
#include <type_traits.hpp>
#endif

#include <hermit/none_type.hpp>

namespace hermit {
  template< typename T >
  struct member_pointer_to_value_type {
    template< typename U, typename V >
    static V hoge( V U ::* );
    static hermit::none_type hoge( ... );
    typedef decltype( hoge( std::declval< T >() ) ) type;
  };
}

#define HPP_HAS_MEMBER_VARIABLE_XXX_INTERNAL( metafunc_name, target_name, judge, library ) \
  namespace detail { \
    template< \
      typename ExpectedType, \
      typename T, \
      typename Result = decltype( &T :: target_name ) > \
    library :: judge < ExpectedType, typename hermit::member_pointer_to_value_type< Result >::type > metafunc_name ( T&& ); \
    template< \
      typename ExpectedType \
    > \
    library :: false_type metafunc_name ( ... ); \
  } \
  template< \
    typename T, \
    typename ExpectedType, \
    typename Result = decltype( detail :: metafunc_name< ExpectedType > ( std::declval< T >() ) ) \
  > struct metafunc_name : public Result {};

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#define HPP_HAS_MEMBER_VARIABLE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_VARIABLE_XXX_INTERNAL( metafunc_name, target_name, is_same, boost )
#define HPP_HAS_CONVERTIBLE_MEMBER_VARIABLE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_VARIABLE_XXX_INTERNAL( metafunc_name, target_name, is_convertible, boost )
#else
#define HPP_HAS_MEMBER_VARIABLE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_VARIABLE_XXX_INTERNAL( metafunc_name, target_name, is_same, std )
#define HPP_HAS_CONVERTIBLE_MEMBER_VARIABLE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_VARIABLE_XXX_INTERNAL( metafunc_name, target_name, is_convertible, std )
#endif

#endif

#endif

