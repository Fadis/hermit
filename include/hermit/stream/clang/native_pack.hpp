#ifndef HERMIT_STREAM_NATIVE_PACK_CLANG_NATIVE_PACK_HPP
#define HERMIT_STREAM_NATIVE_PACK_CLANG_NATIVE_PACK_HPP

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/array.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/utility/enable_if.hpp>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

namespace hermit {
  namespace stream {
    namespace detail {
      template< typename T, size_t size_ >
      struct native_packed_type {};

#define HERMIT_STREAM_NATIVE_POD_TYPES \
    ( 8, ( float, double, int32_t, uint32_t, int16_t, uint16_t, int8_t, uint8_t ) )

#define HERMIT_STREAM_NATIVE_PACKED_TYPES_I( z, index, type_ ) \
    template<> \
    struct native_packed_type< type_, index > { \
      typedef type_ type __attribute__( ( __vector_size__( index * sizeof( type_ ) ) ) ); \
    };

#define HERMIT_STREAM_NATIVE_PACKED_TYPES( z, index, types ) \
  BOOST_PP_REPEAT_FROM_TO( 1, 255, HERMIT_STREAM_NATIVE_PACKED_TYPES_I, BOOST_PP_ARRAY_ELEM( index, types ) )
BOOST_PP_REPEAT(
  BOOST_PP_ARRAY_SIZE( HERMIT_STREAM_NATIVE_POD_TYPES ),
  HERMIT_STREAM_NATIVE_PACKED_TYPES,
  HERMIT_STREAM_NATIVE_POD_TYPES
)
#undef HERMIT_STREAM_NATIVE_POD_TYPES
#undef HERMIT_STREAM_NATIVE_PACKED_TYPES_I
#undef HERMIT_STREAM_NATIVE_PACKED_TYPES
    }
    template< typename T, size_t size_ >
    class native_pack< T, size_, true > {
    public:
      typedef typename detail::native_packed_type< T, size_ > type;
      typedef T value_type;
      typedef T& reference;
      typedef const T& const_reference;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef size_t size_type;
      typedef T* iterator;
      typedef const T* const_iterator;
      native_pack() {}
      template< typename T_,
        typename enabled = typename boost::enable_if<
          boost::mpl::or_<
            boost::is_integral< T_ >,
            boost::is_float< T_ >
          >
        >::type
      >
      native_pack( const T_ &src ) {
        std::fill( src.begin(), src.end(), src );
      }
      native_pack( const native_pack< T, size_ > &src ) : value( src.value ) {
      }
      native_pack( native_pack< T, size_ > &&src ) : value( src.value ) {
      }
      template< typename Iterator >
      native_pack( Iterator begin_, Iterator end_ ) {
        if( std::distance( begin_, end_ ) > size() )
          throw std::out_of_range( "The range is larger than pack length." );
        std::copy( begin_, end_, begin() );
      }
      template< typename T_,
        typename enabled = typename boost::enable_if<
          boost::mpl::or_<
            boost::is_integral< T_ >,
            boost::is_float< T_ >
          >
        >::type
      >
      native_pack( const std::initializer_list< T_ > &src ) {
        if( src.size() > size() )
          throw std::out_of_range( "The range is larger than pack length." );
        std::copy( src.begin(), src.end(), begin() );
      }
      reference operator[]( int index ) {
        return reinterpret_cast< pointer >( &value )[ index ];
      }
      const reference operator[]( int index ) const {
        return reinterpret_cast< const_pointer >( &value )[ index ];
      }
      reference at( int index ) {
        if( index >= size() )
          throw std::out_of_range( "The index is larger than pack length." );
        return reinterpret_cast< pointer >( &value )[ index ];
      }
      const reference at( int index ) const {
        if( index >= size() )
          throw std::out_of_range( "The index is larger than pack length." );
        return reinterpret_cast< const_pointer >( &value )[ index ];
      }
      iterator begin() {
        return reinterpret_cast< pointer >( &value );
      }
      const_iterator begin() const {
        return cbegin();
      }
      const_iterator cbegin() const {
        return reinterpret_cast< const_pointer >( &value );
      }
      iterator end() {
        return reinterpret_cast< pointer >( &value ) + size();
      }
      const_iterator end() const {
        return cend();
      }
      const_iterator cend() const {
        return reinterpret_cast< const_pointer >( &value ) + size();
      }
      constexpr size_type size() {
        return size_;
      }
      type get() {
        return value;
      }
      const type get() const {
        return value;
      }
      template< typename T_, size_t size__ >
      operator native_pack< T_, size__ >() const {
        const auto begin_ = begin();
        auto end_ = begin_;
        std::advance( end_, std::min( size__, size() ) );
        return native_pack< T_, size_ >( begin_, end_ );
      }
    private:
      type value;
    };
  }
}

#endif

