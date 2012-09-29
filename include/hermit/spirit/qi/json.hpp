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
#include <hermit/format/read/utf8.hpp>
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
          namespace phx = boost::phoenix;
          root = ( qi::byte_ >> qi::byte_ >> qi::byte_ >> qi::byte_ )[
            phx::if_( qi::_1 == 0 )[
              phx::if_( qi::_2 == 0 )[
                phx::if_( qi::_3 == 0 )[
                  qi::_val = hermit::json_utf32be
                ].else_[
                  qi::_val = hermit::json_utf32
                ]
              ].else_[
                qi::_val = hermit::json_utf16be
              ]
            ].else_[
              phx::if_( qi::_2 == 0 )[
                phx::if_( qi::_3 == 0 )[
                  qi::_val = hermit::json_utf32le
                ].else_[
                  qi::_val = hermit::json_utf16le
                ]
              ].else_[
                phx::if_( qi::_3 == 0 )[
                  phx::if_( qi::_4 == 0 )[
                    qi::_val = hermit::json_utf32
                  ].else_[
                    qi::_val = hermit::json_utf16
                  ]
                ].else_[
                  phx::if_( qi::_4 == 0 )[
                    qi::_val = hermit::json_utf16
                  ].else_[
                    qi::_val = hermit::json_utf8
                  ]
                ]
              ]
            ]
          ];
        }
      private:
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
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          null_ = qi::lit( "null" )[ qi::_val = none_type() ];
          root = qi::omit[ *qi::standard::space ] >> (object|array) >> qi::omit[ *qi::standard::space ];
          value_ = string_|qi::double_|object|array|qi::bool_|null_;
          escape_sequence = '\\' >> (
            qi::standard::char_( '"' )[ qi::_val = '"' ] |
            qi::standard::char_( '\\' )[ qi::_val = '\\' ] |
            qi::standard::char_( '/' )[ qi::_val = '/' ] |
            qi::standard::char_( 'b' )[ qi::_val = '\b' ] |
            qi::standard::char_( 'n' )[ qi::_val = '\n' ] |
            qi::standard::char_( 'f' )[ qi::_val = '\f' ] |
            qi::standard::char_( 'r' )[ qi::_val = '\r' ] |
            qi::standard::char_( 't' )[ qi::_val = '\t' ]
          );
          unicode_string = +(
            "\\u" >> hex2_p >> hex2_p
          )[
            phx::push_back( qi::_a, qi::_1 ),
            phx::push_back( qi::_a, qi::_2 )
          ] >> qi::eps[
            qi::_val = phx::bind( &json::utf16_to_utf8, qi::_a, qi::_pass )
          ];
          string_ = string_utf8[
            qi::_val = phx::bind( &json::utf8_to_ucs4, qi::_1, qi::_pass )
          ];
          string_utf8 = '"' >> *( (
            qi::repeat(1)[
              ( qi::standard::char_ - qi::standard::cntrl - '\\' - '"' )|
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
          array = qi::skip( qi::standard::space )[ '[' >> -( value_ % ',' ) >>  ']' ];
          named_value = qi::skip( qi::standard::space )[ ( string_ >> ':' >> value_ ) ];
          object = qi::skip( qi::standard::space )[ '{' >> -( named_value % ',' ) >> '}' ];
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
        static std::u32string utf8_to_ucs4( const std::string &src, bool &_pass ) {
          const auto ucs4_string = hermit::format::read_utf8( src );
          if( !ucs4_string ) {
            _pass = false;
            return std::u32string();
          }
          _pass = true;
          return *ucs4_string;
        }
        boost::spirit::qi::rule< Iterator, hermit::json() > root;
        boost::spirit::qi::rule< Iterator, char() > escape_sequence;
        boost::spirit::qi::rule< Iterator, std::string() > string_utf8;
        boost::spirit::qi::rule< Iterator, std::u32string() > string_;
        boost::spirit::qi::rule< Iterator, std::string(), boost::spirit::qi::locals< std::vector< uint8_t > > > unicode_string;
        boost::spirit::qi::rule< Iterator, none_type() > null_;
        boost::spirit::qi::rule< Iterator, hermit::json() > value_;
        boost::spirit::qi::rule< Iterator, std::vector< hermit::json >() > array;
        boost::spirit::qi::rule< Iterator, std::pair< std::u32string, hermit::json >() > named_value;
        boost::spirit::qi::rule< Iterator, std::map< std::u32string, hermit::json >() > object;
      };
    }
  }
}

#endif

