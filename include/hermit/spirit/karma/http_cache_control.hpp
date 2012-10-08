#ifndef HERMIT_SPIRIT_KARMA_HTTP_CACHE_CONTROL_HPP
#define HERMIT_SPIRIT_KARMA_HTTP_CACHE_CONTROL_HPP

#include <iterator>
#include <vector>
#include <string>
#include <utility>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/http_cache_control.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename InputIterator>
        class http_cache_control_request :
          public boost::spirit::karma::grammar< InputIterator, hermit::http::cache_control_request() > {
            public:
              typedef boost::fusion::vector3< std::string, boost::optional< unsigned int >, boost::optional< std::string > > attr_type;
              http_cache_control_request() : http_cache_control_request::base_type( root ) {
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
                root = ( "Cache-Control: " <<
                  ( ( karma::string << -( '=' << karma::uint_ ) << -( '=' << quoted_string ) ) % ", " ) )[
                  phx::if_( phx::at_c< 0 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-cache", boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 1 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-store", boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 2 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "max-age", phx::at_c< 2 >( karma::_val ), boost::none ) )
                  ],
                  phx::if_( phx::at_c< 3 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "max-stale", phx::at_c< 4 >( karma::_val ), boost::none ) )
                  ],
                  phx::if_( phx::at_c< 5 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "min-fresh", phx::at_c< 5 >( karma::_val ), boost::none ) )
                  ],
                  phx::if_( phx::at_c< 6 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-transform", boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 7 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-transform", boost::none, boost::none ) )
                  ],
                  phx::bind( &http_cache_control_request::build_extensions, phx::at_c< 8 >( karma::_val ), karma::_1, karma::_pass )
                ];
              }
            private:
              static void build_extensions( const std::map< std::string, boost::optional< std::string > > src, std::vector< attr_type > &dest, bool &_pass ) {
                std::transform( src.begin(), src.end(), std::back_inserter( dest ), [&]( const std::pair< std::string, boost::optional< std::string > > &src ) -> attr_type { _pass = !src.first.empty(); return attr_type( src.first, boost::none, src.second ); } );
              }
              boost::spirit::karma::rule< InputIterator, char() > text_char;
              boost::spirit::karma::rule< InputIterator, char() > text;
              boost::spirit::karma::rule< InputIterator, char() > quoted_pair;
              boost::spirit::karma::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::karma::rule< InputIterator, char() > token_char;
              boost::spirit::karma::rule< InputIterator, std::string() > token;
              boost::spirit::karma::rule< InputIterator, hermit::http::cache_control_request() > root;
      };


      template <typename InputIterator>
        class http_cache_control_response :
          public boost::spirit::karma::grammar< InputIterator, hermit::http::cache_control_response() > {
            public:
              typedef boost::fusion::vector4< std::string, boost::optional< unsigned int >, boost::optional< std::string >, boost::optional< std::vector< std::string > > > attr_type;
              http_cache_control_response() : http_cache_control_response::base_type( root ) {
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
                tokens = '"' << ( token % ", " ) << '"';
                text_char = karma::byte_[
                  karma::_pass = karma::_val >= ' ' && karma::_val != '"',
                  karma::_1 = karma::_val
                ];
                text = text_char | karma::standard::space;
                quoted_pair = '\\' << karma::ascii::char_;
                quoted_string = '"' << *( text|quoted_pair ) << '"';
                root = ( "Cache-Control: " <<
                  ( ( karma::string << -( '=' << karma::uint_ ) <<
                                       -( '=' << quoted_string ) <<
                                       -( '=' << tokens ) ) % ", " ) )[
                  phx::if_( phx::at_c< 0 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "public", boost::none, boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 1 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "private", boost::none, boost::none, phx::at_c< 2 >( karma::_val ) ) )
                  ],
                  phx::if_( phx::at_c< 3 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-cache", boost::none, boost::none, phx::at_c< 4 >( karma::_val ) ) )
                  ],
                  phx::if_( phx::at_c< 5 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-store", boost::none, boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 6 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "no-transform", boost::none, boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 7 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "must-revalidate", boost::none, boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 8 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "proxy-revalidate", boost::none, boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 9 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "max-age", phx::at_c< 9 >( karma::_val ), boost::none, boost::none ) )
                  ],
                  phx::if_( phx::at_c< 10 >( karma::_val ) )[
                    phx::push_back( karma::_1,
                      phx::construct< attr_type >( "max-age", phx::at_c< 10 >( karma::_val ), boost::none, boost::none ) )
                  ],
                  phx::bind( &http_cache_control_response::build_extensions, phx::at_c< 11 >( karma::_val ), karma::_1, karma::_pass )
                ];
              }
            private:
              static void build_extensions( const std::map< std::string, boost::optional< std::string > > src, std::vector< attr_type > &dest, bool &_pass ) {
                std::transform( src.begin(), src.end(), std::back_inserter( dest ), [&]( const std::pair< std::string, boost::optional< std::string > > &src ) -> attr_type { _pass = !src.first.empty(); return attr_type( src.first, boost::none, src.second, boost::none ); } );
              }
              boost::spirit::karma::rule< InputIterator, char() > separators;
              boost::spirit::karma::rule< InputIterator, char() > text_char;
              boost::spirit::karma::rule< InputIterator, char() > text;
              boost::spirit::karma::rule< InputIterator, char() > quoted_pair;
              boost::spirit::karma::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::karma::rule< InputIterator, char() > token_char;
              boost::spirit::karma::rule< InputIterator, std::string() > token;
              boost::spirit::karma::rule< InputIterator, std::vector< std::string >() > tokens;
              boost::spirit::karma::rule< InputIterator, hermit::http::cache_control_response() > root;
      };

/*
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
              boost::spirit::karma::rule< InputIterator, char() > separators;
              boost::spirit::karma::rule< InputIterator, char() > text;
              boost::spirit::karma::rule< InputIterator, char() > quoted_pair;
              boost::spirit::karma::rule< InputIterator, std::string() > quoted_string;
              boost::spirit::karma::rule< InputIterator, std::string() > token;
              boost::spirit::karma::rule< InputIterator, std::vector< std::string >() > tokens;
              boost::spirit::karma::rule< InputIterator, hermit::http::cache_control_response() > root;
      };
*/
    }
  }
}

#endif

