#include <stdexcept>
#include <chrono>
#include <vector>
#include <array>
#include <cfenv>
#include <boost/fusion/container/vector.hpp>
#include <boost/proto/proto.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/range.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/ref.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/swap.hpp>
#include <boost/type_traits.hpp>
#include <boost/preprocessor.hpp>
namespace stream {
  namespace simd {
    namespace compiler {
      template< typename T >
      struct internal_vector {
        static constexpr bool vectorized = false;
        typedef T type;
        typedef int index_type;
        static constexpr size_t length = 1;
      };
      template< typename T, typename direction >
      struct approx {
        approx() {}
        inline void operator()( T &buffer, const T &src ) {
          const auto old_mode = fegetround();
          fesetround( direction::value );
          buffer = static_cast< T >( static_cast< int >( src ) );
          fesetround( old_mode );
        }
        inline void operator()( int &buffer, const T &src ) {
          const auto old_mode = fegetround();
          fesetround( direction::value );
          buffer = static_cast< int >( src );
          fesetround( old_mode );
        }
      };
      template< typename T, bool vectorized = stream::simd::compiler::internal_vector< T >::vectorized >
      struct at {};
      template< typename T >
      struct at< T, false > {
        at() {}
        inline T& operator()( typename internal_vector< T >::type &src, size_t ) {
          return src;
        }
        inline const T& operator()( const typename internal_vector< T >::type &src, size_t ) {
          return src;
        }
      };
#if defined( __GNUC__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
      template< typename T >
      struct at< T, true > {
        at() {}
        inline T& operator()( typename internal_vector< T >::type &src, size_t index ) const {
          return reinterpret_cast< const T* >( src )[ index ];
        }
        inline const T& operator()( const typename internal_vector< T >::type &src, size_t index ) const {
          return reinterpret_cast< const T* >( src )[ index ];
        }
      };
#endif
      template< typename T, bool vectorized = stream::simd::compiler::internal_vector< T >::vectorized >
      struct fill {};
      template< typename T >
      struct fill< T, false > {
        fill() {}
        inline void operator()( typename internal_vector< T >::type &dest, const T &src ) {
          dest = src;
        }
      };
      template< typename T >
      struct fill< T, true > {
        fill() {}
        inline void operator()( typename internal_vector< T >::type &dest, const T &src ) {
          static const at< T > at_;
          for( size_t index = 0; index != internal_vector< T >::length; ++index )
            at_( dest, index ) = src;
        }
      };


#define HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_INTERNAL( name, func ) \
      template< typename T, bool vectorized = stream::simd::compiler::internal_vector< T >::vectorized > \
      struct name {}; \
      template< typename T > \
      struct name < T, false > { \
        name () {} \
        inline void operator()( \
          T &dest, \
          T &src \
        ) const { \
          func \
        } \
        inline void operator()( \
          T &src \
        ) const { \
          T dest; \
          func \
        } \
      }; \

#define HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_LOOP_ELEMENT( z, index, args ) \
      HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_INTERNAL( \
        BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_PP_SEQ_ELEM( index, args ) ) \
      )

#define HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR( args ) \
      BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( args ), HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_LOOP_ELEMENT, args )

#define HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_INTERNAL( name, func ) \
      template< typename T, bool vectorized = stream::simd::compiler::internal_vector< T >::vectorized > \
      struct name {}; \
      template< typename T > \
      struct name < T, false > { \
        name () {} \
        inline void operator()( \
          T &dest, \
          const T &src \
        ) const { \
          func \
        } \
      };

#define HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_LOOP_ELEMENT( z, index, args ) \
      HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_INTERNAL( \
        BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_PP_SEQ_ELEM( index, args ) ) \
      )

#define HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR( args ) \
      BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( args ), HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_LOOP_ELEMENT, args )

#define HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_INTERNAL( name, func ) \
      template< typename T, bool vectorized = stream::simd::compiler::internal_vector< T >::vectorized > \
      struct name {}; \
      template< typename T > \
      struct name < T, false > { \
        name () {} \
        inline void operator()( \
          T &dest, \
          const T &left, \
          const T &right \
        ) const { \
          func \
        } \
      };

#define HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_LOOP_ELEMENT( z, index, args ) \
      HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_INTERNAL( \
        BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_PP_SEQ_ELEM( index, args ) ) \
      )

#define HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR( args ) \
      BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( args ), HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_LOOP_ELEMENT, args )

#define HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_FUNCS \
      (( pre_inc, ++src; )) \
      (( post_inc, src++; )) \
      (( pre_dec, --src; )) \
      (( post_dec, src--; ))

HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR( HERMIT_STREAM_COMPILER_1OP_DEFAULT_SCALAR_FUNCS )

#define HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_FUNCS \
      (( assign, dest = src; )) \
      (( plus_assign, dest += src; )) \
      (( minus_assign, dest -= src; )) \
      (( multiplies_assign, dest *= src; )) \
      (( divides_assign, dest /= src; )) \
      (( modulus_assign, dest %= src; )) \
      (( bitwise_and_assign, dest &= src; )) \
      (( bitwise_or_assign, dest |= src; )) \
      (( bitwise_xor_assign, dest ^= src; )) \
      (( shift_left_assign, dest <<= src; )) \
      (( shift_right_assign, dest >>= src; )) \
      (( negate, dest = -src; )) \
      (( unary_plus, dest = +src; )) \
      (( logical_not, dest = !src; )) \
      (( complement, dest = ~src; ))

HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR( HERMIT_STREAM_COMPILER_2OP_DEFAULT_SCALAR_FUNCS )

#define HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_FUNCS \
      (( min, dest = std::min( left BOOST_PP_COMMA() right ); )) \
      (( max, dest = std::max( left BOOST_PP_COMMA() right ); )) \
      (( plus, dest = left + right; )) \
      (( minus, dest = left - right; )) \
      (( multiplies, dest = left * right; )) \
      (( divides, dest = left / right; )) \
      (( modulus, dest = left % right; )) \
      (( bitwise_and, dest = left & right; )) \
      (( bitwise_or, dest = left | right; )) \
      (( bitwise_xor, dest = left ^ right; )) \
      (( logical_and, dest = left && right; )) \
      (( logical_or, dest = left || right; )) \
      (( shift_left, dest = left << right; )) \
      (( shift_right, dest = left >> right; )) \
      (( equal_to, dest = left == right; )) \
      (( not_equal_to, dest = left != right; )) \
      (( less, dest = left < right; )) \
      (( less_equal, dest = left <= right; )) \
      (( greater, dest = left > right; )) \
      (( greater_equal, dest = left >= right; ))

HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR( HERMIT_STREAM_COMPILER_3OP_DEFAULT_SCALAR_FUNCS )


#if defined( __GNUC__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
      template< typename T >
      struct assign< T, true > {
        assign() {}
        inline void operator()( typename internal_vector< T >::type &dest, const typename internal_vector< T >::type &src ) {
          dest = src;
        }
      };
#else
      template< typename T >
      struct assign< T, true > {
        assign() {}
        inline void operator()( typename internal_vector< T >::type &dest, const typename internal_vector< T >::type &src ) {
          static const at< T > at_;
          for( size_t index = 0; index != internal_vector< T >::length; ++index )
            at_( dest, index ) = at_( src, index );
        }
      };
#endif


#define HERMIT_STREAM_COMPILER_2OP_INTERNAL( target_type, name, func ) \
      template<> \
      struct name < target_type, true > { \
        name () {} \
        inline void operator()( \
          stream::simd::compiler::internal_vector< target_type >::type &dest, \
          const stream::simd::compiler::internal_vector< target_type >::type &src \
        ) const { \
          func \
        } \
      };

#define HERMIT_STREAM_COMPILER_2OP_LOOP_ELEMENT( z, index, args ) \
      HERMIT_STREAM_COMPILER_2OP_INTERNAL( \
        BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, BOOST_PP_SEQ_ELEM( index, args ) ) \
      )

#define HERMIT_STREAM_COMPILER_2OP( args ) \
      BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( args ), HERMIT_STREAM_COMPILER_2OP_LOOP_ELEMENT, args )


#define HERMIT_STREAM_COMPILER_3OP_INTERNAL( target_type, name, func ) \
      template<> \
      struct name < target_type, true > { \
        name () {} \
        inline void operator()( \
          stream::simd::compiler::internal_vector< target_type >::type &dest, \
          const stream::simd::compiler::internal_vector< target_type >::type &left, \
          const stream::simd::compiler::internal_vector< target_type >::type &right \
        ) const { \
          func \
        } \
      };

#define HERMIT_STREAM_COMPILER_3OP_LOOP_ELEMENT( z, index, args ) \
      HERMIT_STREAM_COMPILER_3OP_INTERNAL( \
        BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, BOOST_PP_SEQ_ELEM( index, args ) ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, BOOST_PP_SEQ_ELEM( index, args ) ) \
      )

#define HERMIT_STREAM_COMPILER_3OP( args ) \
      BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( args ), HERMIT_STREAM_COMPILER_3OP_LOOP_ELEMENT, args )

#if defined( __GNUC__ ) && !defined( __clang__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
#if defined(__AVX__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute( ( vector_size( 32 ) ) );
        typedef int32_t index_type __attribute( ( vector_size( 32 ) ) );
        static constexpr size_t length = 8;
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute( ( vector_size( 32 ) ) );
        static constexpr size_t length = 4;
        typedef int32_t index_type __attribute( ( vector_size( 16 ) ) );
      };
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
#elif defined(__SSE2__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
        typedef int32_t index_type __attribute( ( vector_size( 16 ) ) );
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 2;
        typedef int32_t index_type __attribute( ( vector_size( 8 ) ) );
      };
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
      template<>
      struct internal_vector< int16_t > {
        static constexpr bool vectorized = true;
        typedef int16_t type __attribute( ( vector_size( 16 ) ) );
        static constexpr size_t length = 8;
        typedef type index_type;
      };
#endif
#elif defined( __clang__ )
#if defined(__AVX__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute__( ( __vector_size__( 32 ) ) );
        static constexpr size_t length = 8;
        typedef int32_t index_type __attribute( ( vector_size( 32 ) ) );
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute__( ( __vector_size__( 32 ) ) );
        static constexpr size_t length = 4;
        typedef int32_t index_type __attribute( ( vector_size( 32 ) ) );
      };
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
        typedef type index_type;
      };
#elif defined(__SSE2__)
      template<>
      struct internal_vector< float > {
        static constexpr bool vectorized = true;
        typedef float type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 4;
        typedef int32_t index_type __attribute( ( vector_size( 16 ) ) );
      };
      template<>
      struct internal_vector< double > {
        static constexpr bool vectorized = true;
        typedef double type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 2;
        typedef int32_t index_type __attribute( ( vector_size( 8 ) ) );
      };
      template<>
      struct internal_vector< int32_t > {
        static constexpr bool vectorized = true;
        typedef int32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 2;
        typedef type index_type;
      };
      template<>
      struct internal_vector< uint32_t > {
        static constexpr bool vectorized = true;
        typedef uint32_t type __attribute__( ( __vector_size__( 16 ) ) );
        static constexpr size_t length = 2;
        typedef type index_type;
      };
#endif
#endif

#if defined( __GNUC__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
#if defined(__AVX__)
      template< typename direction >
      struct system_specific_approx_direction_value {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TONEAREST > >
      : public boost::mpl::int_< 0 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_DOWNWARD > >
      : public boost::mpl::int_< 1 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_UPWARD > >
      : public boost::mpl::int_< 2 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TOWARDZERO > >
      : public boost::mpl::int_< 3 > {};
      template< typename direction >
      struct approx< float, direction > {
        approx() {}
        inline void operator()( internal_vector< float >::type &buffer, const internal_vector< float >::type &src ) const {
          buffer = __builtin_ia32_roundps256 ( src, system_specific_approx_direction_value< direction >::value );
        }
        inline void operator()( internal_vector< float >::index_type &buffer, const internal_vector< float >::type &src ) const {
          buffer = __builtin_ia32_cvtps2dq256( __builtin_ia32_roundps256 ( src, system_specific_approx_direction_value< direction >::value ) );
        }
      };
      template< typename direction >
      struct approx< double, direction > {
        approx() {}
        inline void operator()( internal_vector< double >::type &buffer, const internal_vector< double >::type &src ) const {
          buffer = __builtin_ia32_roundpd256 ( src, system_specific_approx_direction_value< direction >::value );
        }
        inline void operator()( internal_vector< double >::index_type &buffer, const internal_vector< double >::type &src ) const {
          buffer = __builtin_ia32_cvtpd2pi256( __builtin_ia32_roundpd256 ( src, system_specific_approx_direction_value< direction >::value ) );
        }
      };
      template< typename direction >
      struct approx< int32_t, direction > {
        approx() {}
        inline void operator()( internal_vector< int32_t >::type &buffer, const internal_vector< int32_t >::type &src ) const {
          buffer = src;
        }
      };
      template< typename direction >
      struct approx< uint32_t, direction > {
        approx() {}
        inline void operaotr()( internal_vector< uint32_t >::type &buffer, const internal_vector< uint32_t >::type &src ) const {
          buffer = src;
        }
      };
#elif defined(__SSE2__)
#if defined(__SSE4_1__)
      template< typename direction >
      struct system_specific_approx_direction_value {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TONEAREST > >
      : public boost::mpl::int_< 0 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_DOWNWARD > >
      : public boost::mpl::int_< 1 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_UPWARD > >
      : public boost::mpl::int_< 2 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TOWARDZERO > >
      : public boost::mpl::int_< 3 > {};
      template< typename direction >
      struct approx< float, direction > {
        approx() {}
        inline void operator()( internal_vector< float >::type &buffer, const internal_vector< float >::type &src ) const {
          buffer = __builtin_ia32_roundps ( src, system_specific_approx_direction_value< direction >::value );
        }
        inline void operator()( internal_vector< float >::index_type &buffer, const internal_vector< float >::type &src ) const {
          buffer = __builtin_ia32_cvtps2dq( __builtin_ia32_roundps ( src, system_specific_approx_direction_value< direction >::value ) );
        }
      };
      template< typename direction >
      struct approx< double, direction > {
        approx() {}
        inline void operator()( internal_vector< double >::type &buffer, const internal_vector< double >::type &src ) const {
          buffer = __builtin_ia32_roundpd ( src, system_specific_approx_direction_value< direction >::value );
        }
        inline void operator()( internal_vector< double >::index_type &buffer, const internal_vector< double >::type &src ) const {
          buffer = __builtin_ia32_cvtpd2pi( __builtin_ia32_roundpd ( src, system_specific_approx_direction_value< direction >::value ) );
        }
      };
#else
      template< typename direction >
      struct system_specific_approx_direction_value {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TONEAREST > >
      : public boost::mpl::int_< 0 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_DOWNWARD > >
      : public boost::mpl::int_< 0x2000 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_UPWARD > >
      : public boost::mpl::int_< 0x4000 > {};
      template<>
      struct system_specific_approx_direction_value< boost::mpl::int_< FE_TOWARDZERO > >
      : public boost::mpl::int_< 0x6000 > {};
      template< typename direction >
      struct approx< float, direction > {
        approx() {}
        inline void operator()( internal_vector< float >::type &buffer, const internal_vector< float >::type &src ) const {
          const auto old_mxcsr = __builtin_ia32_stmxcsr();
          __builtin_ia32_ldmxcsr( ( old_mxcsr & 0xFFFF9FFF ) | system_specific_approx_direction_value< direction >::value );
          buffer = __builtin_ia32_cvtdq2ps ( __builtin_ia32_cvtps2dq( src ) );
          __builtin_ia32_ldmxcsr( old_mxcsr );
        }
        inline void operator()( internal_vector< float >::index_type &buffer, const internal_vector< float >::type &src ) const {
          const auto old_mxcsr = __builtin_ia32_stmxcsr();
          __builtin_ia32_ldmxcsr( ( old_mxcsr & 0xFFFF9FFF ) | system_specific_approx_direction_value< direction >::value );
          buffer = __builtin_ia32_cvtps2dq( src );
          __builtin_ia32_ldmxcsr( old_mxcsr ); 
        }
      };
      template< typename direction >
      struct approx< double, direction > {
        approx() {}
        inline void operator()( internal_vector< double >::type &buffer, const internal_vector< double >::type &src ) const {
          const auto old_mxcsr = __builtin_ia32_stmxcsr();
          __builtin_ia32_ldmxcsr( ( old_mxcsr & 0xFFFF9FFF ) | system_specific_approx_direction_value< direction >::value );
          buffer = __builtin_ia32_cvtpi2pd ( __builtin_ia32_cvtpd2pi( src ) );
          __builtin_ia32_ldmxcsr( old_mxcsr ); 
        }
        inline void operator()( internal_vector< double >::index_type &buffer, const internal_vector< double >::type &src ) const {
          const auto old_mxcsr = __builtin_ia32_stmxcsr();
          __builtin_ia32_ldmxcsr( ( old_mxcsr & 0xFFFF9FFF ) | system_specific_approx_direction_value< direction >::value );
          buffer = __builtin_ia32_cvtpd2pi( src );
          __builtin_ia32_ldmxcsr( old_mxcsr ); 
        }
      };
#endif
      template< typename direction >
      struct approx< int32_t, direction > {
        approx() {}
        inline void operator()( internal_vector< int32_t >::type &buffer, const internal_vector< int32_t >::type &src ) const {
          buffer = src;
        }
      };
      template< typename direction >
      struct approx< uint32_t, direction > {
        approx() {}
        inline void operator()( internal_vector< uint32_t >::type &buffer, const internal_vector< uint32_t >::type &src ) const {
          buffer = src;
        }
      };
#endif
#endif


#if defined( __GNUC__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )

#define HERMIT_STREAM_COMPILER_2OP_FUNCS \
      (( float, assign, dest = src; )) \
      (( float, plus_assign, dest += src; )) \
      (( float, minus_assign, dest -= src; )) \
      (( float, multiplies_assign, dest *= src; )) \
      (( float, divides_assign, dest /= src; )) \
      (( float, negate, dest = -src; )) \
      (( float, unary_plus, dest = +src; )) \
      (( double, assign, dest = src; )) \
      (( double, plus_assign, dest += src; )) \
      (( double, minus_assign, dest -= src; )) \
      (( double, multiplies_assign, dest *= src; )) \
      (( double, divides_assign, dest /= src; )) \
      (( double, negate, dest = -src; )) \
      (( double, unary_plus, dest = +src; )) \
      (( int32_t, assign, dest = src; )) \
      (( int32_t, plus_assign, dest += src; )) \
      (( int32_t, minus_assign, dest -= src; )) \
      (( int32_t, multiplies_assign, dest *= src; )) \
      (( int32_t, divides_assign, dest /= src; )) \
      (( int32_t, modulus_assign, dest %= src; )) \
      (( int32_t, bitwise_and_assign, dest &= src; )) \
      (( int32_t, bitwise_or_assign, dest |= src; )) \
      (( int32_t, bitwise_xor_assign, dest ^= src; )) \
      (( int32_t, negate, dest = -src; )) \
      (( int32_t, unary_plus, dest = +src; )) \
      (( uint32_t, assign, dest = src; )) \
      (( uint32_t, plus_assign, dest += src; )) \
      (( uint32_t, minus_assign, dest -= src; )) \
      (( uint32_t, multiplies_assign, dest *= src; )) \
      (( uint32_t, divides_assign, dest /= src; )) \
      (( uint32_t, modulus_assign, dest %= src; )) \
      (( uint32_t, bitwise_and_assign, dest &= src; )) \
      (( uint32_t, bitwise_or_assign, dest |= src; )) \
      (( uint32_t, bitwise_xor_assign, dest ^= src; )) \
      (( uint32_t, negate, dest = -src; )) \
      (( uint32_t, unary_plus, dest = +src; ))

HERMIT_STREAM_COMPILER_2OP( HERMIT_STREAM_COMPILER_2OP_FUNCS )

#if defined(__AVX__)
#define HERMIT_STREAM_COMPILER_3OP_FUNCS \
      (( float, min, dest = __builtin_ia32_minps256( left BOOST_PP_COMMA() right ); )) \
      (( float, max, dest = __builtin_ia32_maxps256( left BOOST_PP_COMMA() right ); )) \
      (( float, plus, dest = left + right; )) \
      (( float, minus, dest = left - right; )) \
      (( float, multiplies, dest = left * right; )) \
      (( float, divides, dest = left / right; )) \
      (( double, min, dest = __builtin_ia32_minpd256( left BOOST_PP_COMMA() right ); )) \
      (( double, max, dest = __builtin_ia32_maxpd256( left BOOST_PP_COMMA() right ); )) \
      (( double, plus, dest = left + right; )) \
      (( double, minus, dest = left - right; )) \
      (( double, multiplies, dest = left * right; )) \
      (( double, divides, dest = left / right; )) \
      (( int32_t, min, dest = __builtin_ia32_pminsd128( left BOOST_PP_COMMA() right ); )) \
      (( int32_t, max, dest = __builtin_ia32_mmaxsd128( left BOOST_PP_COMMA() right ); )) \
      (( int32_t, plus, dest = left + right; )) \
      (( int32_t, minus, dest = left - right; )) \
      (( int32_t, multiplies, dest = left * right; )) \
      (( int32_t, divides, dest = left / right; )) \
      (( int32_t, modulus, dest = left % right; )) \
      (( uint32_t, min, dest = __builtin_ia32_pminud128( left BOOST_PP_COMMA() right ); )) \
      (( uint32_t, max, dest = __builtin_ia32_mmaxud128( left BOOST_PP_COMMA() right ); )) \
      (( uint32_t, plus, dest = left + right; )) \
      (( uint32_t, minus, dest = left - right; )) \
      (( uint32_t, multiplies, dest = left * right; )) \
      (( uint32_t, divides, dest = left / right; )) \
      (( uint32_t, modulus, dest = left % right; )) \
      (( int16_t, min, dest = __builtin_ia32_pminsw128( left BOOST_PP_COMMA() right ); )) \
      (( int16_t, max, dest = __builtin_ia32_mmaxsw128( left BOOST_PP_COMMA() right ); )) \
      (( int16_t, plus, dest = left + right )) \
      (( int16_t, minus, dest = left - right )) \
      (( int16_t, multiplies, dest = left * right )) \
      (( int16_t, divides, dest = left / right )) \
      (( int16_t, modulus, dest = left % right )) \
      (( uint16_t, min, dest = __builtin_ia32_pminuw128( left BOOST_PP_COMMA() right ); )) \
      (( uint16_t, max, dest = __builtin_ia32_mmaxuw128( left BOOST_PP_COMMA() right ); )) \
      (( uint16_t, plus, dest = left + right )) \
      (( uint16_t, minus, dest = left - right )) \
      (( uint16_t, multiplies, dest = left * right )) \
      (( uint16_t, divides, dest = left / right )) \
      (( uint16_t, modulus, dest = left % right )) \

HERMIT_STREAM_COMPILER_3OP( HERMIT_STREAM_COMPILER_3OP_FUNCS )

#elif defined(__SSE2__)
#if defined(__SSE4_1__)
#define HERMIT_STREAM_COMPILER_3OP_FUNCS \
      (( float, min, dest = __builtin_ia32_minps( left BOOST_PP_COMMA() right ); )) \
      (( float, max, dest = __builtin_ia32_maxps( left BOOST_PP_COMMA() right ); )) \
      (( float, plus, dest = left + right; )) \
      (( float, minus, dest = left - right; )) \
      (( float, multiplies, dest = left * right; )) \
      (( float, divides, dest = left / right; )) \
      (( double, min, dest = __builtin_ia32_minpd( left BOOST_PP_COMMA() right ); )) \
      (( double, max, dest = __builtin_ia32_maxpd( left BOOST_PP_COMMA() right ); )) \
      (( double, plus, dest = left + right; )) \
      (( double, minus, dest = left - right; )) \
      (( double, multiplies, dest = left * right; )) \
      (( double, divides, dest = left / right; )) \
      (( int32_t, min, dest = __builtin_ia32_pminsd128( left BOOST_PP_COMMA() right ); )) \
      (( int32_t, max, dest = __builtin_ia32_mmaxsd128( left BOOST_PP_COMMA() right ); )) \
      (( int32_t, plus, dest = left + right; )) \
      (( int32_t, minus, dest = left - right; )) \
      (( int32_t, multiplies, dest = left * right; )) \
      (( int32_t, divides, dest = left / right; )) \
      (( int32_t, modulus, dest = left % right; )) \
      (( uint32_t, min, dest = __builtin_ia32_pminud128( left BOOST_PP_COMMA() right ); )) \
      (( uint32_t, max, dest = __builtin_ia32_mmaxud128( left BOOST_PP_COMMA() right ); )) \
      (( uint32_t, plus, dest = left + right; )) \
      (( uint32_t, minus, dest = left - right; )) \
      (( uint32_t, multiplies, dest = left * right; )) \
      (( uint32_t, divides, dest = left / right; )) \
      (( uint32_t, modulus, dest = left % right; )) \
      (( int16_t, min, dest = __builtin_ia32_pminsw128( left BOOST_PP_COMMA() right ); )) \
      (( int16_t, max, dest = __builtin_ia32_mmaxsw128( left BOOST_PP_COMMA() right ); )) \
      (( int16_t, plus, dest = left + right; )) \
      (( int16_t, minus, dest = left - right; )) \
      (( int16_t, multiplies, dest = left * right; )) \
      (( int16_t, divides, dest = left / right; )) \
      (( int16_t, modulus, dest = left % right; )) \
      (( int16_t, min, dest = __builtin_ia32_pminuw128( left BOOST_PP_COMMA() right ); )) \
      (( uint16_t, max, dest = __builtin_ia32_mmaxuw128( left BOOST_PP_COMMA() right ); )) \
      (( uint16_t, plus, dest = left + right )) \
      (( uint16_t, minus, dest = left - right )) \
      (( uint16_t, multiplies, dest = left * right )) \
      (( uint16_t, divides, dest = left / right )) \
      (( uint16_t, modulus, dest = left % right ))
#else
#define HERMIT_STREAM_COMPILER_3OP_FUNCS \
      (( float, min, dest = __builtin_ia32_minps( left BOOST_PP_COMMA() right ); )) \
      (( float, max, dest = __builtin_ia32_maxps( left BOOST_PP_COMMA() right ); )) \
      (( float, plus, dest = left + right; )) \
      (( float, minus, dest = left - right; )) \
      (( float, multiplies, dest = left * right; )) \
      (( float, divides, dest = left / right; )) \
      (( double, min, dest = __builtin_ia32_minpd( left BOOST_PP_COMMA() right ); )) \
      (( double, max, dest = __builtin_ia32_maxpd( left BOOST_PP_COMMA() right ); )) \
      (( double, plus, dest = left + right; )) \
      (( double, minus, dest = left - right; )) \
      (( double, multiplies, dest = left * right; )) \
      (( double, divides, dest = left / right; )) \
      (( int32_t, min, \
          const auto dest_ptr = reinterpret_cast< int32_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const int32_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const int32_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< int32_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] < right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( int32_t, max, \
          const auto dest_ptr = reinterpret_cast< int32_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const int32_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const int32_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< int32_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] > right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( int32_t, plus, dest = left + right; )) \
      (( int32_t, minus, dest = left - right; )) \
      (( int32_t, multiplies, dest = left * right; )) \
      (( int32_t, divides, dest = left / right; )) \
      (( int32_t, modulus, dest = left % right; )) \
      (( uint32_t, min, \
          const auto dest_ptr = reinterpret_cast< uint32_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const uint32_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const uint32_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< uint32_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] < right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( uint32_t, max, \
          const auto dest_ptr = reinterpret_cast< uint32_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const uint32_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const uint32_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< uint32_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] > right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( uint32_t, plus, dest = left + right; )) \
      (( uint32_t, minus, dest = left - right; )) \
      (( uint32_t, multiplies, dest = left * right; )) \
      (( uint32_t, divides, dest = left / right; )) \
      (( uint32_t, modulus, dest = left % right; )) \
      (( int16_t, min, \
          const auto dest_ptr = reinterpret_cast< int16_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const int16_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const int16_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< int16_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] < right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( int16_t, max, \
          const auto dest_ptr = reinterpret_cast< int16_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const int16_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const int16_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< int16_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] > right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( int16_t, plus, dest = left + right; )) \
      (( int16_t, minus, dest = left - right; )) \
      (( int16_t, multiplies, dest = left * right; )) \
      (( int16_t, divides, dest = left / right; )) \
      (( int16_t, modulus, dest = left % right; )) \
      (( uint16_t, min, \
          const auto dest_ptr = reinterpret_cast< uint16_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const uint16_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const uint16_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< uint16_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] < right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( uint16_t, max, \
          const auto dest_ptr = reinterpret_cast< uint16_t* >( &dest ); \
          const auto left_ptr = reinterpret_cast< const uint16_t* >( &left ); \
          const auto right_ptr = reinterpret_cast< const uint16_t* >( &right ); \
          for( size_t index = 0ul; index != internal_vector< uint16_t >::length; ++index ) \
            dest_ptr[ index ] = ( left_ptr[ index ] > right_ptr[ index ] ) ? left_ptr[ index ] : right_ptr[ index ]; \
      )) \
      (( uint16_t, plus, dest = left + right; )) \
      (( uint16_t, minus, dest = left - right; )) \
      (( uint16_t, multiplies, dest = left * right; )) \
      (( uint16_t, divides, dest = left / right; )) \
      (( uint16_t, modulus, dest = left % right; ))
#endif
HERMIT_STREAM_COMPILER_3OP( HERMIT_STREAM_COMPILER_3OP_FUNCS )
#endif
#endif

#if defined( __GNUC__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
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
        inline vector( const T &value ) {
          std::fill( begin(), end(), value );
        }
        inline self_type &operator=( const T &value ) {
          std::fill( begin(), end(), value );
          return *this;
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
  namespace simd {
    namespace operation {
#define HERMIT_STREAM_SIMD_OPERATION_3( name ) \
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
        static const simd::compiler:: name < T > operation; \
        for( ; dest_cur != dest_end; ++dest_cur, ++left_cur, ++right_cur ) \
          operation ( dest_cur->packed, left_cur->packed, right_cur->packed ); \
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
        static const simd::compiler:: name < T > operation; \
        for( ; dest_cur != dest_end; ++dest_cur, ++left_cur ) \
          operation ( dest_cur->packed, left_cur->packed, right_vec.packed ); \
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
        static const simd::compiler:: name < T > operation; \
        for( ; dest_cur != dest_end; ++dest_cur, ++right_cur ) \
          operation ( dest_cur->packed, left_vec.packed, right_cur->packed ); \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        T &dest, \
        const T &left, \
        const typename container::vector< T, Size > &right \
      ) { \
        typename compiler::vector< T, Size >::type::value_type left_vec; \
        for( size_t index = 0; index != compiler::internal_vector< T >::length; ++index ) \
          left_vec.unpacked[ index ] = left; \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator right_cur = right.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator dest_end = dest.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; dest_cur != dest_end; ++dest_cur, ++right_cur ) \
          operation ( dest_cur->packed, left_vec.packed, right_cur->packed ); \
      } \
      template<typename T > \
      inline void name ( \
        T &dest, \
        const T &left, \
        const T &right, \
        typename boost::disable_if< is_vector< T > >::type* = 0 \
      ) { \
        static const simd::compiler:: name < T > operation; \
        operation ( dest, left, right ); \
      }

#define HERMIT_STREAM_SIMD_OPERATION_2( name ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left, \
        const typename container::vector< T, Size > &right \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::const_vector_iterator right_cur = right.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++left_cur, ++right_cur ) \
          operation ( left_cur->packed, right_cur->packed ); \
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
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++left_cur ) \
          operation ( left_cur->packed, right.packed ); \
      } \
      template<typename T > \
      inline void name ( \
        T &dest, \
        const T &src, \
        typename boost::disable_if< is_vector< T > >::type* = 0 \
      ) { \
        static const simd::compiler:: name < T > operation; \
        operation ( dest, src ); \
      }

#define HERMIT_STREAM_SIMD_OPERATION_1PRE( name ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::const_vector_iterator left_end = left.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++dest_cur, ++left_cur ) \
          operation ( dest_cur->packed, left_cur->packed ); \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++left_cur ) \
          operation ( left_cur->packed ); \
      } \
      template<typename T > \
      inline void name ( \
        T &dest, \
        const T &src, \
        typename boost::disable_if< is_vector< T > >::type* = 0 \
      ) { \
        static const simd::compiler:: name < T > operation; \
        operation ( dest, src ); \
      }

#define HERMIT_STREAM_SIMD_OPERATION_1POST( name ) \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &dest, \
        const typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::const_vector_iterator left_cur = left.vbegin(); \
        typename container::vector< T, Size >::vector_iterator dest_cur = dest.vbegin(); \
        const typename container::vector< T, Size >::const_vector_iterator left_end = left.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++dest_cur, ++left_cur ) \
          operation ( dest_cur->packed, left_cur->packed ); \
      } \
      template<typename T, typename Size> \
      inline void name ( \
        typename container::vector< T, Size > &left \
      ) { \
        typename container::vector< T, Size >::vector_iterator left_cur = left.vbegin(); \
        const typename container::vector< T, Size >::vector_iterator left_end = left.vend(); \
        static const simd::compiler:: name < T > operation; \
        for( ; left_cur != left_end; ++left_cur ) \
          operation ( left_cur->packed ); \
      } \
      template<typename T > \
      inline void name ( \
        T &dest, \
        const T &src, \
        typename boost::disable_if< is_vector< T > >::type* = 0 \
      ) { \
        static const simd::compiler:: name < T > operation; \
        operation ( dest, src ); \
      }

HERMIT_STREAM_SIMD_OPERATION_3( plus )
HERMIT_STREAM_SIMD_OPERATION_2( plus_assign )
HERMIT_STREAM_SIMD_OPERATION_3( minus )
HERMIT_STREAM_SIMD_OPERATION_2( minus_assign )
HERMIT_STREAM_SIMD_OPERATION_3( multiplies )
HERMIT_STREAM_SIMD_OPERATION_2( multiplies_assign )
HERMIT_STREAM_SIMD_OPERATION_3( divides )
HERMIT_STREAM_SIMD_OPERATION_2( divides_assign )
HERMIT_STREAM_SIMD_OPERATION_3( modulus )
HERMIT_STREAM_SIMD_OPERATION_2( modulus_assign )
HERMIT_STREAM_SIMD_OPERATION_3( equal_to )
HERMIT_STREAM_SIMD_OPERATION_3( not_equal_to )
HERMIT_STREAM_SIMD_OPERATION_3( greater )
HERMIT_STREAM_SIMD_OPERATION_3( less )
HERMIT_STREAM_SIMD_OPERATION_3( greater_equal )
HERMIT_STREAM_SIMD_OPERATION_3( less_equal )
HERMIT_STREAM_SIMD_OPERATION_3( shift_right )
HERMIT_STREAM_SIMD_OPERATION_2( shift_right_assign )
HERMIT_STREAM_SIMD_OPERATION_3( shift_left )
HERMIT_STREAM_SIMD_OPERATION_2( shift_left_assign )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_and )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_and_assign )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_or )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_or_assign )
HERMIT_STREAM_SIMD_OPERATION_3( logical_and )
HERMIT_STREAM_SIMD_OPERATION_3( logical_or )
HERMIT_STREAM_SIMD_OPERATION_3( bitwise_xor )
HERMIT_STREAM_SIMD_OPERATION_2( bitwise_xor_assign )
HERMIT_STREAM_SIMD_OPERATION_1PRE( pre_inc )
HERMIT_STREAM_SIMD_OPERATION_1PRE( pre_dec )
HERMIT_STREAM_SIMD_OPERATION_1POST( post_inc )
HERMIT_STREAM_SIMD_OPERATION_1POST( post_dec )
HERMIT_STREAM_SIMD_OPERATION_1PRE( unary_plus )
HERMIT_STREAM_SIMD_OPERATION_1PRE( negate )
HERMIT_STREAM_SIMD_OPERATION_1PRE( complement )
HERMIT_STREAM_SIMD_OPERATION_1PRE( logical_not )
    }
    namespace wrapper {
    }
  }


  namespace detail {
  }


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
 
  template< typename direction >
  struct approx {
    approx() {}
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &src,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >,
            boost::mpl::not_<
              boost::is_integral<
                typename vector_element_type< T >::type
              >
            >
          >
        >::type* = 0
      ) const {
        typename T::const_vector_iterator src_cur = src.vbegin();
        typename T::vector_iterator dest_cur = ctx.current_buffer->vbegin();
        const typename T::const_vector_iterator src_end = src.vend();
        static const simd::compiler::approx< typename vector_element_type< T >::type, direction > approx_method;
        for( ; src_cur != src_end; ++src_cur, ++dest_cur )
          approx_method( dest_cur->packed, src_cur->packed );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &src,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >,
            boost::is_integral<
              typename vector_element_type< T >::type
            >
          >
        >::type* = 0
      ) const {
        return src;
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
            boost::mpl::not_<
              boost::is_integral< T >
            >
          >
        >::type* = 0
      ) const {
        const int old_mode = fegetround();
        if( fesetround( direction::value ) )
          throw -1;
        T temp = static_cast< T >( static_cast< int32_t >( source ) );
        fesetround( old_mode );
        return temp;
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
        return source;
      }
  };
  boost::proto::terminal< approx< boost::mpl::int_< FE_DOWNWARD > > >::type const floor = {{}};
  boost::proto::terminal< approx< boost::mpl::int_< FE_TONEAREST > > >::type const round = {{}};
  boost::proto::terminal< approx< boost::mpl::int_< FE_TOWARDZERO > > >::type const trunc = {{}};
  boost::proto::terminal< approx< boost::mpl::int_< FE_UPWARD > > >::type const ceil = {{}};

  struct max_ {
    max_() {}
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &left,
        const T &right,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >
          >
        >::type* = 0
      ) const {
        typename T::const_vector_iterator left_cur = left.vbegin();
        typename T::const_vector_iterator right_cur = right.vbegin();
        typename T::vector_iterator dest_cur = ctx.current_buffer->vbegin();
        const typename T::const_vector_iterator left_end = left.vend();
        static const simd::compiler::max< typename vector_element_type< T >::type > max_method;
        for( ; left_cur != left_end; ++left_cur, ++right_cur, ++dest_cur )
          max_method( dest_cur->packed, left_cur->packed, right_cur->packed );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline T operator()(
        const T &left,
        const T &right,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >,
            boost::mpl::not_<
              boost::is_integral< T >
            >
          >
        >::type* = 0
      ) const {
        return std::max( left, right );
      }
  };

  boost::proto::terminal< max_ >::type const max = {{}};
  
  struct min_ {
    min_() {}
    template< typename LeftType, typename RightType, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &left,
        const T &right,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >
          >
        >::type* = 0
      ) const {
        typename T::const_vector_iterator left_cur = left.vbegin();
        typename T::const_vector_iterator right_cur = right.vbegin();
        typename T::vector_iterator dest_cur = ctx.current_buffer->vbegin();
        const typename T::const_vector_iterator left_end = left.vend();
        static const simd::compiler::min< typename vector_element_type< T >::type > min_method;
        for( ; left_cur != left_end; ++left_cur, ++right_cur, ++dest_cur )
          min_method( dest_cur->packed, left_cur->packed, right_cur->packed );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline const typename Context::buffer_type &operator()(
        const T &left,
        const T &right,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >
          >
        >::type* = 0
      ) const {
        typename T::const_vector_iterator left_cur = left.vbegin();
        typename T::const_vector_iterator right_cur = right.vbegin();
        typename T::vector_iterator dest_cur = ctx.current_buffer->vbegin();
        const typename T::const_vector_iterator left_end = left.vend();
        static const simd::compiler::min< typename vector_element_type< T >::type > min_method;
        for( ; left_cur != left_end; ++left_cur, ++right_cur, ++dest_cur )
          min_method( dest_cur->packed, left_cur->packed, right_cur->packed );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context >
      inline T operator()(
        const T &left,
        const T &right,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >,
            boost::mpl::not_<
              boost::is_integral< T >
            >
          >
        >::type* = 0
      ) const {
        return std::min( left, right );
      }
  };

  boost::proto::terminal< min_ >::type const min = {{}};

  struct nearest_sampler_ {
    nearest_sampler_() {}
    template< typename T, typename Context, typename Range >
      inline const typename Context::buffer_type &operator()(
        const Range &texture,
        const T &src,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            is_vector< T >,
            is_same_length< T, typename Context::buffer_type >
          >
        >::type* = 0
      ) const {
        static const approx< boost::mpl::int_< FE_TONEAREST > > round;
        static const min_ min;
        static const max_ max;
        simd::operation::multiplies( *ctx.current_buffer, src, static_cast< typename vector_element_type< T >::type >( boost::size( texture ) ) );
        round( *ctx.current_buffer, ctx );
        min( *ctx.current_buffer, static_cast< typename vector_element_type< T >::type >( 0 ), ctx );
        max( *ctx.current_buffer, static_cast< typename vector_element_type< T >::type >( boost::size( texture ) - 1 ), ctx );
        const auto texture_head = boost::begin( texture );
        std::transform(
          ctx.current_buffer->begin(),
          ctx.current_buffer->end(),
          ctx.current_buffer->begin(),
          [&]( typename vector_element_type< T >::type &elem ) -> typename vector_element_type< T >::type {
            auto texture_cur = texture_head;
            std::advance( texture_cur, static_cast< int >( elem ) );
            return *texture_cur;
          }
        );
        return *ctx.current_buffer;
      }
    template< typename T, typename Context, typename Range >
      inline T operator()(
        const Range &texture,
        const T &src,
        Context &ctx,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< is_vector< T > >
          >
        >::type* = 0
      ) const {
        static const approx< boost::mpl::int_< FE_TONEAREST > > round;
        auto texture_cur = boost::begin( texture );
        std::advance( texture_cur, round( src * boost::size( texture ), ctx ) );
        return *texture_cur;
      }
  private:
  };

  boost::proto::terminal< nearest_sampler_ >::type const nearest_sampler = {{}};

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
          is_vector<
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >,
          is_vector<
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >
        > {};
        template< typename Expr >
        struct only_left_is_vector : public boost::mpl::and_<
          is_vector<
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >,
          boost::mpl::not_< is_vector<
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          > >
        > {};
        template< typename Expr >
        struct only_right_is_vector : public boost::mpl::and_<
          boost::mpl::not_< is_vector<
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          > >,
          is_vector<
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          >
        > {};
        template< typename Expr >
        struct neither_is_vector : public boost::mpl::and_<
          boost::mpl::not_< is_vector<
            decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          > >,
          boost::mpl::not_< is_vector<
            decltype( proto::eval( boost::proto::right( std::declval< Expr& >() ), std::declval< self_type& >() ) )
          > >
        > {};
        template< typename Expr >
        struct source_is_vector : public is_vector<
          decltype( proto::eval( boost::proto::left( std::declval< Expr& >() ), std::declval< self_type& >() ) )
        > {};
        template< typename Expr >
        struct source_is_not_vector : public boost::mpl::not_<
          is_vector<
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

#define HERMIT_STREAM_CONTEXT_OPERATION_2( name, oper ) \
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
            typename boost::enable_if< source_is_vector< Expr > >::type* = 0 \
          ) { \
            const auto &value = proto::eval( boost::proto::left( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, value ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static auto name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< source_is_not_vector< Expr > >::type* = 0 \
          ) -> decltype( oper proto::eval( boost::proto::left( e ), ctx ) ) { \
            return oper proto::eval( boost::proto::left( e ), ctx ); \
          }
#define HERMIT_STREAM_CONTEXT_OPERATION_1POST( name, oper ) \
        template< typename Expr > \
          inline static const buffer_type &name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< source_is_vector< Expr > >::type* = 0 \
          ) { \
            const buffer_type value = proto::eval( boost::proto::left( e ), ctx ); \
            simd::operation:: name < T, Size >( *ctx.current_buffer, value ); \
            return *ctx.current_buffer; \
          } \
        template< typename Expr > \
          inline static auto name ( \
            Expr &e, self_type &ctx, \
            typename boost::enable_if< source_is_not_vector< Expr > >::type* = 0 \
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

#define HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( tag ) \
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

HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( plus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( minus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( multiplies )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( divides )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( modulus )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( equal_to )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( not_equal_to )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( bitwise_and )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( bitwise_or )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( bitwise_xor )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( logical_and )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( logical_or )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( greater )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( less )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( greater_equal )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( less_equal )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( shift_left )
HERMIT_STREAM_CONTEXT_ACTIVE_OPERATION_2( shift_right )
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
  {
    std::cout << stream::is_vector< stream::simd::container::vector< float, boost::mpl::size_t< 100 > > >::value << std::endl;
    std::array< float, 64 > sin_table;
    for( int index = 0; index != 64; ++index )
      sin_table[ index ] = sinf( index / 64.0f * 2.0f * M_PI );
    stream::simd::container::vector< float, boost::mpl::size_t< 100 > > a;
    for( int index = 0; index != 100; ++index )
      a[ index ] = index;
    boost::fusion::vector<
      stream::simd::container::vector< float, boost::mpl::size_t< 100 > >
    > args( a );
    stream::my_context< decltype( args ), float, boost::mpl::size_t< 100 > > ctx( args );
//    boost::proto::eval( stream::_1 + stream::_1, ctx );
    boost::proto::eval( stream::nearest_sampler( sin_table, stream::_1 + 50.0f ), ctx );
    for( const auto &elem: ctx.get_origin_buffer() )
      std::cout << elem << " ";
    std::cout << std::endl;
  }
  {
    std::cout << stream::is_vector< float >::value << std::endl;
    std::cout << stream::is_vector< stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::value << std::endl;
    std::cout << stream::is_same_length< stream::simd::container::vector< float, boost::mpl::size_t< 64 > >, stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::value << std::endl;
    std::cout << boost::is_same< float, typename stream::vector_element_type< stream::simd::container::vector< float, boost::mpl::size_t< 64 > > >::type >::value << std::endl;
    stream::simd::container::vector< float, boost::mpl::size_t< 64 > > woo = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    boost::fusion::vector< int, int, int, stream::simd::container::vector< float, boost::mpl::size_t< 64 > > > args( 1, 2, 10, woo );
    stream::my_context< decltype( args ), float, boost::mpl::size_t< 64 > > ctx( args );
//  boost::proto::eval( stream::pass_through( boost::proto::lit( 1 ) + woo + stream::_4 - 20 ), ctx );
    boost::proto::eval( stream::floor( stream::sin( -boost::proto::lit( 0.1f ) ) + woo ) + stream::_4 - 20, ctx );
    for( const auto &elem: ctx.get_origin_buffer() )
      std::cout << elem << " ";
    std::cout << std::endl;
  }
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
