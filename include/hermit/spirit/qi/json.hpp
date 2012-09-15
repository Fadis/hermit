#ifndef HERMIT_SPIRIT_QI_JSON_HPP
#define HERMIT_SPIRIT_QI_JSON_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/json.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      boost::spirit::qi::uint_parser<uint8_t, 16, 1, 2> hex2_p;
      template< typename Iterator >
      class json :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::json()
        > {
      public:
        json() : json::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;
          null_ = lit( "null" )[ _val = none_type() ];
          root = object|array;
          value_ = string_|double_|object|array|bool_|null_;
          escape_sequence = '\\' >> (
            char_( '"' )[ _val = '"' ] |
            char_( '\\' )[ _val = '\\' ] |
            char_( '/' )[ _val = '/' ] |
            char_( 'b' )[ _val = '\b' ] |
            char_( 'n' )[ _val = '\n' ] |
            char_( 'f' )[ _val = '\f' ] |
            char_( 'r' )[ _val = '\r' ] |
            char_( 't' )[ _val = '\t' ] |
            hex2_p[ _val = _1 ]
          );
          string_ = '"' >> *( ( char_ - cntrl - '\\' - '"' )|escape_sequence ) >> '"';
          array = skip( blank )[ '[' >> -( value_ % ',' ) >>  ']' ];
          named_value = skip( blank )[ ( string_ >> ':' >> value_ ) ];
          object = skip( blank )[ '{' >> -( named_value % ',' ) >> '}' ];
        } 
      private:
        boost::spirit::qi::rule< Iterator, hermit::json() > root;
        boost::spirit::qi::rule< Iterator, char() > escape_sequence;
        boost::spirit::qi::rule< Iterator, std::string() > string_;
        boost::spirit::qi::rule< Iterator, none_type() > null_;
        boost::spirit::qi::rule< Iterator, hermit::json() > value_;
        boost::spirit::qi::rule< Iterator, std::vector< hermit::json >() > array;
        boost::spirit::qi::rule< Iterator, std::pair< std::string, hermit::json >() > named_value;
        boost::spirit::qi::rule< Iterator, std::map< std::string, hermit::json >() > object;
      };
    }
  }
}

#endif

