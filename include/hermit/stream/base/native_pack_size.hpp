#ifndef HERMIT_STREAM_BASE_NATIVE_PACK_SIZE_HPP
#define HERMIT_STREAM_BASE_NATIVE_PACK_SIZE_HPP

#include <cstdint>
#include <boost/mpl/size_t.hpp>

namespace hermit {
  namespace stream {
template<
  typename T
> struct native_pack_size : public boost::mpl::size_t< 1 > {};
  }
}

#endif

