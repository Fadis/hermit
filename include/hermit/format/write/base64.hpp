#ifndef HERMIT_FORMAT_WRITE_BASE64_HPP
#define HERMIT_FORMAT_WRITE_BASE64_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/spirit/karma/base64.hpp>

namespace hermit {
  namespace format {
    boost::optional< std::string > write_base64(
      const std::vector< uint8_t > &source
    ) {
      std::string result;
      hermit::spirit::karma::base64< std::back_insert_iterator< std::string > > gen;
      if( boost::spirit::karma::generate(
        std::back_inserter( result ),
        gen,
        source
      ) )
        return result;
      else
        return boost::optional< std::string >();
    }
  }
}

#endif

