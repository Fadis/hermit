#ifndef HERMIT_FLATTEN_ITERATOR
#define HERMIT_FLATTEN_ITERATOR

#include <boost/detail/endian.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace hermit {
template< typename BaseIterator, typename Traversal = typename boost::iterator_traversal< BaseIterator >::type >
class flatten_iterator {};

template< typename BaseIterator >
class flatten_iterator< BaseIterator, boost::incrementable_traversal_tag > :
  public boost::iterator_facade<
    flatten_iterator< BaseIterator, boost::incrementable_traversal_tag >,
    typename boost::iterator_value< BaseIterator >::type::reference,
    boost::incrementable_traversal_tag
  > {
public:
  flatten_iterator() {}
  flatten_iterator( const BaseIterator &base_ ) : base( base_ ), internal_iterator( base->begin() ) {}
  typename boost::iterator_value< BaseIterator >::type::reference dereference() const {
    return *internal_iterator;
  }
  void increment() {
    ++internal_iterator;
    if( internal_iterator == base->end() ) {
      ++base;
      internal_iterator = base->begin();
    }
  }
private:
  BaseIterator base;
  typename boost::iterator_value< BaseIterator >::type::iterator internal_iterator;
};


template< typename BaseIterator >
class flatten_iterator< BaseIterator, boost::single_pass_traversal_tag > :
  public boost::iterator_facade<
    flatten_iterator< BaseIterator, boost::single_pass_traversal_tag >,
    typename boost::iterator_value< BaseIterator >::type::reference,
    boost::single_pass_traversal_tag
  > {
public:
  flatten_iterator() {}
  flatten_iterator( const BaseIterator &base_ ) : base( base_ ), internal_iterator( base->begin() ) {}
  typename boost::iterator_value< BaseIterator >::type::reference dereference() const {
    return *internal_iterator;
  }
  void increment() {
    ++internal_iterator;
    if( internal_iterator == base->end() ) {
      ++base;
      internal_iterator = base->begin();
    }
  }
  bool equal( const flatten_iterator< BaseIterator, boost::single_pass_traversal_tag > &y ) const {
    return y.base == base && y.internal_iterator == internal_iterator;
  }
private:
  BaseIterator base;
  typename boost::iterator_value< BaseIterator >::type::iterator internal_iterator;
};

template< typename BaseIterator >
class flatten_iterator< BaseIterator, boost::forward_traversal_tag > :
  public boost::iterator_facade<
    flatten_iterator< BaseIterator, boost::forward_traversal_tag >,
    typename boost::iterator_value< BaseIterator >::type::reference,
    boost::forward_traversal_tag
  > {
public:
  flatten_iterator() {}
  flatten_iterator( const BaseIterator &base_ ) : base( base_ ), internal_iterator( base->begin() ) {}
  typename boost::iterator_value< BaseIterator >::type::reference dereference() const {
    return *internal_iterator;
  }
  void increment() {
    ++internal_iterator;
    if( internal_iterator == base->end() ) {
      ++base;
      internal_iterator = base->begin();
    }
  }
  bool equal( const flatten_iterator< BaseIterator, boost::forward_traversal_tag > &y ) const {
    return y.base == base && y.internal_iterator == internal_iterator;
  }
private:
  BaseIterator base;
  typename boost::iterator_value< BaseIterator >::type::iterator internal_iterator;
};

template< typename BaseIterator >
class flatten_iterator< BaseIterator, boost::bidirectional_traversal_tag > :
  public boost::iterator_facade<
    flatten_iterator< BaseIterator, boost::bidirectional_traversal_tag >,
    typename boost::iterator_value< BaseIterator >::type::reference,
    boost::bidirectional_traversal_tag
  > {
public:
  flatten_iterator() {}
  flatten_iterator( const BaseIterator &base_ ) : base( base_ ), internal_iterator( base->begin() ) {}
  typename boost::iterator_value< BaseIterator >::type::reference dereference() const {
    return *internal_iterator;
  }
  void increment() {
    ++internal_iterator;
    if( internal_iterator == base->end() ) {
      ++base;
      internal_iterator = base->begin();
    }
  }
  bool equal( const flatten_iterator< BaseIterator, boost::bidirectional_traversal_tag > &y ) const {
    return y.base == base && y.internal_iterator == internal_iterator;
  }
  void decrement() {
    if( internal_iterator == base->begin() ) {
      --base;
      internal_iterator = base->end();
    }
    --internal_iterator;
  }
private:
  BaseIterator base;
  typename boost::iterator_value< BaseIterator >::type::iterator internal_iterator;
};

template< typename BaseIterator >
class flatten_iterator< BaseIterator, boost::random_access_traversal_tag > :
  public boost::iterator_facade<
    flatten_iterator< BaseIterator, boost::random_access_traversal_tag >,
    typename boost::iterator_value< BaseIterator >::type::reference,
    boost::bidirectional_traversal_tag
  > {
public:
  flatten_iterator() {}
  flatten_iterator( const BaseIterator &base_ ) : base( base_ ), internal_iterator( base->begin() ) {}
  typename boost::iterator_value< BaseIterator >::type::reference dereference() const {
    return *internal_iterator;
  }
  void increment() {
    ++internal_iterator;
    if( internal_iterator == base->end() ) {
      ++base;
      internal_iterator = base->begin();
    }
  }
  bool equal( const flatten_iterator< BaseIterator, boost::random_access_traversal_tag > &y ) const {
    return y.base == base && y.internal_iterator == internal_iterator;
  }
  void decrement() {
    if( internal_iterator == base->begin() ) {
      --base;
      internal_iterator = base->end();
    }
    --internal_iterator;
  }
private:
  BaseIterator base;
  typename boost::iterator_value< BaseIterator >::type::iterator internal_iterator;
};
}

#endif
