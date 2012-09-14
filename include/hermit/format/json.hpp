#ifndef HERMIT_FORMAT_JSON_HPP
#define HERMIT_FORMAT_JSON_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/json.hpp>
#include <hermit/spirit/json.hpp>

namespace hermit {
  namespace format {
    template<
      typename T,
      typename Concept = typename boost::enable_if<
        boost::mpl::and_<
          boost::is_convertible<
            typename range_value< const T >::type,
            const char
          >,
          is_forward_traversal_range< const T >,
          is_readable_range< const T >
        >
      >::type
    >
    boost::optional< json > read_json(
      const T &str
    ) {
      hermit::spirit::qi::json< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      json result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< json >();
    }
 
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

