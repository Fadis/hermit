#ifndef HERMIT_STREAM_GCC_SSE2_NATIVE_PACK_OPERATOR_HPP
#define HERMIT_STREAM_GCC_SSE2_NATIVE_PACK_OPERATOR_HPP

#include <hermit/stream/gcc/native_pack.hpp>

namespace hermit {
  namespace stream {
    template<
      typename Type, size_t size
    >
    struct plus< Type, size, Type, size > {
      static native_pack< Type, size >
      run(
        const native_pack< Type, size > &left,
        const native_pack< Type, size > &right
      ) {
        native_pack< Type, size > temp;
        temp.get() = left.get() + right.get();
        return temp;
      }
    };
  }
}

#endif
