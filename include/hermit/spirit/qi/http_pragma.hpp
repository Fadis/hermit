#ifndef HERMIT_SPIRIT_QI_HTTP_PRAGMA_HPP
#define HERMIT_SPIRIT_QI_HTTP_PRAGMA_HPP

#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <hermit/http_pragma.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class http_pragma :
          public boost::spirit::qi::grammar< InputIterator, hermit::http::pragma() > {
            public:
              http_pragma() : http_pragma::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace phx = boost::phoenix;
                separators = qi::standard::char_( "()<>@,;:\\\"/[]?={} \t" );
                token = +( qi::ascii::char_ - separators - qi::ascii::cntrl );
                text = ( qi::byte_ - qi::standard::cntrl ) | qi::standard::space;
                quoted_pair = '\\' >> qi::ascii::char_;
                quoted_string = '"' >> *( ( text - '"' )|quoted_pair ) >> '"';
                root = qi::skip( qi::standard::space )[
                  qi::lit( "Pragma" ) >> ':' >> ( (
                    qi::lit( "no-cache" )[ phx::at_c< 0 >( qi::_val ) = true ]|
                    ( token >> -( '=' >> ( quoted_string|token ) ) )[
                      phx::insert( phx::at_c< 1 >( qi::_val ), phx::construct< std::pair< std::string, boost::optional< std::string > > >( qi::_1, qi::_2 ) )
                    ]
                  ) % ',' )
                ][ qi::_pass = true ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, char() > separators;
              boost::spirit::qi::rule< InputIterator, char() > text;
              boost::spirit::qi::rule< InputIterator, char() > quoted_pair;
              boost::spirit::qi::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::qi::rule< InputIterator, std::string() > token;
              boost::spirit::qi::rule< InputIterator, hermit::http::pragma() > root;
      };
    }
  }
}

#endif

