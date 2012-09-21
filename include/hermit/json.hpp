#ifndef HERMIT_JSON_HPP
#define HERMIT_JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <hermit/none_type.hpp>

namespace hermit {

  enum json_encoding_type {
    json_utf8,
    json_utf16,
    json_utf32,
    json_utf16be,
    json_utf16le,
    json_utf32be,
    json_utf32le
  };

  typedef boost::make_recursive_variant<
    std::string,
    double,
    std::map< std::string, boost::recursive_variant_ >,
    std::vector< boost::recursive_variant_ >,
    bool,
    none_type
  >::type json;
}

#endif

