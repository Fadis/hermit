#ifndef HERMIT_FORMAT_WRITE_UTF16_HPP
#define HERMIT_FORMAT_WRITE_UTF16_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/json.hpp>
#include <hermit/spirit/karma/utf16.hpp>

namespace hermit {
  namespace format {
    boost::optional< std::vector<uint8_t> > write_utf16(
      const std::u32string &source
    ) {
      std::vector<uint8_t> result;
      hermit::spirit::karma::utf16string< std::back_insert_iterator< std::vector<uint8_t> > > gen;
      if( boost::spirit::karma::generate(
        std::back_inserter( result ),
        gen,
        source
      ) )
        return result;
      else
        return boost::optional< std::vector<uint8_t> >();
    }
  }
}

#endif

