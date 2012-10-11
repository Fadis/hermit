#ifndef HERMIT_SPIRIT_KARMA_HTTP_CONNECTION_HPP
#define HERMIT_SPIRIT_KARMA_HTTP_CONNECTION_HPP

#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename Iterator>
        class http_connection :
          public boost::spirit::karma::grammar< Iterator, std::vector< std::string >() > {
            public:
              http_connection() : http_connection::base_type( root ) {
                namespace karma = boost::spirit::karma;
                namespace phx = boost::phoenix;
                token_char = karma::ascii::char_[
                  karma::_pass = karma::_val >= ' ' && karma::_val != '(' &&
                                 karma::_val != ')' && karma::_val != '<' &&
                                 karma::_val != '>' && karma::_val != '@' &&
                                 karma::_val != '.' && karma::_val != ';' &&
                                 karma::_val != ':' && karma::_val != '\\' &&
                                 karma::_val != '"' && karma::_val != '/' &&
                                 karma::_val != '[' && karma::_val != ']' &&
                                 karma::_val != '?' && karma::_val != '=' &&
                                 karma::_val != '{' && karma::_val != '}' &&
                                 karma::_val != ' ',
                  karma::_1 = karma::_val
                ];
                token = +token_char;
                root = "Connection: " << ( token % ", " );
              }
            private:
              boost::spirit::karma::rule< Iterator, char() > token_char;
              boost::spirit::karma::rule< Iterator, std::string() > token;
              boost::spirit::karma::rule< Iterator, std::vector< std::string >() > root;
      };
    }
  }
}

#endif

