#ifndef HERMIT_STREAM_CLANG_NATIVE_PACK_SIZE_HPP
#define HERMIT_STREAM_CLANG_NATIVE_PACK_SIZE_HPP

#if defined(__AVX2__)
#include <hermit/stream/clang/avx2/native_pack_size.hpp>
#elif defined(__AVX__)
#include <hermit/stream/clang/avx/native_pack_size.hpp>
#elif defined(__SSE2__)
#include <hermit/stream/clang/sse2/native_pack_size.hpp>
#elif defined(__SSE__)
#include <hermit/stream/clang/sse/native_pack_size.hpp>
#elif defined(__MMX__)
#include <hermit/stream/clang/mmx/native_pack_size.hpp>
#endif

#endif
