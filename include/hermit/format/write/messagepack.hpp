#ifndef HERMIT_FORMAT_WRITE_MESSAGEPACK_HPP
#define HERMIT_FORMAT_WRITE_MESSAGEPACK_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/messagepack.hpp>
#include <hermit/spirit/karma/messagepack.hpp>

namespace hermit {
  namespace fromat {
    boost::optional< std::vector< uint8_t > > write_messagepack(
      const messagepack &source
    ) {
      std::vector< uint8_t > result;
      hermit::spirit::karma::messagepack< std::back_insert_iterator< std::vector< uint8_t > > > gen;
      if( boost::spirit::karma::generate(
        std::back_inserter( result ),
        gen,
        source
      ) )
        return result;
      else
        return boost::optional< std::vector< uint8_t > >();
    }
  }
}

#endif

