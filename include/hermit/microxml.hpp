#ifndef HERMIT_MICROXML_HPP
#define HERMIT_MICROXML_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <hermit/none_type.hpp>

namespace hermit {
  class microxml;
  struct microxml {
    typedef boost::variant< std::string, boost::recursive_wrapper< microxml > > child_t;
    typedef std::map< std::string, std::string > attr_t;
    std::string tag;
    attr_t attribute;
    std::vector< child_t > child;
  };
}

BOOST_FUSION_ADAPT_STRUCT(
  hermit::microxml,
  (std::string, tag)
  (hermit::microxml::attr_t, attribute)
  (std::vector< hermit::microxml::child_t >, child)
)

#endif

