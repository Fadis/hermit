#ifndef HERMIT_FORMAT_MESSAGEPACK_HPP
#define HERMIT_FORMAT_MESSAGEPACK_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/messagepack.hpp>
#include <hermit/spirit/messagepack.hpp>

namespace hermit {
  namespace format {
    template<
      typename T,
      typename Concept = typename boost::enable_if<
        boost::mpl::and_<
          boost::is_convertible<
            typename range_value< const T >::type,
            uint8_t
          >,
          is_forward_traversal_range< const T >,
          is_readable_range< const T >
        >
      >::type
    >
    boost::optional< messagepack > read_messagepack(
      const T &str
    ) {
      hermit::spirit::qi::messagepack< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = boost::begin( str );
      messagepack result;
      if( boost::spirit::qi::parse(iter, boost::end( str ), grammar, result ) && iter == boost::end( str ) )
        return result;
      else
        return boost::optional< messagepack >();
    }
    
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

