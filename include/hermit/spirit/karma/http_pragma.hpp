#ifndef HERMIT_SPIRIT_KARMA_HTTP_PRAGMA_HPP
#define HERMIT_SPIRIT_KARMA_HTTP_PRAGMA_HPP

#include <iterator>
#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/http_pragma.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename Iterator>
        class http_pragma :
          public boost::spirit::karma::grammar< Iterator, hermit::http::pragma() > {
            public:
              typedef boost::fusion::vector3< std::string, boost::optional< unsigned int >, boost::optional< std::string > > attr_type;
              http_pragma() : http_pragma::base_type( root ) {
                namespace karma = boost::spirit::karma;
                namespace phx = boost::phoenix;
                token_char = karma::ascii::char_[
                  karma::_pass = karma::_val >= ' ' && karma::_val != '(' && karma::_val != ')' &&
                    karma::_val != '<' && karma::_val != '>' && karma::_val != '@' && karma::_val != '.' &&
                    karma::_val != ';' && karma::_val != ':' && karma::_val != '\\' && karma::_val != '"' &&
                    karma::_val != '/' && karma::_val != '[' && karma::_val != ']' && karma::_val != '?' &&
                    karma::_val != '=' && karma::_val != '{' && karma::_val != '}' && karma::_val != ' ',
                  karma::_1 = karma::_val
                ];
                token = +token_char;
                text_char = karma::byte_[
                  karma::_pass = karma::_val >= ' ' && karma::_val != '"',
                  karma::_1 = karma::_val
                ];
                text = text_char | karma::standard::space;
                quoted_pair = '\\' << karma::ascii::char_;
                quoted_string = '"' << *( text|quoted_pair ) << '"';
                root = ( "Pragma: " <<
                  ( ( karma::string << -( '=' << karma::uint_ ) << -( '=' << quoted_string ) ) % ", " ) )[
                  phx::if_( phx::at_c< 0 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-cache", boost::none, boost::none ) )
                  ],
                  phx::bind( &http_pragma::build_extensions, phx::at_c< 1 >( karma::_val ), karma::_1, karma::_pass )
                ];
              }
            private:
              static void build_extensions( const std::map< std::string, boost::optional< std::string > > src, std::vector< attr_type > &dest, bool &_pass ) {
                std::transform( src.begin(), src.end(), std::back_inserter( dest ), [&]( const std::pair< std::string, boost::optional< std::string > > &src ) -> attr_type { _pass = !src.first.empty(); return attr_type( src.first, boost::none, src.second ); } );
              }
              boost::spirit::karma::rule< Iterator, char() > text_char;
              boost::spirit::karma::rule< Iterator, char() > text;
              boost::spirit::karma::rule< Iterator, char() > quoted_pair;
              boost::spirit::karma::rule< Iterator, std::string() > quoted_string;
              boost::spirit::karma::rule< Iterator, char() > token_char;
              boost::spirit::karma::rule< Iterator, std::string() > token;
              boost::spirit::karma::rule< Iterator, hermit::http::pragma() > root;
      };
    }
  }
}

#endif

