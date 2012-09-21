#ifndef HERMIT_FORMAT_READ_UTF32_HPP
#define HERMIT_FORMAT_READ_UTF32_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/json.hpp>
#include <hermit/spirit/qi/utf32.hpp>

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
    boost::optional< std::u32string > read_utf32le(
      const T &str
    ) {
      hermit::spirit::qi::utf32lestring< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      std::u32string result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< std::u32string >();
    }
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
    boost::optional< std::u32string > read_utf16be(
      const T &str
    ) {
      hermit::spirit::qi::utf32bestring< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      std::u32string result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< std::u32string >();
    }
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
    boost::optional< std::u32string > read_utf32(
      const T &str
    ) {
      hermit::spirit::qi::utf32string< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      std::u32string result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< std::u32string >();
    }
  }
}

#endif

