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

namespace hermit {
  namespace spirit {
    namespace karma {
      template < typename Iterator >
      class json :
        public boost::spirit::karma::grammar< Iterator, hermit::json() > {
        public:
          json() : json::base_type( value_ ) {
            namespace karma = boost::spirit::karma;
            null_ = karma::string[ karma::_1 = boost::phoenix::val( "null" ) ];
            root = object|array;
            value_ = string_|karma::double_|object|array|karma::bool_|null_;
            escape_sequence.add( '"', "\\\"" )( '\\', "\\\\" )( '/', "\\/" )
                ( '\b', "\\b" )( '\n', "\\n" )( '\f', "\\f" )
                ( '\r', "\\r" )( '\t', "\\t" );
            string_ = '"' << *( escape_sequence | karma::print | "\\" << karma::hex ) << '"';
            array = '[' << -( value_ % ',' ) << ']';
            named_value = ( string_ << ':' << value_ );
            object = '{' << -( named_value % ',' ) << '}';
          }
        private:
          boost::spirit::karma::rule< Iterator, hermit::json() > root;
          boost::spirit::karma::symbols<char, std::string> escape_sequence;
          boost::spirit::karma::rule<Iterator, std::string()> string_;
          boost::spirit::karma::rule< Iterator, std::nullptr_t> null_;
          boost::spirit::karma::rule< Iterator, hermit::json() > value_;
          boost::spirit::karma::rule< Iterator, std::vector< hermit::json >() > array;
          boost::spirit::karma::rule< Iterator, std::pair< std::string, hermit::json >() > named_value;
          boost::spirit::karma::rule< Iterator, std::map< std::string, hermit::json >() > object;
      };
    }
  }
}

#endif

