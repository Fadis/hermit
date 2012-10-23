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
namespace stream {
  namespace simd {
    namespace arch {
      template< typename T >
      struct fast_packed_size : public boost::mpl::size_t< 1 > {};
#if defined(__AVX__)
      template<>
      struct fast_packed_size< float > : public boost::mpl::size_t< 8 > {};
      template<>
      struct fast_packed_size< double > : public boost::mpl::size_t< 4 > {};
#elif defined(__SSE2__)
      template<>
      struct fast_packed_size< float > : public boost::mpl::size_t< 4 > {};
      template<>
      struct fast_packed_size< double > : public boost::mpl::size_t< 2 > {};
#elif defined(__SSE__)
      template<>
      struct fast_packed_size< float > : public boost::mpl::size_t< 1 > {};
#endif
#if defined(__MMX__)
      template<>
      struct fast_packed_size< std::int32_t > : public boost::mpl::size_t< 4 > {};
      template<>
      struct fast_packed_size< std::uint32_t > : public boost::mpl::size_t< 4 > {};
#endif
    }

    namespace compiler {
#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )

#define HERMIT_VECTOR_TYPE_IS_AVAILABLE
      namespace detail {
        template< typename T, typename U = boost::mpl::int_< 1 > >
          struct smallest_pot
          : public boost::mpl::if_<
            boost::mpl::bool_< T::value <= U::value >,
            U,
            smallest_pot< T, boost::mpl::shift_left< U, boost::mpl::int_< 1 > > >
            >::type {};

        template< typename T, typename Size >
          struct vector {
            typedef T type __attribute( ( vector_size( smallest_pot< Size >::value * sizeof( T ) ) ) );
          };
      }
      template< typename T, typename Size >
        struct vector {
          static constexpr unsigned int group_size = Size::value / arch::fast_packed_size< T >::value +
            ( ( Size::value % arch::fast_packed_size< T >::value ) ? 1 : 0 );
          static constexpr unsigned int local_size = arch::fast_packed_size< T >::value;
          typedef typename detail::vector< T, arch::fast_packed_size< T > >::type packed_element;
          union accessible_element {
            packed_element packed;
            T unpacked[ local_size ];
          };
          typedef std::array<
            accessible_element,
            group_size
          > type;
        };
#endif
#endif
    }
    namespace operation {
      template<typename T, typename Size>
      T &get_scalar(
        typename compiler::vector< T, Size >::type &src,
        size_t index
      ) {
        return src[ index / compiler::vector< T, Size >::local_size ].unpacked[ index % compiler::vector< T, Size >::local_size ];
      }
      template<typename T, typename Size>
      const T &get_scalar(
        const typename compiler::vector< T, Size >::type &src,
        size_t index
      ) {
        return src[ index / compiler::vector< T, Size >::local_size ].unpacked[ index % compiler::vector< T, Size >::local_size ];
      }
      template<typename T, typename Size>
      void add(
        typename compiler::vector< T, Size >::type &dest,
        const typename compiler::vector< T, Size >::type &left,
        const typename compiler::vector< T, Size >::type &right
      ) {
        for( unsigned int index = 0; index != compiler::vector< T, Size >::group_size; ++index )
          dest[ index ].packed = left[ index ].packed + right[ index ].packed;
      }
      template<typename T, typename Size>
      void sub(
        typename compiler::vector< T, Size >::type &dest,
        const typename compiler::vector< T, Size >::type &left,
        const typename compiler::vector< T, Size >::type &right
      ) {
        for( unsigned int index = 0; index != compiler::vector< T, Size >::group_size; ++index )
          dest[ index ].packed = left[ index ].packed - right[ index ].packed;
      }
      template<typename T, typename Size>
      void mul(
        typename compiler::vector< T, Size >::type &dest,
        const typename compiler::vector< T, Size >::type &left,
        const typename compiler::vector< T, Size >::type &right
      ) {
        for( unsigned int index = 0; index != compiler::vector< T, Size >::group_size; ++index )
          dest[ index ].packed = left[ index ].packed * right[ index ].packed;
      }
      template<typename T, typename Size>
      void div(
        typename compiler::vector< T, Size >::type &dest,
        const typename compiler::vector< T, Size >::type &left,
        const typename compiler::vector< T, Size >::type &right
      ) {
        for( unsigned int index = 0; index != compiler::vector< T, Size >::group_size; ++index )
          dest[ index ].packed = left[ index ].packed / right[ index ].packed;
      }
    }
    namespace wrapper {
    template< typename T, typename Size >
    class const_vector {
      public:
        static const size_t local_size_ = arch::fast_packed_size< T >::value;
        class const_iterator : public boost::iterator_facade<
          const_iterator,
          const T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef const_iterator this_type;
          typedef typename compiler::vector< T, Size >::type base_type;
          const_iterator( const base_type *base_, size_t index_ = 0 ) : base( base_ ), index( index_ ) {}
          const T& dereference() const {
            return operation::get_scalar< T, Size >( *base, index );
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
          ptrdiff_t distance_to( const this_type &comp ) const {
            return comp.index - index;
          }
        private:
          const base_type *base;
          size_t index;
        };
        typedef const_vector< T, Size > self_type;
        typedef typename compiler::vector< T, Size >::type base_type;
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef const T& reference;
        typedef const T& const_reference;
        typedef const T* pointer;
        typedef const T* const_pointer;
        typedef const_iterator iterator;
        typedef boost::reverse_iterator< iterator > reverse_iterator;
        typedef boost::reverse_iterator< const_iterator > const_reverse_iterator;
        const_vector( const base_type &value_ ) : value( value_ ) {}
        const_reference operator[]( unsigned int index ) const {
          return operation::get_scalar< T, Size >( value, index );
        }
        const_reference at( unsigned int index ) const {
          if( index >= size() )
            throw std::out_of_range();
          return operation::get_scalar< T, Size >( value, index );
        }
        const_reference front() const {
          return get_scalar( value, 0 );
        }
        const_reference back() const {
          return get_scalar( value, size() - 1 );
        }
        const_iterator begin() const {
          return const_iterator( &value, 0 );
        }
        const_iterator end() const {
          return const_iterator( &value, size() );
        }
        const_reverse_iterator rbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        const_reverse_iterator rend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        const_iterator cbegin() const {
          return const_iterator( &value, 0 );
        }
        const_iterator cend() const {
          return const_iterator( &value, size() );
        }
        const_reverse_iterator crbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        const_reverse_iterator crend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        static size_type size() {
          return Size::value;
        }
        static bool empty() {
          return Size::value == 0;
        }
        static size_type max_size() {
          return Size::value;
        }
      private:
        const base_type &value;
    };
    template< typename T, typename Size >
    class vector {
      public:
        static const size_t local_size_ = arch::fast_packed_size< T >::value;
        class const_iterator : public boost::iterator_facade<
          const_iterator,
          const T&,
          boost::random_access_traversal_tag
        > {
        public:
          typedef const_iterator this_type;
          typedef const typename compiler::vector< T, Size >::type base_type;
          const_iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          const T& dereference() const {
            return operation::get_scalar< T, Size >( *base, index );
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
          ptrdiff_t distance_to( const this_type &comp ) const {
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
          iterator( base_type *base_, int index_ = 0 ) : base( base_ ), index( index_ ) {}
          T& dereference() const {
            return operation::get_scalar< T, Size >( *base, index );
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
          ptrdiff_t distance_to( const this_type &comp ) const {
            return comp.index - index;
          }
          operator const_iterator () const {
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
        vector( base_type &value_ ) : value( value_ ) {}
        reference operator[]( unsigned int index ) {
          return operation::get_scalar< T, Size >( value, index );
        }
        const_reference operator[]( unsigned int index ) const {
          return operation::get_scalar< T, Size >( value, index );
        }
        reference at( unsigned int index ) {
          if( index >= size() )
            throw std::out_of_range();
          return operation::get_scalar< T, Size >( value, index );
        }
        const_reference at( unsigned int index ) const {
          if( index >= size() )
            throw std::out_of_range();
          return operation::get_scalar< T, Size >( value, index );
        }
        reference front() {
          return get_scalar( value, 0 );
        }
        reference back() {
          return get_scalar( value, size() - 1 );
        }
        const_reference front() const {
          return get_scalar( value, 0 );
        }
        const_reference back() const {
          return get_scalar( value, size() - 1 );
        }
        iterator begin() {
          return iterator( &value, 0 );
        }
        const_iterator begin() const {
          return const_iterator( &value, 0 );
        }
        reverse_iterator rbegin() {
          return reverse_iterator( iterator( &value, size() ) );
        }
        const_reverse_iterator rbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        const_iterator cbegin() const {
          return const_iterator( &value, 0 );
        }
        const_reverse_iterator crbegin() const {
          return const_reverse_iterator( const_iterator( &value, size() ) );
        }
        iterator end() {
          return iterator( &value, size() );
        }
        const_iterator end() const {
          return const_iterator( &value, size() );
        }
        reverse_iterator rend() {
          return reverse_iterator( iterator( &value, 0 ) );
        }
        const_reverse_iterator rend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        const_iterator cend() const {
          return const_iterator( &value, size() );
        }
        const_reverse_iterator crend() const {
          return const_reverse_iterator( const_iterator( &value, 0 ) );
        }
        static size_type size() {
          return Size::value;
        }
        static size_type empty() {
          return Size::value == 0;
        }
        static size_type max_size() {
          return Size::value;
        }
        operator const_vector< T, Size > () const {
          return const_vector< T, Size >( value );
        }
      private:
        base_type &value;
      };
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


  template< typename ArgumentsType, typename BufferType >
    class my_context {
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
              typedef void result_type;
              result_type operator()( Expr &e, const my_context< ArgumentsType, BufferType > &context ) {
                my_context< ArgumentsType, BufferType >::as_value( boost::proto::value( e ), context );
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
              typedef void result_type;
              result_type operator()( Expr &e, const my_context< ArgumentsType, BufferType > &ctx ) {
                proto::eval( boost::proto::left( e ), ctx );
                proto::eval( boost::proto::right( e ), ctx );
              }
            };
      private:
        static int as_value( int value, const my_context< ArgumentsType, BufferType > & ) {
          return value;
        }
        static unsigned int as_value( unsigned int value, const my_context< ArgumentsType, BufferType > & ) {
          return value;
        }
        static float as_value( float value, const my_context< ArgumentsType, BufferType > & ) {
          return value;
        }
        static double as_value( double value, const my_context< ArgumentsType, BufferType > & ) {
          return value;
        }
        template < typename Index >
          static auto as_value( const placeholder< Index >&, const my_context< ArgumentsType, BufferType > &context ) -> decltype( boost::fusion::at< Index >( std::declval< ArgumentsType >() ) ) {
            return boost::fusion::at< Index >( context.arguments );
          }
        const ArgumentsType arguments;
        BufferType origin_buffer;
        BufferType *current_buffer;
    };
}

int main() {
  boost::fusion::vector< int, int, int > args( 1, 2, 10 );
  stream::my_context< decltype( args ), stream::simd::compiler::vector< float, boost::mpl::size_t< 64 > >::type > ctx( args );
  boost::proto::eval( boost::proto::lit( 1 ) + 2 + stream::_3, ctx );
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
  }
}
