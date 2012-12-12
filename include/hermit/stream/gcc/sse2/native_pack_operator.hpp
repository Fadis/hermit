#ifndef HERMIT_STREAM_GCC_SSE2_NATIVE_PACK_OPERATOR_HPP
#define HERMIT_STREAM_GCC_SSE2_NATIVE_PACK_OPERATOR_HPP

#include <hermit/stream/gcc/native_pack.hpp>

namespace hermit {
  namespace stream {
    template<
      typename LeftType, size_t left_size,
      typename RightType, size_t right_size
    >
    native_pack<
      decltype( std::declval< LeftType >() + std::declval< RightType >() ),
      boost::mpl::max<
        boost::mpl::size_t_< left_size >,
        boost::mpl::size_t_< right_size >
      >::value
    > add
  }
}

#if defined(__AVX2__)
#include <hermit/stream/gcc/avx2/native_pack_operator.hpp>
#elif defined(__AVX__)
#include <hermit/stream/gcc/avx/native_pack_operator.hpp>
#elif defined(__SSE2__)
#include <hermit/stream/gcc/sse2/native_pack_operator.hpp>
#elif defined(__SSE__)
#include <hermit/stream/gcc/sse/native_pack_operator.hpp>
#elif defined(__MMX__)
#include <hermit/stream/gcc/mmx/native_pack_operator.hpp>
#endif

#endif
