#ifndef HERMIT_IS_RANGE_HPP
#define HERMIT_IS_RANGE_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <hermit/has_member_function_xxx.hpp>

namespace hermit {
  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF( iterator )
    BOOST_MPL_HAS_XXX_TRAIT_DEF( const_iterator )
    template< typename T >
    struct has_range_requirement_types :
      public
        boost::mpl::and_<
          has_iterator< T >,
          has_const_iterator< T >
        >::type {};
    template<
      typename T,
      typename HasTypes = has_single_pass_range_requirement_type< T >
    >
    HPP_HAS_MEMBER_FUNCTION_XXX( has_member_function_begin, begin )
    HPP_HAS_MEMBER_FUNCTION_XXX( has_member_function_end, end )
    struct is_single_pass_range :
      public boost::false_type {};
    template<
      typename T
    >
    struct is_single_pass_range< T, true > :
      public boost::mpl::and_<
        boost::mpl::has_member_function_begin< T, ( typename T :: iterator ) () >,
        boost::mpl::has_member_function_begin< T, ( typename T :: const_iterator ) () >,
        boost::mpl::has_member_function_end< T, ( typename T :: iterator ) () >,
        boost::mpl::has_member_function_end< T, ( typename T :: const_iterator ) () >
      >::type {};

  }
}

int main() {
}

#endif
