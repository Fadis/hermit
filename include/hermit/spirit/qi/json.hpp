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
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;

      template< typename Iterator >
      class json_encoding :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::json_encoding_type()
        > {
      public:
        json_encoding() : json_encoding::base_type( root ) {
          namespace qi = boost::spirit::qi;
          root = qi::big_dword[
            qi::_pass = qi::_1 == 0xfeff,
            qi::_val = hermit::json_utf32
          ]|
          qi::little_dword[
            qi::_pass = qi::_1 == 0xfeff,
            qi::_val = hermit::json_utf32
          ]|
          qi::big_word[
            qi::_pass = qi::_1 == 0xfeff,
            qi::_val = hermit::json_utf16
          ]|
          qi::little_word[
            qi::_pass = qi::_1 == 0xfeff,
            qi::_val = hermit::json_utf16
          ]|
          qi::big_dword[
            qi::_pass = qi::_1 <= 0x7F,
            qi::_val = hermit::json_utf32be
          ]|
          qi::little_dword[
            qi::_pass = qi::_1 <= 0x7F,
            qi::_val = hermit::json_utf32le
          ]|
          qi::big_word[
            qi::_pass = qi::_1 <= 0x7F,
            qi::_val = hermit::json_utf16be
          ]|
          qi::little_word[
            qi::_pass = qi::_1 <= 0x7F,
            qi::_val = hermit::json_utf16le
          ]|
          qi::byte_[
            qi::_pass = qi::_1 <= 0x7F,
            qi::_val = hermit::json_utf8
          ];
        }
        boost::spirit::qi::rule< Iterator, hermit::json_encoding_type() > root;
      };

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
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
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
          unicode_string = +(
            "\\u" >> hex2_p >> hex2_p
          )[
            phx::push_back( qi::_a, qi::_1 ),
            phx::push_back( qi::_a, qi::_2 )
          ] >> eps[
            qi::_val = phx::bind( &json::utf16_to_utf8, qi::_a, qi::_pass )
          ];
          string_ = '"' >> *( (
            repeat(1)[
              ( char_ - cntrl - '\\' - '"' )|
              escape_sequence
            ] )[
              phx::insert(
                qi::_val,
                phx::end( qi::_val ),
                phx::begin( qi::_1 ),
                phx::end( qi::_1 )
              )
            ]|unicode_string[
              phx::insert(
                qi::_val,
                phx::end( qi::_val ),
                phx::begin( qi::_1 ),
                phx::end( qi::_1 )
              )
            ] ) >> '"';
          array = skip( blank )[ '[' >> -( value_ % ',' ) >>  ']' ];
          named_value = skip( blank )[ ( string_ >> ':' >> value_ ) ];
          object = skip( blank )[ '{' >> -( named_value % ',' ) >> '}' ];
        } 
      private:
        static std::string utf16_to_utf8( const std::vector< uint8_t > &src, bool &_pass ) {
          const auto ucs4_string = hermit::format::read_utf16be( src );
          if( !ucs4_string ) {
            _pass = false;
            return "";
          }
          const auto utf8_string = hermit::format::write_utf8( *ucs4_string );
          if( !utf8_string ) {
            _pass = false;
            return "";
          }
          _pass = true;
          return *utf8_string;
        }
        boost::spirit::qi::rule< Iterator, hermit::json() > root;
        boost::spirit::qi::rule< Iterator, char() > escape_sequence;
        boost::spirit::qi::rule< Iterator, std::string() > string_;
        boost::spirit::qi::rule< Iterator, std::string(), boost::spirit::qi::locals< std::vector< uint8_t > > > unicode_string;
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

