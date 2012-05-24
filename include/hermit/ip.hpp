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
    boost::optional< std::pair< uint64_t, uint64_t > > str2ipv6( const std::string &str );

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
    std::string ip2str( typename uint_t< length >::least value,
      typename boost::enable_if< boost::mpl::bool_< length == 32 > >::type* = 0
    ) {
      return
        ( boost::format("%d.%d.%d.%d") %
          ( static_cast< uint8_t >( value >> 24 ) ) %
          ( static_cast< uint8_t >( value >> 16 ) ) %
          ( static_cast< uint8_t >( value >> 8 ) ) %
          ( static_cast< uint8_t >( value ) ) ).str();
    }
    
    template< unsigned int length >
    std::string ip2str( typename uint_t< length >::least value,
      typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
    ) {
      return
        ( boost::format("%X:%X:%X:%X:%X:%X:%X:%X") %
          ( static_cast<uint16_t>( value >> 112 ) ) %
          ( static_cast<uint16_t>( value >> 96 ) ) %
          ( static_cast<uint16_t>( value >> 80 ) ) %
          ( static_cast<uint16_t>( value >> 64 ) ) %
          ( static_cast<uint16_t>( value >> 48 ) ) %
          ( static_cast<uint16_t>( value >> 32 ) ) %
          ( static_cast<uint16_t>( value >> 16 ) ) %
          ( static_cast<uint16_t>( value ) ) ).str();
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
    std::istream &operator>>( std::istream &stream, ip< length > &value ) {
      std::string temp;
      stream >> temp;
      value = ip< length >( detail::str2ip< length >( temp ) );
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
      static const ip< 32 > loopback_mask( "255.0.0.0" );
      static const ip< 32 > loopback( "127.0.0.0" );
      return loopback == value & loopback_mask; 
    }

    template< unsigned int length >
    bool is_loopback( const ip< length > &value,
      typename boost::enable_if< boost::mpl::bool_< length == 128 > >::type* = 0
    ) {
      static const ip< 128 > loopback( "::1" );
      return loopback == value; 
    }
}

#endif
