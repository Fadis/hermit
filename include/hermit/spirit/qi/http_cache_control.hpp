#ifndef HERMIT_SPIRIT_QI_HTTP_CACHE_CONTROL_HPP
#define HERMIT_SPIRIT_QI_HTTP_CACHE_CONTROL_HPP

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
        class http_cache_control_request :
          public boost::spirit::qi::grammar< InputIterator, hermit::http::cache_control_request() > {
            public:
              http_cache_control_request() : http_cache_control_request::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace phx = boost::phoenix;
                separators = qi::standard::char_( "()<>@,;:\\\"/[]?={} \t" );
                token = +( qi::ascii::char_ - separators - qi::ascii::cntrl );
                text = ( qi::byte_ - qi::standard::cntrl ) | qi::standard::space;
                quoted_pair = '\\' >> qi::ascii::char_;
                quoted_string = '"' >> *( ( text - '"' )|quoted_pair ) >> '"';
                root = qi::skip( qi::standard::space )[
                  qi::lit( "Cache-Control" ) >> ':' >> ( (
                    qi::lit( "no-cache" )[ phx::at_c< 0 >( qi::_val ) = true ]|
                    qi::lit( "no-store" )[ phx::at_c< 1 >( qi::_val ) = true ]|
                    ( qi::lit( "max-age" ) >> '=' >> qi::uint_ )[ phx::at_c< 2 >( qi::_val ) = qi::_1 ]|
                    ( qi::lit( "max-stale" ) >> -( '=' >> qi::uint_ ) )[
                      phx::at_c< 3 >( qi::_val ) = true,
                      phx::at_c< 4 >( qi::_val ) = qi::_1
                    ]|
                    ( qi::lit( "min-fresh" ) >> '=' >> qi::uint_ )[ phx::at_c< 5 >( qi::_val ) = qi::_1 ]|
                    qi::lit( "no-transform" )[ phx::at_c< 6 >( qi::_val ) = true ]|
                    qi::lit( "only-if=cached" )[ phx::at_c< 7 >( qi::_val ) = true ]|
                    ( token >> -( '=' >> ( quoted_string|token ) ) )[
                      phx::insert( phx::at_c< 8 >( qi::_val ), phx::construct< std::pair< std::string, boost::optional< std::string > > >( qi::_1, qi::_2 ) )
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
              boost::spirit::qi::rule< InputIterator, hermit::http::cache_control_request() > root;
      };
      template <typename InputIterator>
        class http_cache_control_response :
          public boost::spirit::qi::grammar< InputIterator, hermit::http::cache_control_response() > {
            public:
              http_cache_control_response() : http_cache_control_response::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace phx = boost::phoenix;
                separators = qi::standard::char_( "()<>@,;:\\\"/[]?={} \t" );
                token = *( qi::ascii::char_ - separators - qi::ascii::cntrl );
                tokens = qi::skip( qi::standard::space )[ '"' >> ( token % ',' ) >> '"' ];
                text = ( qi::byte_ - qi::standard::cntrl ) | qi::standard::space;
                quoted_pair = '\\' >> qi::ascii::char_;
                quoted_string = '"' >> *( ( text - '"' )|quoted_pair ) >> '"';
                root = qi::skip( qi::standard::space )[
                  qi::lit( "Cache-Control" ) >> ':' >> ( (
                    qi::lit( "public" )[ phx::at_c< 0 >( qi::_val ) = true ]|
                    ( qi::lit( "private" ) >>
                      '=' >> tokens )[
                      phx::at_c< 1 >( qi::_val ) = true,
                      phx::insert( phx::at_c< 2 >( qi::_val ), phx::end( phx::at_c< 2 >( qi::_val ) ),
                                   phx::begin( qi::_1 ), phx::end( qi::_1 ) )
                    ]|
                    qi::lit( "private" )[
                      phx::at_c< 1 >( qi::_val ) = true
                    ]|
                    ( qi::lit( "no-cache" ) >>
                      '=' >> tokens )[
                      phx::at_c< 3 >( qi::_val ) = true,
                      phx::insert( phx::at_c< 4 >( qi::_val ), phx::end( phx::at_c< 4 >( qi::_val ) ),
                                   phx::begin( qi::_1 ), phx::end( qi::_1 ) )
                    ]|
                    qi::lit( "no-cache" )[
                      phx::at_c< 3 >( qi::_val ) = true
                    ]|
                    qi::lit( "no-store" )[ phx::at_c< 5 >( qi::_val ) = true ]|
                    qi::lit( "no-transform" )[ phx::at_c< 6 >( qi::_val ) = true ]|
                    qi::lit( "must-revalidate" )[ phx::at_c< 7 >( qi::_val ) = true ]|
                    qi::lit( "proxy-revalidate" )[ phx::at_c< 8 >( qi::_val ) = true ]|
                    ( qi::lit( "max-age" ) >> '=' >> qi::uint_ )[
                      phx::at_c< 9 >( qi::_val ) = qi::_1
                    ]|
                    ( qi::lit( "s-maxage" ) >> '=' >> qi::uint_ )[
                      phx::at_c< 10 >( qi::_val ) = qi::_1
                    ]|
                    ( token >> -( '=' >> ( quoted_string|token ) ) )[
                      phx::insert( phx::at_c< 11 >( qi::_val ), phx::construct< std::pair< std::string, boost::optional< std::string > > >( qi::_1, qi::_2 ) )
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
              boost::spirit::qi::rule< InputIterator, std::vector< std::string >() > tokens;
              boost::spirit::qi::rule< InputIterator, hermit::http::cache_control_response() > root;
      };
    }
  }
}

#endif

