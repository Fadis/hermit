#ifndef HERMIT_SPIRIT_QI_COMMON
#define HERMIT_SPIRIT_QI_COMMON

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
      boost::spirit::qi::uint_parser<uint16_t, 10, 1, 5> dec5_p;
    }
  }
}
