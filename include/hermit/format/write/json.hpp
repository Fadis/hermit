#ifndef HERMIT_FORMAT_WRITE_JSON_HPP
#define HERMIT_FORMAT_WRITE_JSON_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/json.hpp>
#include <hermit/spirit/karma/json.hpp>

namespace hermit {
  namespace format {
    boost::optional< std::string > write_json(
      const json &source
    ) {
      std::string result;
      hermit::spirit::karma::json< std::back_insert_iterator< std::string > > gen;
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

