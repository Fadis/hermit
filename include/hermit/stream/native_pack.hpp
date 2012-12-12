#ifndef HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_HPP
#define HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_HPP

#include <hermit/stream/base/native_pack.hpp>

#if defined( __GNUC__ ) && !defined( __clang__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
#include <hermit/stream/gcc/native_pack.hpp>
#elif defined( __clang__ )
#include <hermit/stream/clang/native_pack.hpp>
#endif

#endif

