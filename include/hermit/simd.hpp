#ifndef HERMIT_SIMD_HPP
#define HERMIT_SIMD_HPP

#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE

#include <boost/tr1/array.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/ref.hpp>

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
    template< typename T, unsigned int global_size_, unsigned int local_size_ >
    class vector {
      public:
        class const_iterator : public boost::iterator_facade<
          const_iterator,
          const T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef const_iterator this_type;
          typedef const typename detail::vector< T, local_size_ >::type base_type;
          const_iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          const T& dereference() const {
            return reinterpret_cast< const T* >( base + index / local_size_ )[ index % local_size_ ];
          }
          bool equal( const this_type &comp ) const {
            return base == comp.base && index == comp.index;
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
          base_type *base;
          int index;
        };
        class iterator : public boost::iterator_facade<
          iterator,
          T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef iterator this_type;
          typedef typename detail::vector< T, local_size_ >::type base_type;
          iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          T& dereference() const {
            return reinterpret_cast< T* >( base + index / local_size_ )[ index % local_size_ ];
          }
          bool equal( const this_type &comp ) const {
            return base == comp.base && index == comp.index;
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
          base_type *base;
          int index;
        };
        typedef vector< T, global_size_, local_size_ > self_type;
        typedef std::array< typename detail::vector< T, local_size_ >::type, global_size_ / local_size_ + ( global_size_ % local_size_ ? 1 : 0 ) > internal_type;
        typedef size_t size_type;
        vector() {}
        vector( const self_type &src ) : value( src.value ) {}
        vector( const internal_type &src ) : value( src ) {}
        T &operator[]( unsigned int index ) {
          return reinterpret_cast< T* >( &value[ index / local_size_ ] )[ index % local_size_ ];
        }
        const T &operator[]( unsigned int index ) const {
          return reinterpret_cast< T* >( &value[ index / local_size_ ] )[ index % local_size_ ];
        }
        iterator begin() {
          return iterator( value.data(), 0 );
        }
        const_iterator begin() const {
          return const_iterator( value.data(), 0 );
        }
        iterator end() {
          return iterator( value.data(), size() );
        }
        const_iterator end() const {
          return const_iterator( value.data(), size() );
        }
        static size_type size() {
          return global_size_;
        }
        internal_type &get() {
          return value;
        }
        const internal_type &get() const {
          return value;
        }
      private:
        internal_type value;
    };
    template< typename T, unsigned int global_size_, unsigned int local_size_ >
    static vector< T, global_size_, local_size_ > operator+( const vector< T, global_size_, local_size_ > &left, const vector< T, global_size_, local_size_ >&right ) {
      vector< T, global_size_, local_size_ > dest;
      for( unsigned int index = 0; index != dest.get().size(); ++index )
        dest.get()[ index ] = left.get()[ index ] + right.get()[ index ];
      return dest;
    }
    template< typename T, unsigned int global_size_, unsigned int local_size_ >
    static vector< T, global_size_, local_size_ > operator*( const vector< T, global_size_, local_size_ > &left, const vector< T, global_size_, local_size_ >&right ) {
      vector< T, global_size_, local_size_ > dest;
      for( unsigned int index = 0; index != dest.get().size(); ++index )
        dest.get()[ index ] = left.get()[ index ] * right.get()[ index ];
      return dest;
    }
  }
}

# endif
#endif

#ifdef __clang__
# if __clang_major__ >= 3

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE


# endif
#endif

#ifndef HERMIT_VECTOR_TYPE_IS_AVAILABLE


#endif



#endif
