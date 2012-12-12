#ifndef HERMIT_STREAM_GCC_AVX_NATIVE_PACK_SIZE_HPP
#define HERMIT_STREAM_GCC_AVX_NATIVE_PACK_SIZE_HPP

#include <boost/mpl/size_t.hpp>

namespace hermit {
  namespace stream {

template<>
struct native_pack_size< float > : public boost::mpl::size_t< 8 > {};

template<>
struct native_pack_size< double > : public boost::mpl::size_t< 4 > {};

template<>
struct native_pack_size< int32_t > : public boost::mpl::size_t< 4 > {};

template<>
struct native_pack_size< uint32_t > : public boost::mpl::size_t< 4 > {};

template<>
struct native_pack_size< int16_t > : public boost::mpl::size_t< 8 > {};

template<>
struct native_pack_size< uint16_t > : public boost::mpl::size_t< 8 > {};

template<>
struct native_pack_size< int8_t > : public boost::mpl::size_t< 16 > {};

template<>
struct native_pack_size< uint8_t > : public boost::mpl::size_t< 16 > {};

  }
}

#endif

