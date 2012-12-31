#ifndef HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_OPERATOR_HPP
#define HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_OPERATOR_HPP

#include <hermit/stream/base/native_pack_operator.hpp>

#if defined( __GNUC__ ) && !defined( __clang__ ) && ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
#include <hermit/stream/gcc/native_pack_operator.hpp>
#elif defined( __clang__ )
#include <hermit/stream/clang/native_pack_operator.hpp>
#endif

#endif

