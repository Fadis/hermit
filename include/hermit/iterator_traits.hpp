#ifndef HERMIT_ITERATOR_TRAITS_HPP
#define HERMIT_ITERATOR_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/is_readable_iterator.hpp>
#include <boost/iterator/is_lvalue_iterator.hpp>
#include <hermit/none_type.hpp>

namespace hermit {
  BOOST_MPL_HAS_XXX_TRAIT_DEF( iterator_category )  
#define HPP_ITERATOR_TRAITS_WRAPPER( metafunc_name, target_name ) \
  template< \
    typename T, \
    bool has_category = has_iterator_category< boost::detail::iterator_traits< T > >::value \
  > \
  struct metafunc_name { \
    typedef none_type type; \
  }; \
  template< \
    typename T \
  > \
  struct metafunc_name < T, true > { \
    typedef typename target_name < T >::type type; \
  };
HPP_ITERATOR_TRAITS_WRAPPER( iterator_value, boost::iterator_value )
HPP_ITERATOR_TRAITS_WRAPPER( iterator_reference, boost::iterator_reference )
HPP_ITERATOR_TRAITS_WRAPPER( iterator_pointer, boost::iterator_pointer )
HPP_ITERATOR_TRAITS_WRAPPER( iterator_difference, boost::iterator_difference )
HPP_ITERATOR_TRAITS_WRAPPER( iterator_category, boost::iterator_category )
HPP_ITERATOR_TRAITS_WRAPPER( iterator_traversal, boost::iterator_traversal )
#undef HPP_ITERATOR_TRAITS_WRAPPER

#define HPP_ITERATOR_TRAVERSAL_WRAPPER( metafunc_name, target_name ) \
  template< \
    typename T, \
    bool has_category = has_iterator_category< boost::detail::iterator_traits< T > >::value \
  > \
  struct metafunc_name : \
    public std::false_type {}; \
  template< typename T > \
  struct metafunc_name < T, true > : \
    public boost::is_convertible< \
      typename iterator_traversal< T >::type, \
      target_name \
    >::type {};
HPP_ITERATOR_TRAVERSAL_WRAPPER( is_incrementable_traversal_iterator, boost::incrementable_traversal_tag )
HPP_ITERATOR_TRAVERSAL_WRAPPER( is_single_pass_traversal_iterator, boost::single_pass_traversal_tag )
HPP_ITERATOR_TRAVERSAL_WRAPPER( is_forward_traversal_iterator, boost::forward_traversal_tag )
HPP_ITERATOR_TRAVERSAL_WRAPPER( is_bidirectional_traversal_iterator, boost::bidirectional_traversal_tag )
HPP_ITERATOR_TRAVERSAL_WRAPPER( is_random_access_traversal_iterator, boost::random_access_traversal_tag )

#undef HPP_ITERATOR_TRAVERSAL_WRAPPER
  
  template< typename T >
  struct is_iterator :
    public boost::mpl::or_<
      is_incrementable_traversal_iterator< T >,
      is_single_pass_traversal_iterator< T >,
      is_forward_traversal_iterator< T >,
      is_bidirectional_traversal_iterator< T >,
      is_random_access_traversal_iterator< T >
    >::type {};

  template<
    typename T,
    bool has_category = has_iterator_category< boost::detail::iterator_traits< T > >::value
  >
  struct is_readable_iterator :
    public std::false_type {};
  template<
    typename T
  >
  struct is_readable_iterator< T, true > :
    public boost::is_readable_iterator< T >::type {};

  template<
    typename IteratorType,
    typename ValueType,
    bool has_category = has_iterator_category< boost::detail::iterator_traits< IteratorType > >::value
  >
  struct is_writable_iterator :
    public std::false_type {};
  template<
    typename IteratorType,
    typename ValueType
  >
  class is_writable_iterator< IteratorType, ValueType, true > {
  private:
    template<
      typename IteratorType_,
      typename ReturnType = decltype( *std::declval< IteratorType_ >() = std::declval< ValueType >() )
    >
    static std::true_type check( IteratorType_&& );
    static std::false_type check( ... );
  public:
    typedef decltype( check( std::declval< IteratorType >() ) ) type;
    static constexpr bool value = type::value;
  };

  template<
    typename T,
    bool is_writable_iterator_ = is_writable_iterator< T, typename iterator_value< T >::type >::value
  >
  struct is_swappable_iterator : public std::false_type {};
  template<
    typename T
  >
  struct is_swappable_iterator< T, true > :
    public is_readable_iterator< T >::type {};

  template<
    typename T,
    bool has_category = has_iterator_category< boost::detail::iterator_traits< T > >::value
  >
  struct is_lvalue_iterator :
    public std::false_type {};
  template<
    typename T
  >
  struct is_lvalue_iterator< T, true > :
    public boost::is_lvalue_iterator< T >::type {};
}

#endif

