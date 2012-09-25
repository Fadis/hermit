#ifndef HERMIT_FORMAT_READ_URI_HPP
#define HERMIT_FORMAT_READ_URI_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/uri.hpp>
#include <hermit/spirit/qi/uri.hpp>

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
    boost::optional< hermit::uri > read_uri(
      const T &str
    ) {
      hermit::spirit::qi::uri< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      hermit::uri result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< hermit::uri >();
    }
  }
}

#endif

