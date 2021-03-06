#ifndef HERMIT_HAS_MEMBER_TYPE_XXX_HPP
#define HERMIT_HAS_MEMBER_TYPE_XXX_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_DECLTYPE
#else
#include <utility>

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#include <boost/type_traits.hpp>
#else
#include <type_traits>
#endif

#define HPP_HAS_MEMBER_TYPE_XXX_INTERNAL( metafunc_name, target_name, library ) \
  namespace detail { \
    template< \
      typename T \
    > \
    library :: true_type metafunc_name ( T&&, typename T :: target_name * = 0 ); \
    library :: false_type metafunc_name ( ... ); \
  } \
  template< \
    typename T, \
    typename Result = decltype ( detail:: metafunc_name ( std::declval< T >() ) ) \
  > \
  struct metafunc_name : public Result {};

#ifdef BOOST_NO_0X_HDR_TYPE_TRAITS
#define HPP_HAS_MEMBER_TYPE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_TYPE_XXX_INTERNAL( metafunc_name, target_name, boost )
#else
#define HPP_HAS_MEMBER_TYPE_XXX( metafunc_name, target_name ) \
  HPP_HAS_MEMBER_TYPE_XXX_INTERNAL( metafunc_name, target_name, std )
#endif

#endif

#endif

