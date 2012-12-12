#ifndef HERMIT_STREAM_GCC_NATIVE_PACK_SIZE_HPP
#define HERMIT_STREAM_GCC_NATIVE_PACK_SIZE_HPP

#if defined(__AVX2__)
#include <hermit/stream/gcc/avx2/native_pack_size.hpp>
#elif defined(__AVX__)
#include <hermit/stream/gcc/avx/native_pack_size.hpp>
#elif defined(__SSE2__)
#include <hermit/stream/gcc/sse2/native_pack_size.hpp>
#elif defined(__SSE__)
#include <hermit/stream/gcc/sse/native_pack_size.hpp>
#elif defined(__MMX__)
#include <hermit/stream/gcc/mmx/native_pack_size.hpp>
#endif

#endif
