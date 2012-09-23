#ifndef HERMIT_SPIRIT_QI_URI
#define HERMIT_SPIRIT_QI_URI

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/filesystem/path.hpp>

#include <hermit/ip.hpp>
#include <hermit/uri.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template< typename Iterator >
        class reg_name : public boost::spirit::qi::grammar< Iterator, std::string() > {
          public:
            reg_name() : reg_name::base_type( root ) {
              namespace qi = boost::spirit::qi;
              sub_delims = qi::standard::char_("!$&'()*+,;=");
              unreserved = qi::standard::alnum | qi::standard::char_("-._~");
              pct_encoded = '%' >> hex2_p;
              root = *( sub_delims|unreserved|pct_encoded );
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
            boost::spirit::qi::rule< Iterator, char() > sub_delims; 
            boost::spirit::qi::rule< Iterator, char() > unreserved; 
            boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
            boost::spirit::qi::rule< Iterator, std::string() > root; 
        };

      template< typename Iterator >
        class host : public boost::spirit::qi::grammar< Iterator, hermit::host() > {
          public:
            host() : host::base_type( root ) {
              root = ipv4address|( '[' >> ipv6address >> ']' )|hostname;
            }
          private:
            ipv4_parser< Iterator > ipv4address;
            ipv6_parser< Iterator > ipv6address;
            reg_name< Iterator > hostname;
            boost::spirit::qi::rule< Iterator, hermit::host() > root;
        };

      template< typename Iterator >
        class userinfo : public boost::spirit::qi::grammar< Iterator, std::vector< std::string >() > {
          public:
            userinfo() : userinfo::base_type( root ) {
              namespace qi = boost::spirit::qi;
              sub_delims = qi::standard::char_("!$&'()*+,;=");
              unreserved = qi::standard::alnum | qi::standard::char_("-._~");
              pct_encoded = '%' >> hex2_p;
              info = +( sub_delims|unreserved|pct_encoded );
              root = info % ':';
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
            boost::spirit::qi::rule< Iterator, char() > sub_delims; 
            boost::spirit::qi::rule< Iterator, char() > unreserved; 
            boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
            boost::spirit::qi::rule< Iterator, std::string() > info;
            boost::spirit::qi::rule< Iterator, std::vector< std::string >() > root;
        };

      template< typename Iterator >
        class authority : public boost::spirit::qi::grammar< Iterator, hermit::authority() > {
          public:
            authority() : authority::base_type( root ) {
              namespace qi = boost::spirit::qi;
              opt_userinfo = -( userinfo_[ qi::_a = qi::_1 ] >> '@' )[ qi::_val = qi::_a ];
              root = ( opt_userinfo >> host_ >> -( ':' >> dec5_p ) );
            }
          private:
            boost::spirit::qi::uint_parser<uint16_t, 10, 1, 5> dec5_p;
            userinfo< Iterator > userinfo_;
            boost::spirit::qi::rule< Iterator, std::vector< std::string >(), boost::spirit::qi::locals< std::vector< std::string > > > opt_userinfo;
            host< Iterator > host_;
            boost::spirit::qi::rule< Iterator, hermit::authority() > root;
        };

      template< typename Iterator, bool nz, bool nc >
        class segment : public boost::spirit::qi::grammar< Iterator, std::string() > {
          public:
            segment() : segment::base_type( root ) {
              namespace qi = boost::spirit::qi;
              sub_delims = qi::char_("!$&'()*+,;=");
              unreserved = qi::alnum | qi::char_("-._~");
              pct_encoded = '%' >> hex2_p;
              if( nc )
                pchar = sub_delims|unreserved|pct_encoded|qi::char_("@");
              else
                pchar = sub_delims|unreserved|pct_encoded|qi::char_(":@");
              if( nz )
                root = +pchar;
              else
                root = *pchar;
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
            boost::spirit::qi::rule< Iterator, char() > sub_delims; 
            boost::spirit::qi::rule< Iterator, char() > unreserved; 
            boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
            boost::spirit::qi::rule< Iterator, char() > pchar; 
            boost::spirit::qi::rule< Iterator, std::string() > root; 
        };

      template< typename Iterator, bool absolute, bool accept_empty >
        class path : public boost::spirit::qi::grammar< Iterator, boost::filesystem::path() > {
          public:
            path() : path::base_type( root ) {
              namespace qi = boost::spirit::qi;
              namespace phx = boost::phoenix;
              if( absolute ) {
                if( accept_empty )
                  segs = *( '/' >> segment_ );
                else
                  segs = '/' >> ( segment_ >> *( '/' >> segment_ ) )[
                      phx::push_back( qi::_val, qi::_1 ),
                      phx::insert( qi::_val, phx::end( qi::_val ), phx::begin( qi::_2 ), phx::end( qi::_2 ) )
                    ];
                root = segs[ qi::_val = phx::bind( &path::create_absolute_path, qi::_1 ) ];
              }
              else {
                segs = ( segment_nz >> *( '/' >> segment_ ) )[
                    phx::push_back( qi::_val, qi::_1 ),
                    phx::insert( qi::_val, phx::end( qi::_val ), phx::begin( qi::_2 ), phx::end( qi::_2 ) )
                  ];
                root = segs[ qi::_val = phx::bind( &path::create_relative_path, qi::_1 ) ];
              }
            }
          private:
            static boost::filesystem::path create_absolute_path( const std::vector< std::string > &segs ) {
              boost::filesystem::path temp( "/" );
              std::cout << "foo" << std::endl;
              for( auto elem: segs ) {
                std::cout << elem << std::endl;
                temp /= elem;
              }
              std::cout << "bar" << std::endl;
              return temp;
            }
            static boost::filesystem::path create_relative_path( const std::vector< std::string > &segs ) {
              if( segs.empty() )
                return boost::filesystem::path();
              boost::filesystem::path temp( *segs.begin() );
              for( auto iter = std::next( segs.begin() ); iter != segs.end(); ++iter )
                temp /= *iter;
              return temp;
            }
            segment< Iterator, false, false > segment_;
            segment< Iterator, true, false > segment_nz;
            segment< Iterator, true, true > segment_nz_nc;
            boost::spirit::qi::rule< Iterator, std::vector< std::string >() > segs;
            boost::spirit::qi::rule< Iterator, boost::filesystem::path() > root;
      };


      template< typename Iterator >
        class query : public boost::spirit::qi::grammar< Iterator, std::string() > {
          public:
            query() : query::base_type( root ) {
              namespace qi = boost::spirit::qi;
              namespace phx = boost::phoenix;
              sub_delims = qi::standard::char_("!$&'()*+,;=");
              unreserved = qi::standard::alnum | qi::standard::char_("-._~");
              pct_encoded = '%' >> hex2_p;
              pchar = sub_delims|unreserved|pct_encoded|qi::standard::char_(":@");
              root = *( pchar | qi::standard::char_("/?") );
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
            boost::spirit::qi::rule< Iterator, char() > sub_delims;
            boost::spirit::qi::rule< Iterator, char() > unreserved;
            boost::spirit::qi::rule< Iterator, char() > pct_encoded;
            boost::spirit::qi::rule< Iterator, char() > pchar;
            boost::spirit::qi::rule< Iterator, std::string() > root;
        };

      template< typename Iterator >
        class fragment : public boost::spirit::qi::grammar< Iterator, std::string() > {
          public:
            fragment() : fragment::base_type( root ) {
              namespace qi = boost::spirit::qi;
              sub_delims = qi::standard::char_("!$&'()*+,;=");
              unreserved = qi::standard::alnum | qi::standard::char_("-._~");
              pct_encoded = '%' >> hex2_p;
              pchar = sub_delims|unreserved|pct_encoded|qi::standard::char_(":@");
              root = *( pchar | qi::standard::char_("/?") );
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2> hex2_p;
            boost::spirit::qi::rule< Iterator, char() > sub_delims;
            boost::spirit::qi::rule< Iterator, char() > unreserved;
            boost::spirit::qi::rule< Iterator, char() > pct_encoded;
            boost::spirit::qi::rule< Iterator, char() > pchar;
            boost::spirit::qi::rule< Iterator, std::string() > root;
        };

      template< typename Iterator >
        class scheme : public boost::spirit::qi::grammar< Iterator, std::string() > {
          public:
            scheme() : scheme::base_type( root ) {
              namespace qi = boost::spirit::qi;
              root = +( qi::standard::alnum|qi::standard::char_("+-.") );
            }
          private:
            boost::spirit::qi::rule< Iterator, std::string() > root;
        };

  template< typename Iterator >
    class uri : public boost::spirit::qi::grammar< Iterator, hermit::uri() > {
      public:
        uri() : uri::base_type( root ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          hier_part = ( "//" >> authority_ >> path_abempty )|absolute_path_only|relative_path_only;
          absolute_path_only = path_absolute[
            phx::at_c< 0 >( qi::_val ) = boost::optional< hermit::authority >(),
            phx::at_c< 1 >( qi::_val ) = qi::_1
          ];
          relative_path_only = path_rootless[
            phx::at_c< 0 >( qi::_val ) = boost::optional< hermit::authority >(),
            phx::at_c< 1 >( qi::_val ) = qi::_1
          ];
          root = ( scheme_ >> ':' >> hier_part >> -( '?' >> query_ ) >> -( '#' >> fragment_ ) )[
            phx::at_c< 0 >( qi::_val ) = qi::_1,
            phx::at_c< 1 >( qi::_val ) = phx::at_c< 0 >( qi::_2 ),
            phx::at_c< 2 >( qi::_val ) = phx::at_c< 1 >( qi::_2 ),
            phx::at_c< 3 >( qi::_val ) = qi::_3,
            phx::at_c< 4 >( qi::_val ) = qi::_4
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
        boost::spirit::qi::rule< Iterator, boost::fusion::vector< boost::optional< hermit::authority >, boost::filesystem::path >() > hier_part;
        boost::spirit::qi::rule< Iterator, boost::fusion::vector< boost::optional< hermit::authority >, boost::filesystem::path >() > absolute_path_only;
        boost::spirit::qi::rule< Iterator, boost::fusion::vector< boost::optional< hermit::authority >, boost::filesystem::path >() > relative_path_only;
        boost::spirit::qi::rule< Iterator, hermit::uri() > root;
    };






    }
  }
}

#endif

