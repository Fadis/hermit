#ifndef HERMIT_SPIRIT_KARMA_JSON_HPP
#define HERMIT_SPIRIT_KARMA_JSON_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/json.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template < typename Iterator >
      class json :
        public boost::spirit::karma::grammar< Iterator, hermit::json() > {
        public:
          json() : json::base_type( value_ ) {
            namespace karma = boost::spirit::karma;
            namespace karma = boost::spirit::karma;
            namespace phx = boost::phoenix;
            null_ = karma::string[ karma::_1 = boost::phoenix::val( "null" ) ];
            root = object|array;
            value_ = string_|karma::double_|object|array|karma::bool_|null_;
            escape_sequence.add( '"', "\\\"" )( '\\', "\\\\" )( '/', "\\/" )
                ( '\b', "\\b" )( '\n', "\\n" )( '\f', "\\f" )
                ( '\r', "\\r" )( '\t', "\\t" );
            string_utf8 = '"' <<
              *( escape_sequence | karma::byte_ )
            << '"';
            string_ = string_utf8[
              karma::_1 = phx::bind( &json::ucs4_to_utf8, karma::_val, karma::_pass )
            ];
            array = '[' << -( value_ % ',' ) << ']';
            named_value = ( string_ << ':' << value_ );
            object = '{' << -( named_value % ',' ) << '}';
          }
        private:
        static std::string ucs4_to_utf8( const std::u32string &src, bool &_pass ) {
          const auto utf8_string = hermit::format::write_utf8( src );
          if( !utf8_string ) {
            _pass = false;
            return std::string();
          }
          _pass = true;
          return *utf8_string;
        }
          boost::spirit::karma::rule< Iterator, hermit::json() > root;
          boost::spirit::karma::symbols<char, std::string> escape_sequence;
          boost::spirit::karma::rule<Iterator, std::string() > string_utf8;
          boost::spirit::karma::rule<Iterator, std::u32string() > string_;
          boost::spirit::karma::rule< Iterator, none_type()> null_;
          boost::spirit::karma::rule< Iterator, hermit::json() > value_;
          boost::spirit::karma::rule< Iterator, std::vector< hermit::json >() > array;
          boost::spirit::karma::rule< Iterator, std::pair< std::u32string, hermit::json >() > named_value;
          boost::spirit::karma::rule< Iterator, std::map< std::u32string, hermit::json >() > object;
      };
    }
  }
}

#endif

