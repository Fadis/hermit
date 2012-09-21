#ifndef HERMIT_FORMAT_READ_JSON_HPP
#define HERMIT_FORMAT_READ_JSON_HPP

#include <string>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <hermit/range_traits.hpp>
#include <hermit/json.hpp>
#include <hermit/spirit/qi/json.hpp>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/read/utf32.hpp>
#include <hermit/format/write/utf8.hpp>

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
    boost::optional< json > read_json_utf8(
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
    boost::optional< std::string > convert_to_json_utf8(
      const T &str, hermit::json_encoding_type encoding
    ) {
      boost::optional< std::u32string > str_in_ucs4;
      if( encoding == hermit::json_utf32 )
        str_in_ucs4 = hermit::format::read_utf32( str );
      else if( encoding = hermit::json_utf32be )
        str_in_ucs4 = hermit::format::read_utf32be( str );
      else if( encoding = hermit::json_utf32le )
        str_in_ucs4 = hermit::format::read_utf32le( str );
      else if( encoding == hermit::json_utf16 )
        str_in_ucs4 = hermit::format::read_utf16( str );
      else if( encoding = hermit::json_utf16be )
        str_in_ucs4 = hermit::format::read_utf16be( str );
      else if( encoding = hermit::json_utf16le )
        str_in_ucs4 = hermit::format::read_utf16le( str );
      if( !str_in_ucs4 )
        return boost::optional< std::string >();
      return hermit::format::write_utf8( *str_in_ucs4 );
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
    boost::optional< json > read_json(
      const T &str
    ) {
      hermit::json_encoding_type encoding = hermit::json_utf8;
      {
        hermit::spirit::qi::json_encoding< typename range_iterator< const T >::type > encoding_detector;
        typename range_iterator< const T >::type iter = str.begin();
        boost::spirit::qi::parse(iter, str.end(), encoding_detector, encoding );
      }
      if( encoding == hermit::json_utf8 )
        return read_json_utf8( str );
      else {
        const boost::optional< std::string > str_in_utf8 = convert_to_json_utf8( str, encoding );
        if( !str_in_utf8 )
          return boost::optional< json >();
        return read_json_utf8( *str_in_utf8 );
      }
    }
  }
}

#endif

