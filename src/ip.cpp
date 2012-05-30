#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector.hpp>
#include <vector>
#include <string>
#include <utility>

namespace hermit {
  namespace detail {
    boost::spirit::qi::uint_parser<uint8_t, 10, 1, 3> dec3_p;
    boost::spirit::qi::uint_parser<uint8_t, 10, 1, 2> dec2_p;
    boost::spirit::qi::uint_parser<uint8_t, 16, 1, 4> hex4_p;
  }
}
