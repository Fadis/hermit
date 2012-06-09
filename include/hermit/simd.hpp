#ifndef HERMIT_SIMD_HPP
#define HERMIT_SIMD_HPP

#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE

#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/shift_left.hpp>

namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, typename U = boost::mpl::int_< 1 > >
        struct smallest_pot
        : public boost::mpl::if_<
          boost::mpl::bool_< T::value <= U::value >,
          U,
          smallest_pot< T, boost::mpl::shift_left< U, boost::mpl::int_< 1 > > >
          >::type {};

      template< typename T, unsigned int size >
        struct vector {
          typedef T type __attribute( ( vector_size( smallest_pot< boost::mpl::int_< size > >::value * sizeof( T ) ) ) );
        };
    }
  }
}

# endif
#endif

#ifdef __clang__
# if __clang_major__ >= 3

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE

namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, unsigned int size >
        struct vector {
          typedef T type __attribute__( ( ext_vector_type( size ) ) );
        };
    }
  }
}

# endif
#endif

#ifndef HERMIT_VECTOR_TYPE_IS_AVAILABLE

#include <boost/array.hpp>

// oops, I have to define arithmetic operation for boost::array!
namespace hermit {
  namespace simd {
    namespace detail {
      template< typename T, unsigned int size >
        struct vector {
          typedef boost::array< T, size > type;
        };
    }
  }
}

#endif

#include <boost/iterator_adaptors.hpp>
#include <boost/ref.hpp>

namespace hermit {
  namespace simd {

    // It is not a vector, I have to think something better name.
    template< typename T, unsigned int size_ >
    class vector {
      public:
        class const_iterator : boost::iterator_facade<
          const_iterator,
          const T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef const_iterator this_type;
          typedef boost::reference_wrapper< const typename detail::vector< T, size_ >::type > base_type;
          const_iterator( base_type base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          const T& dereference() const {
            return base[ index ];
          }
          bool equal( const this_type &comp ) const {
            return &base == &comp.base && index == comp.index;
          }
          void increment() {
            ++index;
          }
          void decrement() {
            --index;
          }
          void advance( int distance ) {
            index += distance;
          }
          int distance_to( const this_type &comp ) const {
            return comp.index - index;
          }
        private:
          const base_type base;
          int index;
        };
        class iterator : boost::iterator_facade<
          iterator,
          T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef iterator this_type;
          typedef boost::reference_wrapper< typename detail::vector< T, size_ >::type > base_type;
          iterator( base_type base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          T& dereference() {
            return base[ index ];
          }
          bool equal( const this_type &comp ) const {
            return &base == &comp.base && index == comp.index;
          }
          void increment() {
            ++index;
          }
          void decrement() {
            --index;
          }
          void advance( int distance ) {
            index += distance;
          }
          int distance_to( const this_type &comp ) const {
            return comp.index - index;
          }
          operator const_iterator () const {
            return const_iterator( base, index );
          }
        private:
          base_type base;
          int index;
        };
        typedef vector< T, size_ > this_type;
        typedef typename detail::vector< T, size_ >::type raw_type;
        typedef size_t size_type;
        vector() {}
        explicit vector( const raw_type &value_ ) : value( value_ ) {}
        this_type operator+( const this_type &right ) {
          return vector( value + right.value );
        }
        this_type operator-( const this_type &right ) {
          return vector( value - right.value );
        }
        T operator[]( unsigned int index ) {
          return value[ index ];
        }
        const T operator[]( unsigned int index ) const {
          return value[ index ];
        }
        iterator begin() {
          return iterator( boost::ref( value ), 0 );
        }
        const_iterator begin() const {
          return const_iterator( boost::ref( value ), 0 );
        }
        iterator end() {
          return iterator( boost::ref( value ), size() );
        }
        const_iterator end() const {
          return const_iterator( boost::ref( value ), size() );
        }
        static size_type size() {
          return size_;
        }
      private:
        raw_type value;
    };
  }
}

#endif
