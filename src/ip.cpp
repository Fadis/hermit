#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector.hpp>
#include <vector>
#include <string>
#include <utility>

namespace hermit {
  namespace detail {
    boost::spirit::qi::uint_parser<uint8_t, 10, 1, 3> dec3_p; 
    boost::spirit::qi::uint_parser<uint16_t, 16, 1, 4> hex4_p; 

    template< typename Iterator >
      class ipv4_rule :
        public boost::spirit::qi::grammar<
        Iterator,
        uint32_t()
        > {
          public:
            ipv4_rule() : ipv4_rule::base_type( ipv4address ) {
              using namespace boost::spirit;
              using namespace boost::spirit::ascii;
              ipv4address = ( dec3_p >> '.' >> dec3_p >> '.' >> dec3_p >> '.' >> dec3_p )[
                _val = boost::phoenix::bind( &build_ipv4_address, _1, _2, _3, _4 )
                ] >> eoi;
            }
          private:
            static uint32_t build_ipv4_address(
                uint8_t a, uint8_t b, uint8_t c, uint8_t d
                ) {
              return static_cast< uint32_t >( a ) << 24 |
                static_cast< uint32_t >( b ) << 16 |
                static_cast< uint32_t >( c ) << 8 |
                static_cast< uint32_t >( d );
            }
            uint16_t temp;
            boost::spirit::qi::rule< Iterator, uint32_t() > ipv4address;
        };

    template< typename Iterator, bool is_partial >
      class ipv6_phase2_rule :
        public boost::spirit::qi::grammar<
        Iterator,
        std::pair< uint64_t, uint64_t >()
        > {
          public:
            ipv6_phase2_rule() : ipv6_phase2_rule::base_type( ipv6address ) {
              using namespace boost::spirit;
              using namespace boost::spirit::ascii;
              ipv6address = ( hexpart )[
                _val = boost::phoenix::bind( &build_ipv6_address, _1, _pass )
                ] >> eoi;
              hexpart =
                ( hexseq >> "::" >> hexseq )[ _val = boost::phoenix::bind( &build_partial_ipv6_address, true, _1, _2 ) ] |
                ( hexseq >> omit[ "::" ] )[ _val = boost::phoenix::bind( &build_partial_ipv6_address, true,  _1, std::vector< uint16_t >() ) ] |
                ( "::" >> hexseq )[ _val = boost::phoenix::bind( &build_partial_ipv6_address, true, std::vector< uint16_t >(), _1 ) ] |
                hexseq[ _val = boost::phoenix::bind( &build_partial_ipv6_address, false, _1, std::vector< uint16_t >() ) ];
              hexseq = hex4_p >> *( ':' >> hex4_p );
            }
          private:
            static boost::fusion::vector< bool, std::vector< uint16_t >, std::vector< uint16_t > >
              build_partial_ipv6_address(
                  bool can_shorter,
                  const std::vector< uint16_t > &head,
                  const std::vector< uint16_t > &tail
                  ) {
                return boost::fusion::make_vector( can_shorter, head, tail );
              }
            static std::pair< uint64_t, uint64_t >
              build_ipv6_address(
                  const boost::fusion::vector< bool, std::vector< uint16_t >, std::vector< uint16_t > >  &v6,
                  bool &pass
                  ) {
                static const unsigned int width = is_partial ? 96 :128; 
                std::vector< uint16_t > temp( width / 16 );
                std::fill( temp.begin(), temp.end(), static_cast< uint16_t >( 0 ) );
                std::pair< uint64_t, uint64_t > ipv6_value = std::make_pair( 0ull, 0ull );
                if(
                    ( !boost::fusion::at_c< 0 >( v6 ) && boost::fusion::at_c< 1 >( v6 ).size() + boost::fusion::at_c< 2 >( v6 ).size() == temp.size() ) ||
                    ( boost::fusion::at_c< 0 >( v6 ) && boost::fusion::at_c< 1 >( v6 ).size() + boost::fusion::at_c< 2 >( v6 ).size() <= temp.size() )
                  ) {
                  std::copy( boost::fusion::at_c< 1 >( v6 ).begin(), boost::fusion::at_c< 1 >( v6 ).end(), temp.begin() );
                  std::copy( boost::fusion::at_c< 2 >( v6 ).rbegin(), boost::fusion::at_c< 2 >( v6 ).rend(), temp.rbegin() );
                  std::vector< uint16_t >::const_iterator iter;
                  std::vector< uint16_t >::const_iterator half = temp.begin();
                  std::advance( half, 64 / 16 );
                  for( iter = temp.begin(); iter != half; ++iter ) {
                    ipv6_value.first <<= 16;
                    ipv6_value.first |= *iter;
                  }
                  for( ; iter != temp.end(); ++iter ) {
                    ipv6_value.second <<= 16;
                    ipv6_value.second |= *iter;
                  }
                  ipv6_value.second <<= 128 - width;
                  pass = true;
                }
                else
                  pass = false;
                return ipv6_value;
              }
            uint16_t temp;
            ipv4_rule< Iterator > ipv4address;
            boost::spirit::qi::rule< Iterator, std::vector< uint16_t >() > hexseq;
            boost::spirit::qi::rule< Iterator, boost::fusion::vector< bool, std::vector< uint16_t >, std::vector< uint16_t > >() > hexpart;
            boost::spirit::qi::rule< Iterator, std::pair< uint64_t, uint64_t >() > ipv6address;
        };

    template< typename Iterator >
      class ipv6_phase1_rule :
        public boost::spirit::qi::grammar<
        Iterator,
        std::pair< uint64_t, uint64_t >()
        > {
          public:
            ipv6_phase1_rule() : ipv6_phase1_rule::base_type( root ) {
              using namespace boost::spirit;
              using namespace boost::spirit::ascii;
              root = ( *( digit | char_( "a-fA-F." ) ) % ':' )[
                _val = boost::phoenix::bind( &call_phase2, _1, _pass )
                ];
            }
          private:
            static std::pair< uint64_t, uint64_t > call_phase2(
                const std::vector< std::vector< char > > &str, bool &pass
                ) {
              ipv4_rule< std::vector< char >::const_iterator > ipv4address;
              uint32_t ipv4_value;
              std::vector< char >::const_iterator ipv4_iter = str.back().begin();
              if( boost::spirit::qi::parse( ipv4_iter, str.back().end(), ipv4address, ipv4_value ) && ipv4_iter == str.back().end() ) {
                std::string serial;
                for( std::vector< std::vector< char > >::const_iterator ipv6_iter = str.begin(); ipv6_iter != str.end() - 1; ++ipv6_iter ) {
                  for( char elem: *ipv6_iter )
                    serial += elem;
                  serial += ':';
                }
                if( serial[ serial.size() - 2 ] != ':' && serial[ serial.size() - 1 ] == ':' )
                  serial.pop_back();
                ipv6_phase2_rule< std::string::const_iterator, true > ipv6address;
                std::pair< uint64_t, uint64_t > ipv6_value;
                std::string::const_iterator ipv6_iter = serial.begin();
                if( boost::spirit::qi::parse( ipv6_iter, serial.cend(), ipv6address, ipv6_value ) && ipv6_iter == serial.end() ) {
                  ipv6_value.second |= ipv4_value;
                  pass = true;
                  return ipv6_value;
                }
                else
                  pass = false;
              }
              else {
                std::string serial;
                for( std::vector< std::vector< char > >::const_iterator ipv6_iter = str.begin(); ipv6_iter != str.end(); ++ipv6_iter ) {
                  for( char elem: *ipv6_iter )
                    serial += elem;
                  serial += ':';
                }
                if( str.back().empty() )
                  serial.pop_back();
                if( serial[ serial.size() - 2 ] != ':' && serial[ serial.size() - 1 ] == ':' )
                  serial.pop_back();
                ipv6_phase2_rule< std::string::const_iterator, false > ipv6address;
                std::pair< uint64_t, uint64_t > ipv6_value;
                std::string::const_iterator ipv6_iter = serial.begin();
                if( boost::spirit::qi::parse( ipv6_iter, serial.cend(), ipv6address, ipv6_value ) && ipv6_iter == serial.end() ) {
                  pass = true;
                  return ipv6_value;
                }
                else
                  pass = false;
              }
              return std::make_pair( 0ull, 0ull );
            }
            uint16_t temp;
            boost::spirit::qi::rule< Iterator, std::pair< uint64_t, uint64_t >() > root;
        };

    boost::optional< uint32_t > str2ipv4( const std::string &str ) {
      uint32_t result;
      ipv4_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = str.begin();
      if( boost::spirit::qi::parse( iter, str.end(), rule, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< uint32_t >();
    }

    boost::optional< std::pair< uint64_t, uint64_t > > str2ipv6( const std::string &str ) {
      std::pair< uint64_t, uint64_t > result;
      ipv6_phase1_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = str.begin();
      if( boost::spirit::qi::parse( iter, str.end(), rule, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< std::pair< uint64_t, uint64_t > >();
    }
  }
}
