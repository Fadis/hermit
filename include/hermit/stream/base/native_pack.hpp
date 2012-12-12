#ifndef HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_BASE_NATIVE_PACK_BASE_HPP
#define HERMIT_STREAM_NATIVE_PACK_NATIVE_PACK_BASE_NATIVE_PACK_BASE_HPP

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/mpl/or.hpp>
#include <hermit/stream/native_pack_size.hpp>

namespace hermit {
  namespace stream {
    template<
      typename T,
      size_t size = native_pack_size< T >::value,
      bool is_pod = boost::mpl::or_< boost::is_integral< T >, boost::is_float< T > >::value
    >
    struct native_pack {};
  }
}

#endif

