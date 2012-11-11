#include <stdexcept>
#include <chrono>
#include <vector>
#include <array>
#include <boost/fusion/container/vector.hpp>
#include <boost/proto/proto.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/ref.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/swap.hpp>
#include <boost/type_traits.hpp>
#include <boost/preprocessor.hpp>
namespace stream {
  namespace simd {
    namespace compiler {
#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
      template< typename T >
      struct internal_vector {
        static constexpr bool vectorized = false;
        typedef T type;
        static constexpr size_t length = 1;
      };
#if defined(__AVX__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute( ( vector_size( 32 ) ) );
        static constexpr size_t length = 8;
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute( ( vector_size( 32 ) ) );
        static constexpr size_t length = 4;
      };
#elif defined(__SSE2__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 2;
      };
#elif defined(__SSE__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
      };
#endif
#if defined(__MMX__)
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
      };
#endif
#define HERMIT_VECTOR_TYPE_IS_AVAILABLE
      template< typename T, typename Size, bool is_scalar = internal_vector< T >::length == 1 >
      struct vector {
          static constexpr unsigned int group_size = Size::value / internal_vector< T >::length +
            ( ( Size::value % internal_vector< T >::length ) ? 1 : 0 );
          static constexpr unsigned int local_size = internal_vector< T >::length;
          typedef typename internal_vector< T >::type packed_element;
          union accessible_element {
            packed_element packed;
            T unpacked[ local_size ];
          };
          typedef std::array<
            accessible_element,
            group_size
          > type;
        inline static T &get_scalar(
          type &src,
          size_t index
        ) {
          return src[ index / local_size ].unpacked[ index % local_size ];
        }
        inline static const T &get_scalar(
          const type &src,
          size_t index
        ) {
          return src[ index / local_size ].unpacked[ index % local_size ];
        }
      };
      template< typename T, typename Size >
      struct vector< T, Size, true > {
          static constexpr unsigned int group_size = Size::value;
          static constexpr unsigned int local_size = 1;
          typedef std::array<
            T,
            group_size
          > type;
        inline static T &get_scalar(
          type &src,
          size_t index
        ) {
          return src[ index ];
        }
        inline static const T &get_scalar(
          const type &src,
          size_t index
        ) {
          return src[ index ];
        }
      };
#endif
#endif
#if defined( __clang__ )
      template< typename T >
      struct internal_vector {
        static constexpr bool vectorized = false;
        typedef T type;
        static constexpr size_t length = 1;
      };
#if defined(__AVX__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute__( ( __vector_size__( 32 ) ) );
        static constexpr size_t length = 8;
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute__( ( __vector_size__( 32 ) ) );
        static constexpr size_t length = 4;
      };
#elif defined(__SSE2__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 2;
      };
#elif defined(__SSE__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
      };
#endif
#if defined(__MMX__)
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
      };
#endif
#define HERMIT_VECTOR_TYPE_IS_AVAILABLE
      template< typename T, typename Size, bool is_scalar = internal_vector< T >::length == 1 >
      struct vector {
          static constexpr unsigned int group_size = Size::value / internal_vector< T >::length +
            ( ( Size::value % internal_vector< T >::length ) ? 1 : 0 );
          static constexpr unsigned int local_size = internal_vector< T >::length;
          typedef typename internal_vector< T >::type packed_element;
          union accessible_element {
            packed_element packed;
            T unpacked[ local_size ];
          };
          typedef std::array<
            accessible_element,
            group_size
          > type;
        inline static T &get_scalar(
          type &src,
          size_t index
        ) {
          return src[ index / local_size ].unpacked[ index % local_size ];
        }
        inline static const T &get_scalar(
          const type &src,
          size_t index
        ) {
          return src[ index / local_size ].unpacked[ index % local_size ];
        }
      };
      template< typename T, typename Size >
      struct vector< T, Size, true > {
          static constexpr unsigned int group_size = Size::value;
          static constexpr unsigned int local_size = 1;
          typedef std::array<
            T,
            group_size
          > type;
        inline static T &get_scalar(
          type &src,
          size_t index
        ) {
          return src[ index ];
        }
        inline static const T &get_scalar(
          const type &src,
          size_t index
        ) {
          return src[ index ];
        }
      };
#endif
    }
    namespace container {
    template< typename T, typename Size >
    class vector {
      public:
        static const size_t local_size_ = compiler::internal_vector< T >::length;
        class const_iterator : public boost::iterator_facade<
          const_iterator,
          const T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef const_iterator this_type;
          typedef const typename compiler::vector< T, Size >::type base_type;
          inline const_iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          inline const T& dereference() const {
            return compiler::vector< T, Size >::get_scalar( *base, index );
          }
          inline bool equal( const this_type &comp ) const {
            return base == comp.base && index == comp.index;
          }
          inline void increment() {
            ++index;
          }
          inline void decrement() {
            --index;
          }
          inline void advance( int distance ) {
            index += distance;
          }
          inline ptrdiff_t distance_to( const this_type &comp ) const {
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
          typedef typename compiler::vector< T, Size >::type base_type;
          inline iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          inline T& dereference() const {
            return compiler::vector< T, Size >::get_scalar( *base, index );
          }
          inline bool equal( const this_type &comp ) const {
            return base == comp.base && index == comp.index;
          }
          inline void increment() {
            ++index;
          }
          inline void decrement() {
            --index;
          }
          inline void advance( int distance ) {
            index += distance;
          }
          inline ptrdiff_t distance_to( const this_type &comp ) const {
            return comp.index - index;
          }
          inline operator const_iterator () const {
            return const_iterator( base, index );
          }
        private:
          base_type *base;
          int index;
        };
        typedef vector< T, Size > self_type;
        typedef typename compiler::vector< T, Size >::type base_type;
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef boost::reverse_iterator< iterator > reverse_iterator;
        typedef boost::reverse_iterator< const_iterator > const_reverse_iterator;
        typedef typename base_type::iterator vector_iterator;
        typedef typename base_type::const_iterator const_vector_iterator;
        typedef typename base_type::reverse_iterator reverse_vector_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_vector_iterator;
        inline vector() {}
        inline vector( const self_type &src ) : value( src.value ) {
        }
        inline vector( self_type &&src ) : value( src.value ) {
        }
        inline self_type &operator=( const self_type &src ) {
          value = src.value;
          return *this;
        }
        inline self_type &operator=( self_type &&src ) {
          value = src.value;
          return *this;
        }
        template< typename Iterator >
        inline vector( Iterator begin, Iterator end ) {
          std::copy( begin, end, begin() );
        }
        inline vector( const std::initializer_list< T > &list ) {
          if( list.size() > size() )
            throw std::range_error( "The initializer list is too big." );
          std::copy( list.begin(), list.end(), begin() );
        }
        inline reference operator[]( unsigned int index ) {
          return compiler::vector< T, Size >::get_scalar( value, index );
        }
        inline const_reference operator[]( unsigned int index ) const {
          return compiler::vector< T, Size >::get_scalar( value, index );
        }
        inline reference at( unsigned int index ) {
          if( index >= size() )
            throw std::out_of_range( "The index is larger than vector size." );
          return compiler::vector< T, Size >::get_scalar( value, index );
        }
        inline const_reference at( unsigned int index ) const {
          if( index >= size() )
            throw std::out_of_range( "The index is larger than vector size." );
          return compiler::vector< T, Size >::get_scalar( value, index );
        }
        inline reference front() {
          return compiler::vector< T, Size >::get_scalar( value, 0 );
        }
        inline reference back() {
          return compiler::vector< T, Size >::get_scalar( value, size() - 1 );
        }
        inline const_reference front() const {
          return compiler::vector< T, Size >::get_scalar( value, 0 );
        }
        inline const_reference back() const {
          return compiler::vector< T, Size >::get_scalar( value, size() - 1 );
        }
        inline iterator begin() {
          return iterator( &value, 0 );
        }
        inline const_iterator begin() const {
          return const_iterator( &value, 0 );
        }
        inline reverse_iterator rbegin() {
          return reverse_iterator( iterator( &value, size() ) );
        }
        inline const_reverse_iterator rbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        inline const_iterator cbegin() const {
          return const_iterator( &value, 0 );
        }
        inline const_reverse_iterator crbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        inline iterator end() {
          return iterator( &value, size() );
        }
        inline const_iterator end() const {
          return const_iterator( &value, size() );
        }
        inline reverse_iterator rend() {
          return reverse_iterator( iterator( &value, 0 ) );
        }
        inline const_reverse_iterator rend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        inline const_iterator cend() const {
          return const_iterator( &value, size() );
        }
        inline const_reverse_iterator crend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        inline vector_iterator vbegin() {
          return value.begin();
        }
        inline const_vector_iterator vbegin() const {
          return value.begin();
        }
        inline vector_iterator vend() {
          return value.end();
        }
        inline const_vector_iterator vend() const {
          return value.end();
        }
        inline reverse_vector_iterator rvbegin() {
          return value.rbegin();
        }
        inline const_reverse_vector_iterator rvbegin() const {
          return value.rbegin();
        }
        inline reverse_vector_iterator rvend() {
          return value.rend();
        }
        inline const_reverse_vector_iterator rvend() const {
          return value.rend();
        }
        inline const_vector_iterator cvbegin() const {
          return value.cbegin();
        }
        inline const_vector_iterator cvend() const {
          return value.cend();
        }
        inline const_reverse_vector_iterator crvbegin() const {
          return value.crbegin();
        }
        inline const_reverse_vector_iterator crvend() const {
          return value.crend();
        }
        inline static size_type size() {
          return Size::value;
        }
        inline static size_type global_size() {
          return Size::value;
        }
        inline static size_type group_size() {
          return compiler::vector< T, Size >::group_size;
        }
        inline static size_type local_size() {
          return compiler::vector< T, Size >::local_size;
        }
        inline static size_type empty() {
          return Size::value == 0;
        }
        inline static size_type max_size() {
          return Size::value;
        }
        base_type &get() {
          return value;
        }
        const base_type &get() const {
          return value;
        }
      private:
        base_type value;
      };
      template< typename T, typename Size >
      void swap( vector< T, Size > &left, vector< T, Size > &right ) {
        left.get().swap( right.get() );
      }
    }
    namespace operation {
#define HERMIT_STREAM_SIMD_OPERATION_3( name, oper ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const container::vector< T, Size > &left, \
        const container::vector< T, Size > &right \
      ) { \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator right_cur = right.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator dest_end = dest.vend(); \
        for( ; dest_cur != dest_end; ++dest_cur, ++left_cur, ++right_cur ) \
          dest_cur->packed = left_cur->packed oper right_cur->packed; \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        container::vector< T, Size > &dest, \
        const container::vector< T, Size > &left, \
        const T &right \
      ) { \
        typename compiler::vector< T, Size >::type::value_type right_vec; \
        for( size_t index = 0; index != compiler::internal_vector< T >::length; ++index ) \
          right_vec.unpacked[ index ] = right; \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator dest_end = dest.vend(); \
        for( ; dest_cur != dest_end; ++dest_cur, ++left_cur ) \
          dest_cur->packed = left_cur->packed oper right_vec.packed; \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const T &left, \
        const typename container::vector< T, Size > &right \
      ) { \
        typename compiler::vector< T, Size >::type::value_type left_vec; \
        for( size_t index = 0; index != compiler::internal_vector< T >::length; ++index ) \
          left_vec.unpacked[ index ] = left; \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator right_cur = right.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator dest_end = dest.vend(); \
        for( ; dest_cur != dest_end; ++dest_cur, ++right_cur ) \
          dest_cur->packed = left_vec.packed oper right_cur->packed; \
      }

#define HERMIT_STREAM_SIMD_OPERATION_2( name, oper ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left, \
        const typename container::vector< T, Size > &right \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator right_cur = right.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++left_cur, ++right_cur ) \
          left_cur->packed oper right_cur->packed; \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left, \
        const T &right \
      ) { \
        typename compiler::vector< T, Size >::type::value_type right_vec; \
        for( size_t index = 0; index != compiler::internal_vector< T >::length; ++index ) \
          right_vec.unpacked[ index ] = right; \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++left_cur ) \
          left_cur->packed oper right.packed; \
      }

#define HERMIT_STREAM_SIMD_OPERATION_1PRE( name, oper ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::const_vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++dest_cur, ++left_cur ) \
          dest_cur->packed = oper left_cur->packed; \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++left_cur ) \
          oper left_cur->packed; \
      }

#define HERMIT_STREAM_SIMD_OPERATION_1POST( name, oper ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::const_vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++dest_cur, ++left_cur ) \
          dest_cur->packed = left_cur->packed oper; \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        for( ; left_cur != left_end; ++left_cur ) \
          left_cur->packed oper; \
      }

HERMIT_STREAM_SIMD_OPERATION_3( plus, + )
HERMIT_STREAM_SIMD_OPERATION_2( plus_assign, += )
HERMIT_STREAM_SIMD_OPERATION_3( minus, - )
HERMIT_STREAM_SIMD_OPERATION_2( minus_assign, -= )
HERMIT_STREAM_SIMD_OPERATION_3( multiplies, * )
HERMIT_STREAM_SIMD_OPERATION_2( multiplies_assign, *= )
HERMIT_STREAM_SIMD_OPERATION_3( divides, / )
HERMIT_STREAM_SIMD_OPERATION_2( divides_assign, /= )
HERMIT_STREAM_SIMD_OPERATION_3( modulus, % )
HERMIT_STREAM_SIMD_OPERATION_2( modulus_assign, %= )
HERMIT_STREAM_SIMD_OPERATION_3( equal_to, == )
HERMIT_STREAM_SIMD_OPERATION_3( not_equal_to, != )
HERMIT_STREAM_SIMD_OPERATION_3( greater, > )
HERMIT_STREAM_SIMD_OPERATION_3( less, < )
HERMIT_STREAM_SIMD_OPERATION_3( greater_equal, >= )
HERMIT_STREAM_SIMD_OPERATION_3( less_equal, <= )
HERMIT_STREAM_SIMD_OPERATION_3( shift_right, >> )
HERMIT_STREAM_SIMD_OPERATION_2( shift_right_assign, >>= )
HERMIT_STREAM_SIMD_OPERATION_3( shift_left, << )
HERMIT_STREAM_SIMD_OPERATION_2( shift_left_assign, <<= )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_and, & )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_and_assign, &= )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_or, | )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_or_assign, |= )
HERMIT_STREAM_SIMD_OPERATION_3( logical_and, && )
HERMIT_STREAM_SIMD_OPERATION_3( logical_or, || )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_xor, ^ )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_xor_assign, ^= )
HERMIT_STREAM_SIMD_OPERATION_1PRE( pre_inc, ++ )
HERMIT_STREAM_SIMD_OPERATION_1PRE( pre_dec, -- )
HERMIT_STREAM_SIMD_OPERATION_1POST( post_inc, ++ )
HERMIT_STREAM_SIMD_OPERATION_1POST( post_dec, -- )
HERMIT_STREAM_SIMD_OPERATION_1PRE( unary_plus, + )
HERMIT_STREAM_SIMD_OPERATION_1PRE( negate, - )
HERMIT_STREAM_SIMD_OPERATION_1PRE( complement, ~ )
HERMIT_STREAM_SIMD_OPERATION_1PRE( logical_not, ! )
    }
    namespace wrapper {
    }
  }


  namespace detail {
  }

  template< typename T >
    struct is_vector {
      template< typename U, typename Size >
      static boost::mpl::bool_< true > check( simd::container::vector< U, Size > );
      static boost::mpl::bool_< false > check( ... );
      typedef decltype( is_vector< T >::check( std::declval< T >() ) ) type;
      static constexpr bool value = type::value;
    };

  template< typename T >
    struct vector_length {
      template< typename U, typename Size >
      static Size check( simd::container::vector< U, Size > );
      static boost::mpl::size_t< 1 > check( ... );
      typedef decltype( vector_length< T >::check( std::declval< T >() ) ) type;
      static constexpr bool value = type::value;
    };
  
  template< typename T >
    struct vector_element_type {
      template< typename U, typename Size >
      static typename boost::remove_cv< U >::type check( simd::container::vector< U, Size > );
      static typename boost::remove_cv< T >::type check( ... );
      typedef decltype( vector_element_type< T >::check( std::declval< T >() ) ) type;
    };

  template< typename T, typename U >
    struct is_same_length : public boost::mpl::equal<
      vector_length< T >,
      vector_length< U >
    > {};

  namespace proto = boost::proto;
  using proto::argsns_::list2;
  using proto::exprns_::expr;

  struct placeholder_ {};
  template < typename Index >
    struct placeholder : public placeholder_ {};
#define HERMIT_STREAM_PLACEHOLDER( z, index, unused ) \
  boost::proto::terminal< placeholder< boost::mpl::int_< index > > >::type const BOOST_PP_CAT( _, BOOST_PP_INC( index ) ) = {{}};

BOOST_PP_REPEAT( FUSION_MAX_VECTOR_SIZE, HERMIT_STREAM_PLACEHOLDER, )

  struct sin_ {
    template< typename T, typename Context >
      inline T operator()( const T &value, Context& ) const { return sinf( value ); }
  };
  boost::proto::terminal< sin_ >::type const sin = {{}};
  
  struct floor_ {
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >,
            boost::is_same< float, typename vector_element_type< T >::type >
          >
        >::type* = 0
      ) const {
        for( size_t index = 0; index != ctx.current_buffer->size(); ++index )
          ( *ctx.current_buffer )[ index ] = floorf( source[ index ] );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >,
            boost::is_same< double, typename vector_element_type< T >::type >
          >
        >::type* = 0
      ) const {
        for( size_t index = 0; index != ctx.current_buffer->size(); ++index )
          ( *ctx.current_buffer )[ index ] = floor( source[ index ] );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >,
            boost::is_integral< T >
          >
        >::type* = 0
      ) const {
        return source;
      }
    template< typename T, typename Context >
      inline T operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >,
            boost::is_same< float, T >
          >
        >::type* = 0
      ) const {
        return floorf( source );
      }
    template< typename T, typename Context >
      inline T operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >,
            boost::is_integral< T >
          >
        >::type* = 0
      ) const {
        return floor( source );
      }
    template< typename T, typename Context >
      inline T operator()(
        const T &source,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >,
            boost::is_same< double, T >
          >
        >::type* = 0
      ) const {
        return source;
      }
  };
  boost::proto::terminal< floor_ >::type const floor = {{}};

  struct sampler_ {
    template< typename T, typename U, typename Context >
      inline typename Context::buffer_type &operator()(
        const T &source,
        const U &reference,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< U >,
            is_same_length< U, typename Context::buffer_type >
          >
        >::type* = 0
      ) {
//        for( size_t index = 0; index != ctx.current_buffer->size(); ++index ) {
//          ( *ctx.current_buffer )[ index ] = source[ ( reference[ index ] - static_cast< int >( reference[ index ] ) ) * source.size() ];
          return *ctx.current_buffer;
//        }
      }
/*    template< typename T, typename U, typename Context >
      inline const typename T::value_type &operator()(
        const T &source,
        const U &reference,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< U > >,
            boost::mpl::not_< is_vector< U > >,
            typename Context::buffer_type >,
              vector_length< typename U >
            >
          >
        >::type* = 0
      ) {
//        return source[ ( reference[ index ] ) * source.size() ];
//          return *ctx.current_buffer;
        }
      }*/
    private:
      floor_ floor;
  };

  template< typename ArgumentsType, typename T, typename Size >
    class my_context {
      public:
        typedef simd::container::vector< T, Size > buffer_type;
        typedef my_context< ArgumentsType, T, Size > self_type;
        typedef my_context< ArgumentsType, T, boost::mpl::max< boost::mpl::divides< Size, boost::mpl::int_< 2 > >, boost::mpl::int_< 4 > > > half_type;
      private:
        template< typename ValueType >
        inline static const ValueType &as_value(
          const ValueType &value, self_type&,
          typename boost::disable_if<
            boost::is_convertible<
              placeholder_,
              ValueType
            >
          >::type* = 0
        ) {
          return value;
        }
        template < typename Index >
          inline static buffer_type &as_value( const placeholder< Index >&, self_type &context,
            typename boost::enable_if< boost::is_convertible< buffer_type, decltype( boost::fusion::at< Index >( std::declval< ArgumentsType >() ) ) > >::type* = 0
          ) {
            return boost::fusion::at< Index >( context.arguments );
          }
        template < typename Index >
          inline static auto as_value( const placeholder< Index >&, self_type &context,
            typename boost::disable_if< boost::is_convertible< buffer_type, decltype( boost::fusion::at< Index >( std::declval< ArgumentsType >() ) ) > >::type* = 0
          ) -> decltype( boost::fusion::at< Index >( std::declval< ArgumentsType >() ) ) {
            return boost::fusion::at< Index >( context.arguments );
          }
        template< typename Expr >
        struct both_are_vector : public boost::mpl::and_<
          boost::is_same<
            const buffer_type&,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >,
          boost::is_same<
            const buffer_type&,
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >
        > {};
        template< typename Expr >
        struct only_left_is_vector : public boost::mpl::and_<
          boost::is_same<
            const buffer_type&,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >,
          boost::mpl::not_<
            boost::is_same<
              const buffer_type&,
              decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
            >
          >
        > {};
        template< typename Expr >
        struct only_right_is_vector : public boost::mpl::and_<
          boost::mpl::not_<
            boost::is_same<
              const buffer_type&,
              decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
            >
          >,
          boost::is_same<
            const buffer_type&,
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >
        > {};
        template< typename Expr >
        struct neither_is_vector : public boost::mpl::and_<
          boost::mpl::not_<
            boost::is_convertible<
              const buffer_type&,
              decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
            >
          >,
          boost::mpl::not_<
            boost::is_convertible<
              const buffer_type&,
              decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
            >
          >
        > {};
        template< typename Expr >
        struct is_vector : public
          boost::is_convertible<
            const buffer_type&,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
        > {};
        template< typename Expr >
        struct is_not_vector : public boost::mpl::not_<
          boost::is_convertible<
            const buffer_type&,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >
        > {};

#define HERMIT_STREAM_CONTEXT_OPERATION_3( name, oper ) \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< both_are_vector< Expr > >::type* = 0 \
          ) { \
            const buffer_type &left = proto::eval( boost::proto::left( e ), ctx ); \
            buffer_type temporary_buffer; \
            buffer_type *temporary_buffer_addr = &temporary_buffer; \
            std::swap( ctx.current_buffer, temporary_buffer_addr ); \
            const buffer_type &right = proto::eval( boost::proto::right( e ), ctx ); \
            std::swap( ctx.current_buffer, temporary_buffer_addr ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, left, right ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< only_left_is_vector< Expr > >::type* = 0 \
          ) { \
            const buffer_type &left = proto::eval( boost::proto::left( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, left, proto::eval( boost::proto::right( e ), ctx ) ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< only_right_is_vector< Expr > >::type* = 0 \
          ){ \
            const buffer_type &right = proto::eval( boost::proto::right( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, proto::eval( boost::proto::left( e ), ctx ), right ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static auto name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< neither_is_vector< Expr > >::type* = 0 \
          ) -> decltype( proto::eval( boost::proto::left( e ), ctx ) oper proto::eval( boost::proto::right( e ), ctx ) ) { \
            return proto::eval( boost::proto::left( e ), ctx ) oper proto::eval( boost::proto::right( e ), ctx ); \
          }
#define HERMIT_STREAM_CONTEXT_OPERATION_1PRE( name, oper ) \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< is_vector< Expr > >::type* = 0 \
          ) { \
            const auto &value = proto::eval( boost::proto::left( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, value ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static auto name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< is_not_vector< Expr > >::type* = 0 \
          ) -> decltype( oper proto::eval( boost::proto::left( e ), ctx ) ) { \
            return oper proto::eval( boost::proto::left( e ), ctx ); \
          }
#define HERMIT_STREAM_CONTEXT_OPERATION_1POST( name, oper ) \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< is_vector< Expr > >::type* = 0 \
          ) { \
            const buffer_type value = proto::eval( boost::proto::left( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, value ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static auto name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< is_not_vector< Expr > >::type* = 0 \
          ) -> decltype( proto::eval( boost::proto::left( e ), ctx ) oper ) { \
            return proto::eval( boost::proto::left( e ), ctx ) oper; \
          }
HERMIT_STREAM_CONTEXT_OPERATION_3( plus, + )
HERMIT_STREAM_CONTEXT_OPERATION_3( minus, - )
HERMIT_STREAM_CONTEXT_OPERATION_3( multiplies, * )
HERMIT_STREAM_CONTEXT_OPERATION_3( divides, / )
HERMIT_STREAM_CONTEXT_OPERATION_3( modulus, % )
HERMIT_STREAM_CONTEXT_OPERATION_3( equal_to, == )
HERMIT_STREAM_CONTEXT_OPERATION_3( not_equal_to, != )
HERMIT_STREAM_CONTEXT_OPERATION_3( bitwise_and, & )
HERMIT_STREAM_CONTEXT_OPERATION_3( bitwise_or, | )
HERMIT_STREAM_CONTEXT_OPERATION_3( bitwise_xor, ^ )
HERMIT_STREAM_CONTEXT_OPERATION_3( logical_and, && )
HERMIT_STREAM_CONTEXT_OPERATION_3( logical_or, || )
HERMIT_STREAM_CONTEXT_OPERATION_3( greater, > )
HERMIT_STREAM_CONTEXT_OPERATION_3( less, < )
HERMIT_STREAM_CONTEXT_OPERATION_3( greater_equal, >= )
HERMIT_STREAM_CONTEXT_OPERATION_3( less_equal, <= )
HERMIT_STREAM_CONTEXT_OPERATION_3( shift_left, << )
HERMIT_STREAM_CONTEXT_OPERATION_3( shift_right, >> )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( unary_plus, + )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( negate, - )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( complement, ~ )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( logical_not, ! )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( pre_inc, ++ )
HERMIT_STREAM_CONTEXT_OPERATION_1PRE( pre_dec, -- )
HERMIT_STREAM_CONTEXT_OPERATION_1POST( post_inc, ++ )
HERMIT_STREAM_CONTEXT_OPERATION_1POST( post_dec, -- )
      public:
        my_context( const ArgumentsType &args ) : arguments( args ), current_buffer( &origin_buffer ) {}
        template< typename Expr, typename  Enable = void > struct eval {};
        template<
          class Expr
          > struct eval<
          Expr,
          typename boost::enable_if<
              proto::matches< Expr, proto::terminal< proto::_ > >
          >::type
        > {
              typedef decltype( self_type::as_value( boost::proto::value( std::declval< Expr >() ), std::declval<self_type&>() ) ) result_type;
              inline result_type operator()( Expr &e, self_type &context ) {
                return self_type::as_value( boost::proto::value( e ), context );
              }
            };
#define HERMIT_STREAM_CONTEXT_FUNCTION_PROTO_( z, index, unused ) \
        proto::_

#define HERMIT_STREAM_CONTEXT_FUNCTION_ARGUMENTS_DECLTYPE( z, index, unused ) \
              proto::eval( \
                boost::proto::child< boost::mpl::int_< index > >( \
                  std::declval< Expr& >() \
                ), \
                std::declval< self_type& >() \
              ),

#define HERMIT_STREAM_CONTEXT_FUNCTION_ARGUMENTS( z, index, unused ) \
              proto::eval( \
                boost::proto::child< boost::mpl::int_< index > >( \
                  e \
                ), \
                ctx \
              ),

#define HERMIT_STREAM_CONTEXT_FUNCTION( z, arg_num, unused ) \
        template< \
          class Expr \
          > struct eval< \
          Expr, \
          typename boost::enable_if< \
              proto::matches< Expr, proto::function< BOOST_PP_ENUM( arg_num, HERMIT_STREAM_CONTEXT_FUNCTION_PROTO_, ) > > \
          >::type \
        > { \
          typedef decltype( \
            proto::eval( \
              boost::proto::left( \
                std::declval< Expr& >() \
              ), \
              std::declval< self_type& >() \
            )( \
              BOOST_PP_REPEAT_FROM_TO( 1, arg_num, HERMIT_STREAM_CONTEXT_FUNCTION_ARGUMENTS_DECLTYPE, ) \
              std::declval< self_type& >() \
            ) \
          ) result_type; \
          inline result_type operator()( Expr &e, self_type &ctx ) { \
            return proto::eval( boost::proto::left( e ), ctx )( \
              BOOST_PP_REPEAT_FROM_TO( 1, arg_num, HERMIT_STREAM_CONTEXT_FUNCTION_ARGUMENTS, ) \
              ctx );\
              } \
            };

BOOST_PP_REPEAT_FROM_TO( 1, 11, HERMIT_STREAM_CONTEXT_FUNCTION, )

#define HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( tag ) \
        template< \
          class Expr \
          > struct eval< \
          Expr, \
          typename boost::enable_if< \
            proto::matches< Expr, proto:: tag < proto::_, proto::_ > > \
          >::type \
        > { \
          typedef decltype( self_type:: tag ( std::declval< Expr& >(), std::declval< self_type& >() ) ) result_type; \
          inline result_type operator()( Expr &e, self_type &ctx ) { \
            return self_type:: tag ( e, ctx ); \
          } \
        };
#define HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( tag ) \
        template< \
          class Expr \
          > struct eval< \
          Expr, \
          typename boost::enable_if< \
            proto::matches< Expr, proto:: tag < proto::_ > > \
          >::type \
        > { \
          typedef decltype( self_type:: tag ( std::declval< Expr& >(), std::declval< self_type& >() ) ) result_type; \
          inline result_type operator()( Expr &e, self_type &ctx ) { \
            return self_type:: tag ( e, ctx ); \
          } \
        };

HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( plus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( minus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( multiplies )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( divides )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( modulus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( equal_to )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( not_equal_to )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( bitwise_and )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( bitwise_or )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( bitwise_xor )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( logical_and )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( logical_or )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( greater )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( less )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( greater_equal )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( less_equal )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( shift_left )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_3( shift_right )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( unary_plus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( negate )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( complement )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( logical_not )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( pre_inc )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( pre_dec )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( post_inc )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_1( post_dec )

        const buffer_type &get_origin_buffer() const {
          return origin_buffer;
        }
      private:
        ArgumentsType arguments;
        buffer_type origin_buffer;
      public:
        buffer_type *current_buffer;
    };
}

int main() {
  std::cout << stream::is_vector< float >::value << std::endl;
  std::cout << stream::is_vector< stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::value << std::endl;
  std::cout << stream::is_same_length< stream::simd::container::vector< float, boost::mpl::size_t< 64 > >, stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::value << std::endl;
  std::cout << boost::is_same< float, typename stream::vector_element_type< stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::type >::value << std::endl;
  stream::simd::container::vector< float, boost::mpl::size_t< 64 > > woo = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  boost::fusion::vector< int, int, int, stream::simd::container::vector< float, boost::mpl::size_t< 64 > > > args( 1, 2, 10, woo );
  stream::my_context< decltype( args ), float, boost::mpl::size_t< 64 > > ctx( args );
//  boost::proto::eval( stream::pass_through( boost::proto::lit( 1 ) + woo + stream::_4 - 20 ), ctx );
  boost::proto::eval( stream::floor( stream::sin( boost::proto::lit( 0.1f ) ) + woo ) + stream::_4 - 20, ctx );
  for( const auto &elem: ctx.get_origin_buffer() )
    std::cout << elem << " ";
  std::cout << std::endl;
  {
    typedef stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec_type;
    vec_type vec2 = { 5, 4, 3, 2, 1, 0, -1, -2, -3, -4 };
    vec_type vec3 = { 2, 5, 8, 1, 4, 9, 0, 3, 6, 7 };
    boost::fusion::vector2< vec_type, vec_type > args( vec2, vec3 );
    stream::my_context< decltype( args ), float, boost::mpl::size_t< 16384 > > ctx( args );
    auto expr = vec3 - ( vec2 + stream::_2 );
//    auto expr = vec2 - ( boost::proto::lit( vec2 ) + vec3 );
    auto start = std::chrono::high_resolution_clock::now();
    for( int count = 0; count != 100000; ++count ) {
      boost::proto::eval( expr, ctx );
    }
    auto end = std::chrono::high_resolution_clock::now();
    double d = std::chrono::duration<double>(end - start).count();
    std::cout << d << std::endl;
  }
  {
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec2 = { 5, 4, 3, 2, 1, 0, -1, -2, -3, -4 };
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec3 = { 2, 5, 8, 1, 4, 9, 0, 3, 6, 7 };
    auto start = std::chrono::high_resolution_clock::now();
    for( int count = 0; count != 100000; ++count ) {
      stream::simd::operation::plus( vec1, vec2, vec3 );
      stream::simd::operation::minus( vec2, vec2, vec1 );
    }
    auto end = std::chrono::high_resolution_clock::now();
    double d = std::chrono::duration<double>(end - start).count();
    std::cout << d << std::endl;
    std::cout << "debug: " << vec1[ 3 ] << std::endl;
    std::cout << "debug: " << vec2[ 3 ] << std::endl;
  }
  { 
    std::array< float, 16384 > vec3 = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};
    std::array< float, 16384 > vec4 = {{ 5, 4, 3, 2, 1, 0, -1, -2, -3, -4 }};
    std::array< float, 16384 > vec5 = {{ 2, 5, 8, 1, 4, 9, 0, 3, 6, 7 }};
    auto start = std::chrono::high_resolution_clock::now();
    for( int count = 0; count != 100000; ++count )
      for( int i = 0; i != 16384; ++i ) {
        vec3[ i ] = vec4[ i ] + vec5[ i ];
        vec4[ i ] = vec4[ i ] - vec3[ i ];
      }
    auto end = std::chrono::high_resolution_clock::now();
    double d = std::chrono::duration<double>(end - start).count();
    std::cout << d << std::endl;
    std::cout << "debug: " << vec3[ 3 ] << std::endl;
    std::cout << "debug: " << vec4[ 3 ] << std::endl;
  }
}
