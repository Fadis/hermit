#ifndef HERMIT_FORMAT_WRITE_URI_HPP
#define HERMIT_FORMAT_WRITE_URI_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/uri.hpp>
#include <hermit/spirit/karma/uri.hpp>

namespace hermit {
  namespace format {
    boost::optional< std::string > write_uri(
      const hermit::uri &source
    ) {
      std::string result;
      hermit::spirit::karma::uri< std::back_insert_iterator< std::string > > gen;
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

