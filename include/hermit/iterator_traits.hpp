#ifndef HERMIT_ITERATOR_TRAITS_HPP
#define HERMIT_ITERATOR_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_archetypes.hpp>
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

}

#endif

