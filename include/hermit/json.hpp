#ifndef HERMIT_JSON_HPP
#define HERMIT_JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <hermit/none_type.hpp>

namespace hermit {
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

