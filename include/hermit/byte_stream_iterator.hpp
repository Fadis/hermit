#ifndef HERMIT_BYTE_STREAM_ITERATOR
#define HERMIT_BYTE_STREAM_ITERATOR

#include <boost/detail/endian.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace hermit {
template< typename BaseIterator, typename Traversal = typename boost::iterator_traversal< BaseIterator >::type >
class little_byte_stream_iterator {};

template< typename BaseIterator >
class little_byte_stream_iterator< BaseIterator, boost::incrementable_traversal_tag > :
  public boost::iterator_facade<
    little_byte_stream_iterator< BaseIterator, boost::incrementable_traversal_tag >,
    uint8_t&,
    boost::incrementable_traversal_tag
  > {
public:
  little_byte_stream_iterator() {}
  little_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class little_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag > :
  public boost::iterator_facade<
    little_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag >,
    uint8_t&,
    boost::single_pass_traversal_tag
  > {
public:
    little_byte_stream_iterator() {}
  little_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const little_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class little_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag > :
  public boost::iterator_facade<
    little_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag >,
    uint8_t&,
    boost::forward_traversal_tag
  > {
public:
    little_byte_stream_iterator() {}
  little_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const little_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class little_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag > :
  public boost::iterator_facade<
    little_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag >,
    uint8_t&,
    boost::bidirectional_traversal_tag
  > {
public:
    little_byte_stream_iterator() {}
  little_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const little_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
  void decrement() {
    if( --offset < 0 ) {
      offset = object_size - 1;
      --base;
    }
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class little_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > :
  public boost::iterator_facade<
    little_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag >,
    uint8_t&,
    boost::random_access_traversal_tag
  > {
public:
    little_byte_stream_iterator() {}
  little_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const little_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > &y ) const {
    return y.base == base && y.offset == offset;
  }
  void decrement() {
    if( --offset < 0 ) {
      offset = object_size - 1;
      --base;
    }
  }
  void advance( int n ) {
    if( n > 0 ) {
      offset += n % object_size;
      base += n / object_size;
    }
    else if( n < 0 ) {
      n = -n;
      offset -= n % object_size;
      base -= n / object_size;
    }
  }
  typename boost::iterator_difference< BaseIterator >::type
  distance_to( const little_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > &z ) const {
    return ( z.base - base ) * object_size + z.offset - offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};



template< typename BaseIterator, typename Traversal = typename boost::iterator_traversal< BaseIterator >::type >
class big_byte_stream_iterator {};

template< typename BaseIterator >
class big_byte_stream_iterator< BaseIterator, boost::incrementable_traversal_tag > :
  public boost::iterator_facade<
    big_byte_stream_iterator< BaseIterator, boost::incrementable_traversal_tag >,
    uint8_t&,
    boost::incrementable_traversal_tag
  > {
public:
  big_byte_stream_iterator() {}
  big_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class big_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag > :
  public boost::iterator_facade<
    big_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag >,
    uint8_t&,
    boost::single_pass_traversal_tag
  > {
public:
    big_byte_stream_iterator() {}
  big_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const big_byte_stream_iterator< BaseIterator, boost::single_pass_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class big_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag > :
  public boost::iterator_facade<
    big_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag >,
    uint8_t&,
    boost::forward_traversal_tag
  > {
public:
    big_byte_stream_iterator() {}
  big_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const big_byte_stream_iterator< BaseIterator, boost::forward_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class big_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag > :
  public boost::iterator_facade<
    big_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag >,
    uint8_t&,
    boost::bidirectional_traversal_tag
  > {
public:
    big_byte_stream_iterator() {}
  big_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const big_byte_stream_iterator< BaseIterator, boost::bidirectional_traversal_tag > &y ) const {
    return y.base = base && y.offset == offset;
  }
  void decrement() {
    if( --offset < 0 ) {
      offset = object_size - 1;
      --base;
    }
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};

template< typename BaseIterator >
class big_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > :
  public boost::iterator_facade<
    big_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag >,
    uint8_t&,
    boost::random_access_traversal_tag
  > {
public:
    big_byte_stream_iterator() {}
  big_byte_stream_iterator( const BaseIterator &base_ ) : base( base_ ), offset( 0 ),
    object_size( sizeof( typename boost::iterator_value< BaseIterator >::type ) ) {}

  uint8_t &dereference() const {
#ifdef BOOST_BIG_ENDIAN
    return reinterpret_cast< uint8_t* >( &*base )[ offset ];
#else
    return reinterpret_cast< uint8_t* >( &*base )[ object_size - 1 - offset ];
#endif
  }
  void increment() {
    if( ++offset == object_size ) {
      offset = 0;
      ++base;
    }
  }
  bool equal( const big_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > &y ) const {
    return y.base == base && y.offset == offset;
  }
  void decrement() {
    if( --offset < 0 ) {
      offset = object_size - 1;
      --base;
    }
  }
  void advance( int n ) {
    if( n > 0 ) {
      offset += n % object_size;
      base += n / object_size;
    }
    else if( n < 0 ) {
      n = -n;
      offset -= n % object_size;
      base -= n / object_size;
    }
  }
  typename boost::iterator_difference< BaseIterator >::type
  distance_to( const big_byte_stream_iterator< BaseIterator, boost::random_access_traversal_tag > &z ) const {
    return ( z.base - base ) * object_size + z.offset - offset;
  }
private:
  BaseIterator base;
  int offset;
  int object_size;
};
}

#endif
