#ifndef HERMIT_SPIRIT_QI_HTTP_TRANSFER_ENCODING_HPP
#define HERMIT_SPIRIT_QI_HTTP_TRANSFER_ENCODING_HPP

#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/http_transfer_encoding.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class http_transfer_encoding :
          public boost::spirit::qi::grammar< InputIterator, std::vector< hermit::http::transfer_encoding >() > {
            public:
              http_transfer_encoding() : http_transfer_encoding::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace phx = boost::phoenix;
                separators = qi::standard::char_( "()<>@,;:\\\"/[]?={} \t" );
                token = +( qi::ascii::char_ - separators - qi::ascii::cntrl );
                text = ( qi::byte_ - qi::standard::cntrl ) | qi::standard::space;
                quoted_pair = '\\' >> qi::ascii::char_;
                quoted_string = '"' >> *( ( text - '"' )|quoted_pair ) >> '"';
                parameter = qi::skip( qi::standard::space )[
                  token >> '=' >> (token|quoted_string)
                ];
                transfer_encoding = qi::skip( qi::standard::space )[
                  token >> *( ';' >> parameter )
                ];
                root = qi::skip( qi::standard::space )[
                  qi::lit( "Transfer-Encoding" ) >> ':' >> ( transfer_encoding % ',' )
                ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, char() > separators;
              boost::spirit::qi::rule< InputIterator, char() > text;
              boost::spirit::qi::rule< InputIterator, char() > quoted_pair;
              boost::spirit::qi::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::qi::rule< InputIterator, std::string() > token;
              boost::spirit::qi::rule< InputIterator, std::pair< std::string, std::string >() > parameter;
              boost::spirit::qi::rule< InputIterator, hermit::http::transfer_encoding() > transfer_encoding;
              boost::spirit::qi::rule< InputIterator, std::vector< hermit::http::transfer_encoding >() > root;
      };
    }
  }
}

#endif

