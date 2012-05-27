#ifndef HERMIT_IP_HPP
#define HERMIT_IP_HPP
#include <stdexcept>
#include <boost/utility/enable_if.hpp>
#include <boost/format.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/lexical_cast.hpp>
#include <hermit/integer.hpp>

namespace hermit {
  namespace detail {
    boost::optional< uint32_t > str2ipv4( const std::string &str );
    boost::optional< std::pair< uint32_t, uint32_t > > str2ipv4segment( const std::string &str );
    boost::optional< std::pair< uint64_t, uint64_t > > str2ipv6( const std::string &str );
    boost::optional< std::pair< std::pair< uint64_t, uint64_t >, std::pair< uint64_t, uint64_t > > > str2ipv6segment( const std::string &str );
    
    template< unsigned int length >
    typename uint_t< length >::least str2ip( const std::string &str,
      typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
    ) {
      boost::optional< uint32_t > temp = str2ipv4( str );
      if( !temp )
        throw std::runtime_error( "Invalid Syntax" );
      typename uint_t< length >::least value = *temp;
      return value;
    }

    template< unsigned int length >
    typename uint_t< length >::least str2ip( const std::string &str,
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

    template< unsigned int length >
    typename std::pair< typename uint_t< length >::least, typename uint_t< length >::least > str2ipsegment( const std::string &str,
      typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
    ) {
      auto temp = str2ipv4segment( str );
      if( !temp )
        throw std::runtime_error( "Invalid Syntax" );
      typename uint_t< length >::least address = (*temp).first;
      typename uint_t< length >::least mask = (*temp).second;
      return std::make_pair( address, mask );
    }

    template< unsigned int length >
    typename std::pair< typename uint_t< length >::least, typename uint_t< length >::least > str2ipsegment( const std::string &str,
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
        ip( const std::string &value_ ) : value( detail::str2ip< length >( value_ ) ) {
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
        ip_segment( const std::string &str ) {
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
