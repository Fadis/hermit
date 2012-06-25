#ifndef HERMIT_TO_MEMBER_FUNCTION_HPP
#define HERMIT_TO_MEMBER_FUNCTION_HPP

#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/components.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <cxxabi.h>

template< typename T, typename U >
struct to_member_function
: public
  boost::function_types::member_function_pointer<
    typename boost::mpl::insert<
      typename boost::function_types::components< T >::type,
      typename boost::mpl::next<
        typename boost::mpl::begin<
          typename boost::function_types::components< T >::type
        >::type
      >::type,
      U
    >::type
  > {};

#endif

