#ifndef HERMIT_RANGE_TRAITS_HPP
#define HERMIT_RANGE_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <hermit/has_member_function_xxx.hpp>
#include <hermit/has_function_xxx.hpp>
#include <hermit/iterator_traits.hpp>
#include <boost/range.hpp>
#include <boost/iterator/is_readable_iterator.hpp>
#include <boost/iterator/is_lvalue_iterator.hpp>

namespace hermit {
  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF( iterator )
    BOOST_MPL_HAS_XXX_TRAIT_DEF( const_iterator )
    BOOST_MPL_HAS_XXX_TRAIT_DEF( type )
    template< typename T >
    struct has_range_method1_requirement_types :
      public
        boost::mpl::and_<
          has_iterator< T >,
          has_const_iterator< T >
        >::type {};
    
    HPP_HAS_MEMBER_FUNCTION_XXX( has_member_function_begin, begin )
    HPP_HAS_MEMBER_FUNCTION_XXX( has_member_function_end, end )

    template<
      typename T,
      bool has_type = has_range_method1_requirement_types< T >::value
    >
    struct is_range_method1 :
      public boost::false_type {};
    template<
      typename T
    >
    struct is_range_method1< T, true > :
      public boost::mpl::and_<
        has_member_function_begin< T, typename T :: iterator () >,
        has_member_function_begin< T, typename T :: const_iterator () const >,
        has_member_function_end< T, typename T :: iterator () >,
        has_member_function_end< T, typename T :: const_iterator () const >
      >::type {};

    template< typename T >
    struct has_range_method2_requirement_types :
      public
        boost::mpl::and_<
          has_type< boost::range_mutable_iterator< T > >,
          has_type< boost::range_const_iterator< T > >
        >::type {};

    HPP_HAS_FUNCTION_XXX( has_function_range_begin, range_begin )
    HPP_HAS_FUNCTION_XXX( has_function_range_end, range_end )
    template<
      typename T,
      bool has_type = has_range_method2_requirement_types< T >::value
    >
    struct is_range_method2 :
      public boost::false_type {};
    template<
      typename T
    >
    struct is_range_method2< T, true > :
      public boost::mpl::and_<
        has_function_range_begin< typename boost::range_mutable_iterator< T >::type ( T& ) >,
        has_function_range_begin< typename boost::range_const_iterator< T >::type ( const T& ) >,
        has_function_range_end< typename boost::range_mutable_iterator< T >::type ( T& ) >,
        has_function_range_end< typename boost::range_const_iterator< T >::type ( const T& ) >
      >::type {};

    template< typename T >
    struct is_range_capable_iterator :
      public boost::mpl::or_<
        is_single_pass_traversal_iterator< T >,
        is_forward_traversal_iterator< T >,
        is_bidirectional_traversal_iterator< T >,
        is_random_access_traversal_iterator< T >
      >::type {};
  }
  template< typename T >
  struct is_range :
    public boost::mpl::or_<
      detail::is_range_method1< T >,
      detail::is_range_method2< T >
    >::type {};
  template< typename Iterator >
  struct is_range< std::pair< Iterator, Iterator > > :
    public detail::is_range_capable_iterator< Iterator >::type {};

#define HPP_RANGE_TRAITS_WRAPPER( metafunc_name, target_name ) \
  template< \
    typename T, \
    bool is_range_ = is_range< T >::value \
  > \
  struct metafunc_name : \
    public std::false_type {}; \
  template< \
    typename T \
  > \
  struct metafunc_name < T, true > : \
    public boost::mpl::and_< \
      target_name < typename boost::range_iterator< typename boost::remove_const< T >::type >::type >, \
      target_name < typename boost::range_iterator< typename boost::add_const< T >::type >::type > \
    >::type {};
HPP_RANGE_TRAITS_WRAPPER( is_single_pass_traversal_range, is_single_pass_traversal_iterator )
HPP_RANGE_TRAITS_WRAPPER( is_forward_traversal_range, is_forward_traversal_iterator )
HPP_RANGE_TRAITS_WRAPPER( is_bidirectional_traversal_range, is_bidirectional_traversal_iterator )
HPP_RANGE_TRAITS_WRAPPER( is_random_access_traversal_range, is_random_access_traversal_iterator )

#undef HPP_RANGE_TRAITS_WRAPPER

#define HPP_RANGE_TRAITS_WRAPPER( metafunc_name ) \
  template< \
    typename T, \
    bool is_range_ = is_range< T >::value \
  > \
  struct metafunc_name { \
    typedef none_type type; \
  }; \
  template< \
    typename T \
  > \
  struct metafunc_name < T, true > : \
    public boost :: metafunc_name < T > {};

HPP_RANGE_TRAITS_WRAPPER( range_iterator )
HPP_RANGE_TRAITS_WRAPPER( range_value )
HPP_RANGE_TRAITS_WRAPPER( range_reference )
HPP_RANGE_TRAITS_WRAPPER( range_pointer )
HPP_RANGE_TRAITS_WRAPPER( range_category )

#undef HPP_RANGE_TRAITS_WRAPPER

  template<
    typename T,
    bool is_range_ = is_forward_traversal_range< T >::value
  >
  struct range_difference { 
    typedef none_type type;
  };
  template<
    typename T
  >
  struct range_difference < T, true > :
    public boost :: range_difference < T > {};

  template<
    typename T,
    bool is_range_ = is_bidirectional_traversal_range< T >::value
  >
  struct range_reverse_iterator { 
    typedef none_type type;
  };
  template<
    typename T
  >
  struct range_reverse_iterator < T, true > :
    public boost :: range_reverse_iterator < T > {};

  template<
    typename T,
    bool is_range_ = is_range< T >::value
  >
  struct is_readable_range :
    public std::false_type {};
  template<
    typename T
  >
  struct is_readable_range < T, true > :
    public boost::mpl::and_<
      is_readable_iterator < typename boost::range_iterator< typename boost::remove_const< T >::type >::type >,
      is_readable_iterator < typename boost::range_iterator< typename boost::add_const< T >::type >::type >
    >::type {};

  template<
    typename T,
    typename ValueType,
    bool is_range_ = is_range< T >::value
  >
  struct is_writable_range :
    public std::false_type {};
  template<
    typename T,
    typename ValueType
  >
  struct is_writable_range < T, ValueType, true > :
    public boost::mpl::and_<
      is_writable_iterator < typename boost::range_iterator< typename boost::remove_const< T >::type >::type, ValueType >,
      is_readable_iterator < typename boost::range_iterator< typename boost::add_const< T >::type >::type >
    >::type {};

  template<
    typename T,
    bool is_range_ = is_range< T >::value
  >
  struct is_swappable_range :
    public std::false_type {};
  template<
    typename T
  >
  struct is_swappable_range < T, true > :
    public boost::mpl::and_<
      is_swappable_iterator < typename boost::range_iterator< typename boost::remove_const< T >::type >::type >,
      is_readable_iterator < typename boost::range_iterator< typename boost::add_const< T >::type >::type >
    >::type {};

  template<
    typename T,
    bool is_range_ = is_range< T >::value
  >
  struct is_lvalue_range :
    public std::false_type {};
  template<
    typename T
  >
  struct is_lvalue_range < T, true > :
    public boost::mpl::and_<
      is_lvalue_iterator < typename boost::range_iterator< typename boost::remove_const< T >::type >::type >,
      is_readable_iterator < typename boost::range_iterator< typename boost::add_const< T >::type >::type >
    >::type {};
}

#endif

