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
#include <boost/mpl/shift_left.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/ref.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/swap.hpp>
#include <boost/type_traits.hpp>
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
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
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
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
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

HERMIT_STREAM_SIMD_OPERATION_3( add, + )
HERMIT_STREAM_SIMD_OPERATION_2( add, += )
HERMIT_STREAM_SIMD_OPERATION_3( sub, - )
HERMIT_STREAM_SIMD_OPERATION_2( sub, -= )
HERMIT_STREAM_SIMD_OPERATION_3( mul, * )
HERMIT_STREAM_SIMD_OPERATION_2( mul, *= )
HERMIT_STREAM_SIMD_OPERATION_3( div, / )
HERMIT_STREAM_SIMD_OPERATION_2( div, /= )
HERMIT_STREAM_SIMD_OPERATION_3( eq, == )
HERMIT_STREAM_SIMD_OPERATION_3( neq, != )
HERMIT_STREAM_SIMD_OPERATION_3( gt, > )
HERMIT_STREAM_SIMD_OPERATION_3( lt, < )
HERMIT_STREAM_SIMD_OPERATION_3( ge, >= )
HERMIT_STREAM_SIMD_OPERATION_3( le, <= )
HERMIT_STREAM_SIMD_OPERATION_3( rshift, >> )
HERMIT_STREAM_SIMD_OPERATION_2( rshift, >>= )
HERMIT_STREAM_SIMD_OPERATION_3( lshift, << )
HERMIT_STREAM_SIMD_OPERATION_2( lshift, <<= )
HERMIT_STREAM_SIMD_OPERATION_3( and_, & )
HERMIT_STREAM_SIMD_OPERATION_2( and_, &= )
HERMIT_STREAM_SIMD_OPERATION_3( or_, | )
HERMIT_STREAM_SIMD_OPERATION_2( or_, |= )
HERMIT_STREAM_SIMD_OPERATION_3( xor_, ^ )
HERMIT_STREAM_SIMD_OPERATION_2( xor_, ^= )
HERMIT_STREAM_SIMD_OPERATION_1PRE( inc_pre, ++ )
HERMIT_STREAM_SIMD_OPERATION_1PRE( dec_pre, -- )
HERMIT_STREAM_SIMD_OPERATION_1POST( inc_post, ++ )
HERMIT_STREAM_SIMD_OPERATION_1POST( dec_post, -- )
HERMIT_STREAM_SIMD_OPERATION_1PRE( pos, + )
HERMIT_STREAM_SIMD_OPERATION_1PRE( neg, - )
HERMIT_STREAM_SIMD_OPERATION_1PRE( not_, ~ )
    }
    namespace wrapper {
    }
  }

  namespace proto = boost::proto;
  using proto::argsns_::list2;
  using proto::exprns_::expr;

  template < typename Index >
    struct placeholder {};
  boost::proto::terminal< placeholder< boost::mpl::int_< 0 > > >::type const _1 = {{}};
  boost::proto::terminal< placeholder< boost::mpl::int_< 1 > > >::type const _2 = {{}};
  boost::proto::terminal< placeholder< boost::mpl::int_< 2 > > >::type const _3 = {{}};
  boost::proto::terminal< placeholder< boost::mpl::int_< 3 > > >::type const _4 = {{}};
  boost::proto::terminal< placeholder< boost::mpl::int_< 4 > > >::type const _5 = {{}};
  boost::proto::terminal< placeholder< boost::mpl::int_< 5 > > >::type const _6 = {{}};


  template< typename ArgumentsType, typename T, typename Size >
    class my_context {
      public:
        typedef typename simd::container::vector< T, Size > buffer_type;
        typedef my_context< ArgumentsType, T, Size > self_type;
      private:
        static int as_value( int value, self_type & ) {
          return value;
        }
        static unsigned int as_value( unsigned int value, self_type & ) {
          return value;
        }
        static float as_value( float value, self_type & ) {
          return value;
        }
        static double as_value( double value, self_type & ) {
          return value;
        }
        static void as_value( simd::container::vector< T, Size > value, self_type &ctx ) {
          ctx.origin_buffer = value;
        }
        template < typename Index >
          static auto as_value( const placeholder< Index >&, self_type &context ) -> decltype( boost::fusion::at< Index >( std::declval< ArgumentsType >() ) ) {
            return boost::fusion::at< Index >( context.arguments );
          }
        template< typename Expr >
        struct both_are_vector : public boost::mpl::and_<
          boost::is_same<
            void,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< my_context& >() ) )
          >,
          boost::is_same<
            void,
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< my_context& >() ) )
          >
        > {};
        template< typename Expr >
        struct only_left_is_vector : public boost::mpl::and_<
          boost::is_same<
            void,
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< my_context& >() ) )
          >,
          boost::mpl::not_<
            boost::is_same<
              void,
              decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< my_context& >() ) )
            >
          >
        > {};
        template< typename Expr >
        struct only_right_is_vector : public boost::mpl::and_<
          boost::mpl::not_<
            boost::is_same<
              void,
              decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< my_context& >() ) )
            >
          >,
          boost::is_same<
            void,
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< my_context& >() ) )
          >
        > {};
        template< typename Expr >
        struct neither_is_vector : public boost::mpl::and_<
          boost::mpl::not_<
            boost::is_convertible<
              void,
              decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< my_context& >() ) )
            >
          >,
          boost::mpl::not_<
            boost::is_convertible<
              void,
              decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< my_context& >() ) )
            >
          >
        > {};
        template< typename Expr >
          static void add(
            Expr &e, self_type &ctx,
            typename boost::enable_if< both_are_vector< Expr > >::type* = 0
          ) {
            proto::eval( boost::proto::left( e ), ctx );
            buffer_type temporary_buffer;
            boost::swap( ctx.origin_buffer, temporary_buffer );
            proto::eval( boost::proto::right( e ), ctx );
            simd::operation::add< T, Size >( ctx.origin_buffer, temporary_buffer, ctx.origin_buffer );
          }
        template< typename Expr >
          static void add(
            Expr &e, self_type &ctx,
            typename boost::enable_if< only_left_is_vector< Expr > >::type* = 0
          ) {
            proto::eval( boost::proto::left( e ), ctx );
            simd::operation::add< T, Size >( ctx.origin_buffer, ctx.origin_buffer, proto::eval( boost::proto::right( e ), ctx ) );
          }
        template< typename Expr >
          static void add(
            Expr &e, self_type &ctx,
            typename boost::enable_if< only_right_is_vector< Expr > >::type* = 0
          ){
            proto::eval( boost::proto::right( e ), ctx );
            simd::operation::add< T, Size >( ctx.origin_buffer, proto::eval( boost::proto::left( e ), ctx ), ctx.origin_buffer );
          }
        template< typename Expr >
          static auto add(
            Expr &e, self_type &ctx,
            typename boost::enable_if< neither_is_vector< Expr > >::type* = 0
          ) -> decltype( proto::eval( boost::proto::left( e ), ctx ) + proto::eval( boost::proto::right( e ), ctx ) ) {
          return proto::eval( boost::proto::left( e ), ctx ) + proto::eval( boost::proto::right( e ), ctx );
          }


        template< typename Expr >
          static void sub(
            Expr e, self_type &ctx,
            typename boost::enable_if< both_are_vector< Expr > >::type* = 0
          ) {
            proto::eval( boost::proto::left( e ), ctx );
            buffer_type temporary_buffer;
            boost::swap( ctx.origin_buffer, temporary_buffer );
            proto::eval( boost::proto::right( e ), ctx );
            simd::operation::sub< T, Size >( ctx.origin_buffer, temporary_buffer, ctx.origin_buffer );
          }
        template< typename Expr >
          static void sub(
            Expr e, self_type &ctx,
            typename boost::enable_if< only_left_is_vector< Expr > >::type* = 0
          ) {
            proto::eval( boost::proto::right( e ), ctx );
            simd::operation::sub< T, Size >( ctx.origin_buffer, proto::eval( boost::proto::left( e ), ctx ), ctx.origin_buffer );
          }
        template< typename Expr >
          static void sub(
            Expr e, self_type &ctx,
            typename boost::enable_if< only_right_is_vector< Expr > >::type* = 0
          ) {
            proto::eval( boost::proto::left( e ), ctx );
            simd::operation::sub< T, Size >( ctx.origin_buffer, ctx.origin_buffer, proto::eval( boost::proto::right( e ), ctx ) );
          }
        template< typename Expr >
          static auto sub(
            Expr e, self_type &ctx,
            typename boost::enable_if< neither_is_vector< Expr > >::type* = 0
          ) -> decltype( proto::eval( boost::proto::left( e ), ctx ) + proto::eval( boost::proto::right( e ), ctx ) ) {
          return proto::eval( boost::proto::left( e ), ctx ) - proto::eval( boost::proto::right( e ), ctx );
          }
      
      
      public:
        my_context( const ArgumentsType &args ) : arguments( args ) {}
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
              result_type operator()( Expr &e, self_type &context ) { 
                return self_type::as_value( boost::proto::value( e ), context );
              }
            };
        template<
          class Expr
          > struct eval<
          Expr,
          typename boost::enable_if<
            proto::matches< Expr, proto::plus< proto::_, proto::_ > >
          >::type 
        > {
          typedef decltype( self_type::add( std::declval< Expr& >(), std::declval< self_type& >() ) ) result_type;
          result_type operator()( Expr &e, self_type &ctx ) {
            return self_type::add( e, ctx );
          }
        };
        template<
          class Expr
          > struct eval<
          Expr,
          typename boost::enable_if<
            proto::matches< Expr, proto::minus< proto::_, proto::_ > >
          >::type 
        > {
          typedef decltype( self_type::sub( std::declval< Expr& >(), std::declval< self_type& >() ) ) result_type;
          result_type operator()( Expr &e, self_type &ctx ) {
            return self_type::sub( e, ctx );
          }
        };
      private:
        const ArgumentsType arguments;
        buffer_type origin_buffer;
    };
}

int main() {
  stream::simd::container::vector< float, boost::mpl::size_t< 64 > > woo;
  boost::fusion::vector< int, int, int, stream::simd::container::vector< float, boost::mpl::size_t< 64 > > > args( 1, 2, 10, woo );
  stream::my_context< decltype( args ), float, boost::mpl::size_t< 64 > > ctx( args );
  boost::proto::eval( boost::proto::lit( 1 ) + woo, ctx );
  stream::simd::compiler::vector< float, boost::mpl::size_t< 64 > >::type foo;
  {
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec2 = { 5, 4, 3, 2, 1, 0, -1, -2, -3, -4 };
    stream::simd::container::vector< float, boost::mpl::size_t< 16384 > > vec3 = { 2, 5, 8, 1, 4, 9, 0, 3, 6, 7 };
    auto start = std::chrono::high_resolution_clock::now();
    for( int count = 0; count != 100000; ++count ) {
      stream::simd::operation::add( vec1, vec2, vec3 );
      stream::simd::operation::sub( vec2, vec2, vec1 );
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
/*  stream::my_context< decltype( args ), float, boost::mpl::size_t< 64 > > ctx( args );
  boost::proto::eval( boost::proto::lit( 1 ) + 3 + stream::_3, ctx );
  stream::simd::compiler::vector< float, boost::mpl::size_t< 64 > >::type foo;
  {
    stream::simd::wrapper::vector< float, boost::mpl::size_t< 64 > > foow( foo );
    std::fill( foow.begin(), foow.end(), 3.0f );
  }
  stream::simd::compiler::vector< float, boost::mpl::size_t< 64 > >::type bar;
  {
    stream::simd::wrapper::vector< float, boost::mpl::size_t< 64 > > barw( bar );
    std::fill( barw.begin(), barw.end(), 2.0f );
  }
  stream::simd::operation::add< float, boost::mpl::size_t< 64 > >( foo, foo, foo );
  stream::simd::operation::mul< float, boost::mpl::size_t< 64 > >( foo, foo, bar );
  {
    stream::simd::wrapper::const_vector< float, boost::mpl::size_t< 64 > > foow( foo );
    for( const auto &elem: foow )
      std::cout << elem << std::endl;
  }*/
}
