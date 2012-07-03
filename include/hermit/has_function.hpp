#ifndef HERMIT_HAS_FUNCTION_XXX_HPP
#define HERMIT_HAS_FUNCTION_XXX_HPP

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
  } \
  template< typename T, typename Func > \
  struct metafunc_name : \
    public boost::mpl::bool_< \
      detail:: metafunc_name < T, Func >::value \
    > { \
  }; \

#endif

