#ifndef HERMIT_FORMAT_WRITE_IPV4_HPP
#define HERMIT_FORMAT_WRITE_IPV4_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/ip.hpp>
#include <hermit/spirit/karma/ipv4.hpp>

namespace hermit {
  namespace format {
    boost::optional< std::string > write_ipv4(
      const hermit::ipv4 &source
    ) {
      std::string result;
      hermit::spirit::karma::ipv4< std::back_insert_iterator< std::string > > gen;
      if( boost::spirit::karma::generate(
        std::back_inserter( result ),
        gen,
        source
      ) )
        return result;
      else
        return boost::optional< std::string >();
    }
    boost::optional< std::string > write_ipv4segment(
      const std::pair< hermit::ipv4, hermit::ipv4 > &source
    ) {
      std::string result;
      hermit::spirit::karma::ipv4segment< std::back_insert_iterator< std::string > > gen;
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

