#ifndef HERMIT_URI_HPP
#define HERMIT_URI_HPP
#include <stdexcept>
#include <boost/utility/enable_if.hpp>
#include <boost/format.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/lexical_cast.hpp>
#include <hermit/integer.hpp>
#include <hermit/ip.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/home/support/container.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/filesystem/path.hpp>
#include <vector>
#include <string>
#include <utility>

namespace hermit {

  typedef std::string reg_name;

  boost::spirit::qi::uint_parser<uint8_t, 16, 1, 2> hex2_p;
  boost::spirit::qi::uint_parser<uint16_t, 10, 1, 5> dec5_p;
  template< typename Iterator >
    class reg_name_parser : public boost::spirit::qi::grammar< Iterator, reg_name() > {
      public:
        reg_name_parser() : reg_name_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          sub_delims = char_("!$&'()*+,;=");
          unreserved = alnum | char_("-._~");
          pct_encoded = '%' >> hex2_p;
          root = *( sub_delims|unreserved|pct_encoded );
        }
      private:
        boost::spirit::qi::rule< Iterator, char() > sub_delims; 
        boost::spirit::qi::rule< Iterator, char() > unreserved; 
        boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
        boost::spirit::qi::rule< Iterator, reg_name() > root; 
    };

  typedef boost::variant< ipv4, ipv6, reg_name > host;

  template< typename Iterator >
    class host_parser : public boost::spirit::qi::grammar< Iterator, host() > {
      public:
        host_parser() : host_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;
          root = ipv4address|( '[' >> ipv6address >> ']' )|hostname;
        }
      private:
        ipv4_parser< Iterator > ipv4address;
        ipv6_parser< Iterator > ipv6address;
        reg_name_parser< Iterator > hostname;
        boost::spirit::qi::rule< Iterator, host() > root;
    };

  typedef uint16_t port;

  typedef std::vector< std::string > userinfo;

  template< typename Iterator >
    class userinfo_parser : public boost::spirit::qi::grammar< Iterator, userinfo() > {
      public:
        userinfo_parser() : userinfo_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          sub_delims = char_("!$&'()*+,;=");
          unreserved = alnum | char_("-._~");
          pct_encoded = '%' >> hex2_p;
          info = +( sub_delims|unreserved|pct_encoded );
          root = info % ':';
        }
      private:
        boost::spirit::qi::rule< Iterator, char() > sub_delims; 
        boost::spirit::qi::rule< Iterator, char() > unreserved; 
        boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
        boost::spirit::qi::rule< Iterator, std::string() > info;
        boost::spirit::qi::rule< Iterator, userinfo() > root;
    };

  class authority {
    public:
      authority() {}
      authority( const boost::optional< userinfo > &userinfo__, const host &host__, const boost::optional< port > &port__ ) : userinfo_( userinfo__ ), host_( host__ ), port_( port__ ) {}
      const boost::optional< userinfo > &get_userinfo() const {
        return userinfo_;
      }
      const host &get_host() const {
        return host_;
      }
      const boost::optional< port > &get_port() const {
        return port_;
      }
    private:
      boost::optional< userinfo > userinfo_;
      host host_;
      boost::optional< port > port_;
  };

  template< typename Iterator >
    class authority_parser : public boost::spirit::qi::grammar< Iterator, authority() > {
      public:
        authority_parser() : authority_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;
          root = ( userinfo_ >> '@' >> host_ >> -( ':' >> dec5_p ) )[
            _val = boost::phoenix::bind( &build_authority, _1, _2, _3 )
            ] |
            ( host_ >> -( ':' >> dec5_p ) )[
            _val = boost::phoenix::bind( &build_authority, _1, _2 )
            ];
        }
      private:
        static authority build_authority( const userinfo &ui_, const host &host_, boost::optional< port > port_ ) {
          return authority( ui_, host_, port_ );
        }
        static authority build_authority( const host &host_, boost::optional< port > port_ ) {
          return authority( boost::optional< userinfo >(), host_, port_ );
        }
        userinfo_parser< Iterator > userinfo_;
        host_parser< Iterator > host_;
        boost::spirit::qi::rule< Iterator, authority() > root;
    };

  typedef std::string segment;

  template< typename Iterator, bool nz, bool nc >
    class segment_parser : public boost::spirit::qi::grammar< Iterator, segment() > {
      public:
        segment_parser() : segment_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          sub_delims = char_("!$&'()*+,;=");
          unreserved = alnum | char_("-._~");
          pct_encoded = '%' >> hex2_p;
          if( nc )
            pchar = sub_delims|unreserved|pct_encoded|char_("@");
          else
            pchar = sub_delims|unreserved|pct_encoded|char_(":@");
          if( nz )
            root = +pchar;
          else
            root = *pchar;
        }
      private:
        boost::spirit::qi::rule< Iterator, char() > sub_delims; 
        boost::spirit::qi::rule< Iterator, char() > unreserved; 
        boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
        boost::spirit::qi::rule< Iterator, char() > pchar; 
        boost::spirit::qi::rule< Iterator, segment() > root; 
    };

  template< typename Iterator, bool absolute, bool accept_empty >
    class path_parser : public boost::spirit::qi::grammar< Iterator, boost::filesystem::path() > {
      public:
        path_parser() : path_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;
          if( absolute ) {
            if( accept_empty )
              root = ( *( '/' >> segment_ ) )[
                _val = boost::phoenix::bind( &create_absolute_path, _1 )
              ];
            else
              root = ( '/' >> segment_ >> *( '/' >> segment_ ) )[
                _val = boost::phoenix::bind( &create_absolute_path, _1, _2 )
              ];
          }
          else {
            root = ( segment_nz >> *( '/' >> segment_ ) )[
              _val = boost::phoenix::bind( &create_relative_path, _1, _2 )
            ];
          }
        }
      private:
        static boost::filesystem::path create_absolute_path( const std::vector< std::string > &segs ) {
          boost::filesystem::path temp( "/" );
          for( auto elem: segs )
            temp /= elem;
          return temp;
        }
        static boost::filesystem::path create_absolute_path( const std::string &head, const std::vector< std::string > &segs ) {
          boost::filesystem::path temp( "/" );
          temp /= head;
          for( auto elem: segs )
            temp /= elem;
          return temp;
        }
        static boost::filesystem::path create_relative_path( const std::string &head, const std::vector< std::string > &segs ) {
          boost::filesystem::path temp( head );
          for( auto elem: segs )
            temp /= elem;
          return temp;
        }
        segment_parser< Iterator, false, false > segment_; 
        segment_parser< Iterator, true, false > segment_nz; 
        segment_parser< Iterator, true, true > segment_nz_nc;
        boost::spirit::qi::rule< Iterator, boost::filesystem::path() > root; 
    };

  typedef std::string query_type;

  template< typename Iterator >
    class query_parser : public boost::spirit::qi::grammar< Iterator, query_type() > {
      public:
        query_parser() : query_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          sub_delims = char_("!$&'()*+,;=");
          unreserved = alnum | char_("-._~");
          pct_encoded = '%' >> hex2_p;
          pchar = sub_delims|unreserved|pct_encoded|char_(":@");
          root = *( pchar | char_("/?") );
        }
      private:
        boost::spirit::qi::rule< Iterator, char() > sub_delims; 
        boost::spirit::qi::rule< Iterator, char() > unreserved; 
        boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
        boost::spirit::qi::rule< Iterator, char() > pchar; 
        boost::spirit::qi::rule< Iterator, query_type() > root; 
    };

  typedef std::string fragment_type;
  
  template< typename Iterator >
    class fragment_parser : public boost::spirit::qi::grammar< Iterator, fragment_type() > {
      public:
        fragment_parser() : fragment_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          sub_delims = char_("!$&'()*+,;=");
          unreserved = alnum | char_("-._~");
          pct_encoded = '%' >> hex2_p;
          pchar = sub_delims|unreserved|pct_encoded|char_(":@");
          root = *( pchar | char_("/?") );
        }
      private:
        boost::spirit::qi::rule< Iterator, char() > sub_delims; 
        boost::spirit::qi::rule< Iterator, char() > unreserved; 
        boost::spirit::qi::rule< Iterator, char() > pct_encoded; 
        boost::spirit::qi::rule< Iterator, char() > pchar; 
        boost::spirit::qi::rule< Iterator, fragment_type() > root; 
    };

  typedef std::string scheme_type;

  template< typename Iterator >
    class scheme_parser : public boost::spirit::qi::grammar< Iterator, scheme_type() > {
      public:
        scheme_parser() : scheme_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;

          root = +( alnum|char_("+-.") );
        }
      private:
        boost::spirit::qi::rule< Iterator, scheme_type() > root; 
    };

  class uri {
    public:
      uri() {}
      uri( const scheme_type &scheme_, const boost::optional< authority > &authority__, const boost::filesystem::path &path_, const boost::optional< query_type > &query_, const boost::optional< fragment_type > &fragment_ )
      : scheme( scheme_ ), authority_( authority__ ), path( path_ ), query( query_ ), fragment( fragment_ ) {}
      const scheme_type &get_scheme() const {
        return scheme;
      }
      const boost::optional< authority > &get_authority() const {
        return authority_;
      }
      const boost::filesystem::path &get_path() const {
        return path;
      }
      const boost::optional< query_type > &get_query() const {
        return query;
      }
      const boost::optional< fragment_type > &get_fragment() const {
        return fragment;
      }
    private:
      scheme_type scheme;
      boost::optional< authority > authority_;
      boost::filesystem::path path;
      boost::optional< query_type > query;
      boost::optional< fragment_type > fragment;
  };

  typedef std::pair< boost::optional< authority >, boost::filesystem::path > hier_type;

  template< typename Iterator >
    class uri_parser : public boost::spirit::qi::grammar< Iterator, uri() > {
      public:
        uri_parser() : uri_parser::base_type( root ) {
          using namespace boost::spirit;
          using namespace boost::spirit::ascii;
          hier_part = ( "//" >> authority_ >> path_abempty ) [
            _val = boost::phoenix::bind( &create_hier_part, _1, _2 )
          ] |
          path_absolute[
            _val = boost::phoenix::bind( &create_hier_part, boost::optional< authority >(), _1 )
          ] |
          path_rootless[
            _val = boost::phoenix::bind( &create_hier_part, boost::optional< authority >(), _1 )
          ];

          root = ( scheme >> ':' >> hier_part >> -( '?' >> query ) >> -( '#' >> fragment ) )[
            _val = boost::phoenix::bind( &create_uri, _1, _2, _3, _4 )
          ];
        }
      private:
        static uri create_uri( const scheme_type &scheme, const hier_type &hier, const boost::optional< query_type > &query, const boost::optional< fragment_type > &fragment ) {
          return uri( scheme, hier.first, hier.second, query, fragment );
        }
        static hier_type create_hier_part( const boost::optional< authority > &authority_, const boost::filesystem::path &path_ ) {
          return std::make_pair( authority_, path_ );
        }
        path_parser< Iterator, true, true > path_abempty;
        path_parser< Iterator, true, false > path_absolute;
        path_parser< Iterator, false, false > path_rootless;
        authority_parser< Iterator > authority_;
        scheme_parser< Iterator > scheme;
        query_parser< Iterator > query;
        fragment_parser< Iterator > fragment;
        boost::spirit::qi::rule< Iterator, hier_type() > hier_part;
        boost::spirit::qi::rule< Iterator, uri() > root;
    };

}

#endif

