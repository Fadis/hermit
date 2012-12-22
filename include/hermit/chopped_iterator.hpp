#ifndef HERMIT_CHOPPED_ITERATOR
#define HERMIT_CHOPPED_ITERATOR

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace hermit {
template< typename BaseIterator, typename Traversal = typename boost::iterator_traversal< BaseIterator >::type >
class chopped_iterator {};

template< typename BaseIterator >
class chopped_iterator< BaseIterator, boost::forward_traversal_tag > :
  public boost::iterator_facade<
    chopped_iterator< BaseIterator, boost::forward_traversal_tag >,
    const std::pair< BaseIterator, BaseIterator >,
    boost::forward_traversal_tag
  > {
public:
  chopped_iterator() {}
  chopped_iterator( const BaseIterator &base_, size_t width_ ) : range( base_, base_ ), width( width_ ) {
    std::advance( range.second, width );
  }
  const std::pair< BaseIterator, BaseIterator > &dereference() const {
    return range;
  }
  void increment() {
    std::advance( range.first, width );
    std::advance( range.second, width );
  }
  bool equal( const chopped_iterator< BaseIterator, boost::forward_traversal_tag > &y ) const {
    return y.range.first == range.first;
  }
private:
  std::pair< BaseIterator, BaseIterator > range;
  size_t width;
};

template< typename BaseIterator >
class chopped_iterator< BaseIterator, boost::bidirectional_traversal_tag > :
  public boost::iterator_facade<
    chopped_iterator< BaseIterator, boost::bidirectional_traversal_tag >,
    const std::pair< BaseIterator, BaseIterator >,
    boost::bidirectional_traversal_tag
  > {
public:
  chopped_iterator( const BaseIterator &base_, size_t width_ ) : range( base_, base_ ), width( width_ ) {
    std::advance( range.second, width );
  }
  const std::pair< BaseIterator, BaseIterator > &dereference() const {
    return range;
  }
  void increment() {
    std::advance( range.first, width );
    std::advance( range.second, width );
  }
  bool equal( const chopped_iterator< BaseIterator, boost::bidirectional_traversal_tag > &y ) const {
    return y.range.first == range.first;
  }
  void decrement() {
    std::advance( range.first, -static_cast< ptrdiff_t >( width ) );
    std::advance( range.second, -static_cast< ptrdiff_t >( width ) );
  }
private:
  std::pair< BaseIterator, BaseIterator > range;
  size_t width;
};

template< typename BaseIterator >
class chopped_iterator< BaseIterator, boost::random_access_traversal_tag > :
  public boost::iterator_facade<
    chopped_iterator< BaseIterator, boost::random_access_traversal_tag >,
    const std::pair< BaseIterator, BaseIterator >,
    boost::random_access_traversal_tag
  > {
public:
  chopped_iterator( const BaseIterator &base_, size_t width_ ) : range( base_, base_ ), width( width_ ) {
    std::advance( range.second, width );
  }
  const std::pair< BaseIterator, BaseIterator > &dereference() const {
    return range;
  }
  void increment() {
    std::advance( range.first, width );
    std::advance( range.second, width );
  }
  void advance( ptrdiff_t n ) {
    std::advance( range.first, static_cast< ptrdiff_t >( width ) * n );
    std::advance( range.second, static_cast< ptrdiff_t >( width ) * n );
  }
  bool equal( const chopped_iterator< BaseIterator, boost::random_access_traversal_tag > &y ) const {
    return y.range.first == range.first;
  }
  void decrement() {
    std::advance( range.first, -static_cast< ptrdiff_t >( width ) );
    std::advance( range.second, -static_cast< ptrdiff_t >( width ) );
  }
  void distance_to( const chopped_iterator< BaseIterator, boost::random_access_traversal_tag > &y ) const {
    return std::distance( range.first, y.range.first ) / static_cast< ptrdiff_t >( width );
  }
private:
  std::pair< BaseIterator, BaseIterator > range;
  size_t width;
};

}

#endif
