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
  struct microxml {
    typedef boost::variant< std::u32string, boost::recursive_wrapper< microxml > > child_t;
    typedef std::map< std::u32string, std::u32string > attr_t;
    std::u32string tag;
    attr_t attribute;
    std::vector< child_t > child;
  };
}

BOOST_FUSION_ADAPT_STRUCT(
  hermit::microxml,
  (std::u32string, tag)
  (hermit::microxml::attr_t, attribute)
  (std::vector< hermit::microxml::child_t >, child)
)

#endif

