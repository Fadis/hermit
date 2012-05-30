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
          root = +( sub_delims|unreserved|pct_encoded );
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
          root = ( -( userinfo_ >> '@' ) >> host_ >> -( ':' >> dec5_p ) )[
            _val = boost::phoenix::bind( &build_authority, _1, _2, _3 )
          ];
        }
      private:
        static authority build_authority( const boost::optional< userinfo > &ui_, const host &host_, boost::optional< port > port_ ) {
         //broken
          return authority( ui_, host_, port_ );
        }
        userinfo_parser< Iterator > userinfo_;
        host_parser< Iterator > host_;
        boost::spirit::qi::rule< Iterator, authority() > root;
    };
}
#endif

