#ifndef HERMIT_SPIRIT_KARMA_IPV6_HPP
#define HERMIT_SPIRIT_KARMA_IPV6_HPP

#include <utility>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/ip.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
    template< typename Iterator >
      class ipv6 : public boost::spirit::karma::grammar<
        Iterator,
        hermit::ipv6()
        > {
      public:
        ipv6() : ipv6::base_type( root ) {
          namespace karma = boost::spirit::karma;
          namespace phx = boost::phoenix;
          root = ( hex_p << ':' << hex_p << ':' << hex_p << ':' << hex_p << ':' <<
                   hex_p << ':' << hex_p << ':' << hex_p << ':' << hex_p )[
            karma::_1 = phx::static_cast_< uint16_t >( phx::at_c< 0 >( karma::_val ) >> 48 ),
            karma::_2 = phx::static_cast_< uint16_t >( phx::at_c< 0 >( karma::_val ) >> 32 ),
            karma::_3 = phx::static_cast_< uint16_t >( phx::at_c< 0 >( karma::_val ) >> 16 ),
            karma::_4 = phx::static_cast_< uint16_t >( phx::at_c< 0 >( karma::_val ) ),
            karma::_5 = phx::static_cast_< uint16_t >( phx::at_c< 1 >( karma::_val ) >> 48 ),
            karma::_6 = phx::static_cast_< uint16_t >( phx::at_c< 1 >( karma::_val ) >> 32 ),
            karma::_7 = phx::static_cast_< uint16_t >( phx::at_c< 1 >( karma::_val ) >> 16 ),
            karma::_8 = phx::static_cast_< uint16_t >( phx::at_c< 1 >( karma::_val ) )
          ];
        }
      private:
        boost::spirit::karma::uint_generator<uint16_t, 16> hex_p;
        boost::spirit::karma::rule< Iterator, hermit::ipv6() > root;
      };
    }
  }
}

#endif
