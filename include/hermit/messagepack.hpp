#ifndef HERMIT_MESSAGEPACK_HPP
#define HERMIT_MESSAGEPACK_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/optional.hpp>

namespace hermit {
  typedef boost::make_recursive_variant<
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    float,
    double,
    std::nullptr_t,
    bool,
    std::vector< uint8_t >,
    std::vector< boost::recursive_variant_ >,
    std::map< boost::recursive_variant_, boost::recursive_variant_ >
  >::type messagepack;
}

#endif

