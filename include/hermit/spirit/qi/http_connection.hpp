#ifndef HERMIT_SPIRIT_QI_HTTP_CONNECTION_HPP
#define HERMIT_SPIRIT_QI_HTTP_CONNECTION_HPP

#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <hermit/http_cache_control.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class http_connection :
          public boost::spirit::qi::grammar< InputIterator, std::vector< std::string >() > {
            public:
              http_connection() : http_connection::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace phx = boost::phoenix;
                separators = qi::standard::char_( "()<>@,;:\\\"/[]?={} \t" );
                token = +( qi::ascii::char_ - separators - qi::ascii::cntrl );
                text = ( qi::byte_ - qi::standard::cntrl ) | qi::standard::space;
                quoted_pair = '\\' >> qi::ascii::char_;
                quoted_string = '"' >> *( ( text - '"' )|quoted_pair ) >> '"';
                root = qi::skip( qi::standard::space )[
                  qi::lit( "Connection" ) >> ':' >> ( token % ',' )
                ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, char() > separators;
              boost::spirit::qi::rule< InputIterator, char() > text;
              boost::spirit::qi::rule< InputIterator, char() > quoted_pair;
              boost::spirit::qi::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::qi::rule< InputIterator, std::string() > token;
              boost::spirit::qi::rule< InputIterator, std::vector< std::string >() > root;
      };
    }
  }
}

#endif

