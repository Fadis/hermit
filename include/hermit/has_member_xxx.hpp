#ifndef HERMIT_HAS_MEMBER_XXX_HPP
#define HERMIT_HAS_MEMBER_XXX_HPP

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

#include <hermit/has_member_function_xxx.hpp>
#include <hermit/has_static_member_function_xxx.hpp>
#include <hermit/has_member_variable_xxx.hpp>
#include <hermit/has_member_type_xxx.hpp>

namespace hermit {
  struct none_type {};
}

#define HPP_HAS_MEMBER_XXX( metafunc_name, name ) \
  namespace detail_type { \
    HPP_HAS_MEMBER_TYPE_XXX( metafunc_name, name ) \
  } \
  namespace detail_function { \
    HPP_HAS_MEMBER_FUNCTION_XXX( metafunc_name, name ) \
  } \
  namespace detail_static_function { \
    HPP_HAS_STATIC_MEMBER_FUNCTION_XXX( metafunc_name, name ) \
  } \
  namespace detail_variable { \
    HPP_HAS_MEMBER_VARIABLE_XXX( metafunc_name, name ) \
  } \
  template< \
    typename T, \
    typename ExpectedType = ::hermit::none_type, \
    typename is_function = typename boost::is_function< ExpectedType >::type > \
  struct metafunc_name : \
    public detail_variable:: metafunc_name < T, ExpectedType >::type {}; \
  template< typename T, typename ExpectedType > \
  struct metafunc_name < T, ExpectedType, boost::true_type > : \
    public boost::mpl::bool_< \
      detail_function:: metafunc_name < T, ExpectedType >::value || \
      detail_static_function:: metafunc_name < T, ExpectedType >::value \
    > {}; \
  template< typename T > \
  struct metafunc_name < T, ::hermit::none_type, boost::false_type > : \
    public detail_type:: metafunc_name < T > {};

#endif

