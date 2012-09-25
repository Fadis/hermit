#ifndef HERMIT_SPIRIT_KARMA_URI
#define HERMIT_SPIRIT_KARMA_URI

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <hermit/spirit/karma/ipv4.hpp>
#include <hermit/spirit/karma/ipv6.hpp>
#include <hermit/uri.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename Iterator >
        class reg_name : public boost::spirit::karma::grammar< Iterator, std::string() > {
          public:
            reg_name() : reg_name::base_type( root ) {
              namespace karma = boost::spirit::karma;
              sub_delims = karma::standard::char_("!$&'()*+,;=");
              unreserved = karma::standard::alnum | karma::standard::char_("-._~");
              pct_encoded = '%' << hex_p;
              root = *( sub_delims|unreserved|pct_encoded );
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 16> hex_p;
            boost::spirit::karma::rule< Iterator, char() > sub_delims; 
            boost::spirit::karma::rule< Iterator, char() > unreserved; 
            boost::spirit::karma::rule< Iterator, char() > pct_encoded; 
            boost::spirit::karma::rule< Iterator, std::string() > root; 
        };

      template< typename Iterator >
        class host : public boost::spirit::karma::grammar< Iterator, hermit::host() > {
          public:
            host() : host::base_type( root ) {
              root = ipv4address|( '[' << ipv6address << ']' )|hostname;
            }
          private:
            ipv4< Iterator > ipv4address;
            ipv6< Iterator > ipv6address;
            reg_name< Iterator > hostname;
            boost::spirit::karma::rule< Iterator, hermit::host() > root;
        };

      template< typename Iterator >
        class userinfo : public boost::spirit::karma::grammar< Iterator, std::vector< std::string >() > {
          public:
            userinfo() : userinfo::base_type( root ) {
              namespace karma = boost::spirit::karma;
              sub_delims = karma::standard::char_("!$&'()*+,;=");
              unreserved = karma::standard::alnum | karma::standard::char_("-._~");
              pct_encoded = '%' << hex_p;
              info = +( sub_delims|unreserved|pct_encoded );
              root = info % ':';
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 16> hex_p;
            boost::spirit::karma::rule< Iterator, char() > sub_delims; 
            boost::spirit::karma::rule< Iterator, char() > unreserved; 
            boost::spirit::karma::rule< Iterator, char() > pct_encoded; 
            boost::spirit::karma::rule< Iterator, std::string() > info;
            boost::spirit::karma::rule< Iterator, std::vector< std::string >() > root;
        };

      template< typename Iterator >
        class authority : public boost::spirit::karma::grammar< Iterator, hermit::authority() > {
          public:
            authority() : authority::base_type( root ) {
              root = ( -( userinfo_ << '@' ) << host_ << -( ':' << dec_p ) );
            }
          private:
            boost::spirit::karma::uint_generator<uint16_t, 10> dec_p;
            userinfo< Iterator > userinfo_;
            host< Iterator > host_;
            boost::spirit::karma::rule< Iterator, hermit::authority() > root;
        };

      template< typename Iterator, bool nz, bool nc >
        class segment : public boost::spirit::karma::grammar< Iterator, std::string() > {
          public:
            segment() : segment::base_type( root ) {
              namespace karma = boost::spirit::karma;
              sub_delims = karma::standard::char_("!$&'()*+,;=");
              unreserved = karma::standard::alnum | karma::standard::char_("-._~");
              pct_encoded = '%' << hex_p;
              if( nc )
                pchar = sub_delims|unreserved|pct_encoded|karma::standard::char_("@");
              else
                pchar = sub_delims|unreserved|pct_encoded|karma::standard::char_(":@");
              if( nz )
                root = +pchar;
              else
                root = *pchar;
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 16> hex_p;
            boost::spirit::karma::rule< Iterator, char() > sub_delims; 
            boost::spirit::karma::rule< Iterator, char() > unreserved; 
            boost::spirit::karma::rule< Iterator, char() > pct_encoded; 
            boost::spirit::karma::rule< Iterator, char() > pchar; 
            boost::spirit::karma::rule< Iterator, std::string() > root; 
        };

      template< typename Iterator, bool absolute, bool accept_empty >
        class path : public boost::spirit::karma::grammar< Iterator, hermit::path() > {
          public:
            path() : path::base_type( root ) {
              namespace karma = boost::spirit::karma;
              namespace phx = boost::phoenix;
              if( absolute ) {
                if( accept_empty )
                  root = ( *( '/' << segment_ ) )[
                    karma::_pass = phx::at_c< 0 >( karma::_val ) == true,
                    karma::_1 = phx::at_c< 1 >( karma::_val )
                  ];
                else
                  root = ( *( '/' << segment_ ) )[
                    karma::_pass = phx::at_c< 0 >( karma::_val ) == true &&
                      phx::size( phx::at_c< 1 >( karma::_val ) ) > 0ul,
                    karma::_1 = phx::at_c< 1 >( karma::_val )
                  ];
              }
              else {
                root = ( segment_ % '/' )[
                  karma::_pass = phx::at_c< 0 >( karma::_val ) == false &&
                    phx::size( phx::at_c< 1 >( karma::_val ) ) > 0ul &&
                    phx::size( phx::at( phx::at_c< 1 >( karma::_val ), 0 ) ) > 0ul,
                  karma::_1 = phx::at_c< 1 >( karma::_val )
                ];
              }
            }
          private:
            segment< Iterator, false, false > segment_;
            boost::spirit::karma::rule< Iterator, hermit::path() > root;
      };

      template< typename Iterator >
        class query : public boost::spirit::karma::grammar< Iterator, std::string() > {
          public:
            query() : query::base_type( root ) {
              namespace karma = boost::spirit::karma;
              namespace phx = boost::phoenix;
              sub_delims = karma::standard::char_("!$&'()*+,;=");
              unreserved = karma::standard::alnum | karma::standard::char_("-._~");
              pct_encoded = '%' << hex_p;
              pchar = sub_delims|unreserved|pct_encoded|karma::standard::char_(":@");
              root = *( pchar | karma::standard::char_("/?") );
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 16> hex_p;
            boost::spirit::karma::rule< Iterator, char() > sub_delims;
            boost::spirit::karma::rule< Iterator, char() > unreserved;
            boost::spirit::karma::rule< Iterator, char() > pct_encoded;
            boost::spirit::karma::rule< Iterator, char() > pchar;
            boost::spirit::karma::rule< Iterator, std::string() > root;
        };

      template< typename Iterator >
        class fragment : public boost::spirit::karma::grammar< Iterator, std::string() > {
          public:
            fragment() : fragment::base_type( root ) {
              namespace karma = boost::spirit::karma;
              sub_delims = karma::standard::char_("!$&'()*+,;=");
              unreserved = karma::standard::alnum | karma::standard::char_("-._~");
              pct_encoded = '%' << hex_p;
              pchar = sub_delims|unreserved|pct_encoded|karma::standard::char_(":@");
              root = *( pchar | karma::standard::char_("/?") );
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 16> hex_p;
            boost::spirit::karma::rule< Iterator, char() > sub_delims;
            boost::spirit::karma::rule< Iterator, char() > unreserved;
            boost::spirit::karma::rule< Iterator, char() > pct_encoded;
            boost::spirit::karma::rule< Iterator, char() > pchar;
            boost::spirit::karma::rule< Iterator, std::string() > root;
        };

      template< typename Iterator >
        class scheme : public boost::spirit::karma::grammar< Iterator, std::string() > {
          public:
            scheme() : scheme::base_type( root ) {
              namespace karma = boost::spirit::karma;
              root = +( karma::standard::alnum|karma::standard::char_("+-.") );
            }
          private:
            boost::spirit::karma::rule< Iterator, std::string() > root;
        };

      template< typename Iterator >
        class uri : public boost::spirit::karma::grammar< Iterator, hermit::uri() > {
          public:
            uri() : uri::base_type( root ) {
              namespace karma = boost::spirit::karma;
              namespace phx = boost::phoenix;
              hier_part = -( "//" << authority_ ) << (path_absolute|path_rootless|path_abempty);
              root = ( scheme_ << ':' << hier_part << -( '?' << query_ ) << -( '#' << fragment_ ) )[
                karma::_1 = phx::at_c< 0 >( karma::_val ),
                karma::_2 = phx::construct< boost::fusion::vector< boost::optional< hermit::authority >, hermit::path > >( phx::at_c< 1 >( karma::_val ), phx::at_c< 2 >( karma::_val ) ),
                karma::_3 = phx::at_c< 3 >( karma::_val ),
                karma::_4 = phx::at_c< 4 >( karma::_val )
              ];
            }
          private:
            path< Iterator, true, true > path_abempty;
            path< Iterator, true, false > path_absolute;
            path< Iterator, false, false > path_rootless;
            authority< Iterator > authority_;
            scheme< Iterator > scheme_;
            query< Iterator > query_;
            fragment< Iterator > fragment_;
            boost::spirit::karma::rule< Iterator, boost::fusion::vector< boost::optional< hermit::authority >, hermit::path >() > hier_part;
            boost::spirit::karma::rule< Iterator, hermit::uri() > root;
        };
    }
  }
}

#endif

