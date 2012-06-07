#ifndef HERMIT_IP_HPP
#define HERMIT_IP_HPP
#include <stdexcept>
#include <boost/utility/enable_if.hpp>
#include <boost/format.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/lexical_cast.hpp>
#include <hermit/integer.hpp>


#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/home/support/container.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector.hpp>
#include <vector>
#include <string>
#include <utility>

namespace hermit {
  namespace detail {
    extern boost::spirit::qi::uint_parser<uint8_t, 10, 1, 3> dec3_p; 
    extern boost::spirit::qi::uint_parser<uint8_t, 10, 1, 2> dec2_p; 
    extern boost::spirit::qi::uint_parser<uint16_t, 16, 1, 4> hex4_p; 

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
                ];
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
            boost::spirit::qi::rule< Iterator, uint32_t() > ipv4address;
        };


    template< typename Iterator >
      class ipv4segment_rule :
        public boost::spirit::qi::grammar<
        Iterator,
        std::pair< uint32_t, uint32_t >()
        > {
          public:
            ipv4segment_rule() : ipv4segment_rule::base_type( root ) {
              using namespace boost::spirit;
              using namespace boost::spirit::ascii;
              root = ( ipv4address >> '/' >> dec2_p )[
                _val = boost::phoenix::bind( &build_ipv4segment, _1, _2, _pass )
                ];
            }
          private:
            static std::pair< uint32_t, uint32_t > build_ipv4segment(
                uint32_t ip_, uint8_t mask_, bool &pass_
                ) {
              uint32_t mask = 1ul;
              if( mask_ <= 32 && mask_ > 0 ) {
                const uint32_t all = 0xFFFFFFFFul;
                mask <<= ( 32 - mask_ );
                mask -= 1;
                mask = all ^ mask;
                pass_ = ( ip_ & mask ) == ip_; 
              }
              else
                pass_ = false;
              return std::make_pair( ip_, mask );
            }
            ipv4_rule< Iterator > ipv4address;
            boost::spirit::qi::rule< Iterator, std::pair< uint32_t, uint32_t >() > root;
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
                hexseq[ _val = boost::phoenix::bind( &build_partial_ipv6_address, false, _1, std::vector< uint16_t >() ) ] |
                ( lit( "::" ) )[ _val = boost::phoenix::bind( &build_partial_ipv6_address, true, std::vector< uint16_t >(), std::vector< uint16_t >() ) ];
              hexseq = hex4_p >> *( omit[char_(':')] >> hex4_p );
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
                  serial.erase( serial.size() - 1 );
                ipv6_phase2_rule< std::string::const_iterator, true > ipv6address;
                std::pair< uint64_t, uint64_t > ipv6_value;
                std::string::const_iterator ipv6_iter = serial.begin();
                if( boost::spirit::qi::parse( ipv6_iter, static_cast< std::string::const_iterator >( serial.end() ), ipv6address, ipv6_value ) && ipv6_iter == serial.end() ) {
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
                  serial.erase( serial.size() - 1 );
                if( serial[ serial.size() - 2 ] != ':' && serial[ serial.size() - 1 ] == ':' )
                  serial.erase( serial.size() - 1 );
                ipv6_phase2_rule< std::string::const_iterator, false > ipv6address;
                std::pair< uint64_t, uint64_t > ipv6_value;
                std::string::const_iterator ipv6_iter = serial.begin();
                if( boost::spirit::qi::parse( ipv6_iter, static_cast< std::string::const_iterator >( serial.end() ), ipv6address, ipv6_value ) && ipv6_iter == serial.end() ) {
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


    template< typename Iterator >
      class ipv6segment_rule :
        public boost::spirit::qi::grammar<
        Iterator,
        std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > >()
        > {
          public:
            ipv6segment_rule() : ipv6segment_rule::base_type( root ) {
              using namespace boost::spirit;
              using namespace boost::spirit::ascii;
              root = ( ipv6address >> '/' >> dec3_p )[
                _val = boost::phoenix::bind( &build_ipv6segment, _1, _2, _pass )
                ];
            }
          private:
            static std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > build_ipv6segment(
                const std::pair< uint64_t, uint64_t > &ip_, uint8_t mask_, bool &pass_
                ) {
              if( mask_ < 64 && mask_ > 0 ) {
                const uint64_t all = 0xFFFFFFFFFFFFFFFFull;
                uint64_t mask = 1ull;
                mask <<= ( 64 - mask_ );
                mask -= 1;
                mask = all ^ mask;
                pass_ = ip_.second == 0x0ull && ( ip_.first & mask ) == ip_.first;
                return std::make_pair( ip_, std::make_pair( mask, 0x0ull ) );
              }
              else if( mask_ < 128 && mask_ >= 64 ) {
                const uint64_t all = 0xFFFFFFFFFFFFFFFFull;
                uint64_t mask = 1ull;
                mask <<= ( 128 - mask_ );
                mask -= 1;
                mask = all ^ mask;
                pass_ = ( ip_.second & mask ) == ip_.second;
                return std::make_pair( ip_, std::make_pair( 0xFFFFFFFFFFFFFFFFull, mask ) );
              }
              else {
                pass_ = false;
                return std::make_pair( ip_, std::make_pair( 0x0ull, 0x0ull ) );
              }
            }
            ipv6_phase1_rule< Iterator > ipv6address;
            boost::spirit::qi::rule< Iterator, std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > >() > root;
        };

    template< typename String >
    boost::optional< uint32_t > str2ipv4( const String &str ) {
      uint32_t result;
      ipv4_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = boost::begin( str );
      if( boost::spirit::qi::parse( iter, boost::end( str ), rule, result ) && iter == boost::end( str ) )
        return result;
      else
        return boost::optional< uint32_t >();
    }
    
    template< typename String >
    boost::optional< std::pair< uint32_t, uint32_t > > str2ipv4segment( const String &str ) {
      std::pair< uint32_t, uint32_t > result;
      ipv4segment_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = boost::begin( str );
      if( boost::spirit::qi::parse( iter, boost::end( str ), rule, result ) && iter == boost::end( str ) ) {
        return result;
      }
      else
        return boost::optional< std::pair< uint32_t, uint32_t > >();
    }

    template< typename String >
    boost::optional< std::pair< uint64_t, uint64_t > > str2ipv6( const String &str ) {
      std::pair< uint64_t, uint64_t > result;
      ipv6_phase1_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = boost::begin( str );
      if( boost::spirit::qi::parse( iter, boost::end( str ), rule, result ) && iter == boost::end( str ) )
        return result;
      else
        return boost::optional< std::pair< uint64_t, uint64_t > >();
    }

    template< typename String >
    boost::optional< std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > > str2ipv6segment( const String &str ) {
      std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > result;
      ipv6segment_rule< std::string::const_iterator > rule;
      std::string::const_iterator iter = boost::begin( str );
      if( boost::spirit::qi::parse( iter, boost::end( str ), rule, result ) && iter == boost::end( str ) ) {
        return result;
      }
      else
        return boost::optional< std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > >();
    }
    
    template< unsigned int length, typename String >
      typename uint_t< length >::least str2ip( const String &str,
          typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
          ) {
        boost::optional< uint32_t > temp = str2ipv4( str );
        if( !temp )
          throw std::runtime_error( "Invalid Syntax" );
        typename uint_t< length >::least value = *temp;
        return value;
      }

    template< unsigned int length, typename String >
      typename uint_t< length >::least str2ip( const String &str,
          typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
          ) {
        boost::optional< std::pair< uint64_t, uint64_t > > temp = str2ipv6( str );
        if( !temp )
          throw std::runtime_error( "Invalid Syntax" );
        typename uint_t< length >::least value = (*temp).first;
        value <<= 64;
        value |= (*temp).second;
        return value;
      }

    template< unsigned int length, typename String >
      typename std::pair< typename uint_t< length >::least, typename uint_t< length >::least > str2ipsegment( const String &str,
          typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
          ) {
        auto temp = str2ipv4segment( str );
        if( !temp )
          throw std::runtime_error( "Invalid Syntax" );
        typename uint_t< length >::least address = (*temp).first;
        typename uint_t< length >::least mask = (*temp).second;
        return std::make_pair( address, mask );
      }

    template< unsigned int length, typename String >
      typename std::pair< typename uint_t< length >::least, typename uint_t< length >::least > str2ipsegment( const String &str,
          typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
          ) {
        auto temp = str2ipv6segment( str );
        if( !temp )
          throw std::runtime_error( "Invalid Syntax" );
        typename uint_t< length >::least address = (*temp).first.first;
        address <<= 64;
        address |= (*temp).first.second;
        typename uint_t< length >::least mask = (*temp).second.first;
        mask <<= 64;
        mask |= (*temp).second.second;
        return std::make_pair( address, mask );
      }

    template< unsigned int length >
      std::string ip2str( typename uint_t< length >::least value,
          typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
          ) {
        return
          ( boost::format("%d.%d.%d.%d") %
            ( static_cast< int >( static_cast< uint8_t >( value >> 24 ) ) ) %
            ( static_cast< int >( static_cast< uint8_t >( value >> 16 ) ) ) %
            ( static_cast< int >( static_cast< uint8_t >( value >> 8 ) ) ) %
            ( static_cast< int >( static_cast< uint8_t >( value ) ) ) ).str();
      }

    template< unsigned int length >
      std::string ip2str( typename uint_t< length >::least value,
          typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
          ) {
        return
          ( boost::format("%X:%X:%X:%X:%X:%X:%X:%X") %
            ( static_cast< int >( static_cast<uint16_t>( value >> 112 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 96 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 80 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 64 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 48 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 32 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value >> 16 ) ) ) %
            ( static_cast< int >( static_cast<uint16_t>( value ) ) ) ).str();
      }

  }

  template< unsigned int length >
    class ip {
      public:
        typedef typename uint_t< length >::least value_type;
        ip() : value( 0 ) {}
        template< typename String >
        ip( const String &value_,
          typename boost::enable_if< boost::spirit::traits::is_container< String > >::type* = 0
        ) : value( detail::str2ip< length >( value_ ) ) {
        }
        ip( const char *value_
        ) : value( detail::str2ip< length >( std::string( value_ ) ) ) {
        }
        ip( const value_type &value_ ) : value( value_ & get_mask() ) {
        }
        ip<length> operator|( const ip<length> &right ) const {
          return ip<length>( value | right.value );
        }
        ip<length> operator&( const ip<length> &right ) const {
          return ip<length>( value & right.value );
        }
        ip<length> operator^( const ip<length> &right ) const {
          return ip<length>( value ^ right.value );
        }
        ip<length> &operator|=( const ip<length> &right ) {
          value |= right.value;
          return *this;
        }
        ip<length> &operator&=( const ip<length> &right ) {
          value &= right.value;
          return *this;
        }
        ip<length> &operator^=( const ip<length> &right ) {
          value ^= right.value;
          return *this;
        }
        bool operator==( const ip<length> &right ) const {
          return value == right.value;
        }
        bool operator!=( const ip<length> &right ) const {
          return value != right.value;
        }
        bool operator>( const ip<length> &right ) const {
          return value > right.value;
        }
        bool operator<( const ip<length> &right ) const {
          return value < right.value;
        }
        bool operator>=( const ip<length> &right ) const {
          return value >= right.value;
        }
        bool operator<=( const ip<length> &right ) const {
          return value <= right.value;
        }
        value_type get_raw() const {
          return value;
        }
      private:
        static value_type get_mask() {
          value_type top( 1 );
          top <<= ( length - 1 );
          value_type sub = top;
          --sub;
          return top|sub;
        }
        static ip<length> get_max_address() {
          return ip( get_mask() );
        }
        static ip<length> get_subnetmask( unsigned int bits ) {
          value_type mask( 1 );
          mask <<= ( length - bits );
          --mask;
          return ip( mask ) ^ get_max_address();
        }
        value_type value;
    };


  template< unsigned int length >
    class ip_segment {
      public:
        typedef typename uint_t< length >::least value_type;
        ip_segment() {}
        ip_segment( std::pair< value_type, value_type > value ) : address( value.first ), mask( value.second ) {}
        
        template< typename String >
        ip_segment( const String &str,
          typename boost::enable_if< boost::spirit::traits::is_container< String > >::type* = 0 ) {
          const std::pair< typename uint_t< length >::least, typename uint_t< length >::least > temp = detail::str2ipsegment< length >( str );
          address = temp.first;
          mask = temp.second;
        }
        ip_segment( const char *str ) {
          const std::pair< typename uint_t< length >::least, typename uint_t< length >::least > temp = detail::str2ipsegment< length >( str );
          address = temp.first;
          mask = temp.second;
        }
        template< typename AddrType, typename MaskType >
          ip_segment( AddrType address_, MaskType mask_ ) : address( address_ ), mask( mask_ ) {}
        bool operator==( const ip<length> &right ) const {
          return address == right.address && mask == right.mask;
        }
        bool operator!=( const ip<length> &right ) const {
          return address != right.address || mask != right.mask;
        }
        bool contains( ip< length > address_ ) const {
          return address == ( mask & address_ );
        }
        const ip< length > &get_address() const {
          return address;
        }
        const ip< length > &get_mask() const {
          return mask;
        }
      private:
        ip< length > address;
        ip< length > mask;
    };

    typedef ip< 32 > ipv4;
    typedef ip< 128 > ipv6;


    typedef ip_segment< 32 > ipv4segment;
    typedef ip_segment< 128 > ipv6segment;

    template< typename Iterator >
      class ipv4_parser :
        public boost::spirit::qi::grammar<
        Iterator,
        ipv4()
        > {
          public:
            ipv4_parser() : ipv4_parser::base_type( root ) {
              using namespace boost::spirit;
              root = ipv4address[
                _val = boost::phoenix::bind( &create, _1 )
                ];
            }
          private:
            static ipv4 create(
                uint32_t value
                ) {
              return ipv4( value );
            }
            boost::spirit::qi::rule< Iterator, ipv4() > root;
            detail::ipv4_rule< Iterator > ipv4address;
        };


    template< typename Iterator >
      class ipv6_parser :
        public boost::spirit::qi::grammar<
        Iterator,
        ipv6()
        > {
          public:
            ipv6_parser() : ipv6_parser::base_type( root ) {
              using namespace boost::spirit;
              root = ipv6address[
                _val = boost::phoenix::bind( &create, _1 )
                ];
            }
          private:
            static ipv6 create(
                std::pair< uint64_t, uint64_t > value
                ) {
                uint_t< 128 >::least temp = value.first;
                temp <<= 64;
                temp |= value.second;
              return ipv6( temp );
            }
            boost::spirit::qi::rule< Iterator, ipv6() > root;
            detail::ipv6_phase1_rule< Iterator > ipv6address;
        };

    template< typename Iterator >
      class ipv4segment_parser :
        public boost::spirit::qi::grammar<
        Iterator,
        ipv4segment()
        > {
          public:
            ipv4segment_parser() : ipv4segment_parser::base_type( root ) {
              using namespace boost::spirit;
              root = ipv4address[
                _val = boost::phoenix::bind( &create, _1 )
                ];
            }
          private:
            static ipv4segment create(
                std::pair< uint32_t, uint32_t > value
                ) {
              return ipv4segment( value.first, value.second );
            }
            boost::spirit::qi::rule< Iterator, ipv4segment() > root;
            detail::ipv4segment_rule< Iterator > ipv4address;
        };


    template< typename Iterator >
      class ipv6segment_parser :
        public boost::spirit::qi::grammar<
        Iterator,
        ipv6segment()
        > {
          public:
            ipv6segment_parser() : ipv6segment_parser::base_type( root ) {
              using namespace boost::spirit;
              root = ipv6address[
                _val = boost::phoenix::bind( &create, _1 )
                ];
            }
          private:
            static ipv6segment create(
                std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > value
                ) {
              return ipv6segment( value.first, value.second );
            }
            boost::spirit::qi::rule< Iterator, ipv6segment() > root;
            detail::ipv6segment_rule< Iterator > ipv6address;
        };


  template< unsigned int length >
    std::istream &operator>>( std::istream &stream, ip< length > &value ) {
      std::string temp;
      stream >> temp;
      value = ip< length >( detail::str2ip< length >( temp ) );
      return stream;
    }
  template< unsigned int length >
    std::istream &operator>>( std::istream &stream, ip_segment< length > &value ) {
      std::string temp;
      stream >> temp;
      value = ip_segment< length >( detail::str2ipsegment< length >( temp ) );
      return stream;
    }
  template< unsigned int length >
    std::ostream &operator<<( std::ostream &stream, const ip< length > &value ) {
      std::string temp = detail::ip2str< length >( value.get_raw() );
      stream << temp;
      return stream;
    }

  template< unsigned int length >
    bool is_loopback( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
        ) {
      static const ip_segment< 32 > loopback( "127.0.0.0/8" );
      return loopback.contains( value );
    }

  template< unsigned int length >
    bool is_loopback( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
        ) {
      static const ip< 128 > loopback( "::1" );
      return loopback == value; 
    }

  template< unsigned int length >
    bool is_unspecified( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
        ) {
      static const ip< 32 > unspecified( "0.0.0.0" );
      return unspecified == value; 
    }

  template< unsigned int length >
    bool is_unspecified( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
        ) {
      static const ip< 128 > unspecified( "0::0" );
      return unspecified == value; 
    }

  template< unsigned int length >
    bool is_local( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
        ) {
      static const ip_segment< 32 > local_a( "10.0.0.0/8" );
      static const ip_segment< 32 > local_b( "172.16.0.0/12" );
      static const ip_segment< 32 > local_c( "192.168.0.0/16" );
      return local_a.contains( value ) || local_b.contains( value ) || local_c.contains( value );
    }

  template< unsigned int length >
    bool is_local( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
        ) {
      static const ip_segment< 128 > local( "FC00::/7" );
      return local.contains( value );
    }

  template< unsigned int length >
    bool is_link_local( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
        ) {
      static const ip_segment< 32 > link_local( "169.254.0.0/16" );
      return link_local.contains( value );
    }

  template< unsigned int length >
    bool is_link_local( const ip< length > &value,
        typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
        ) {
      static const ip_segment< 128 > link_local( "FE80::/10" );
      return link_local.contains( value );
    }
}

#endif
