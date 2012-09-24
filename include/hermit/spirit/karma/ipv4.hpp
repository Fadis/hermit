#ifndef HERMIT_SPIRIT_KARMA_IPV4_HPP
#define HERMIT_SPIRIT_KARMA_IPV4_HPP

#include <cstdint>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/ip.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename Iterator >
        class ipv4 : public boost::spirit::karma::grammar<
          Iterator,
          hermit::ipv4()
        > {
          public:
            ipv4() : ipv4::base_type( root ) {
              namespace karma = boost::spirit::karma;
              namespace phx = boost::phoenix;
              root = ( dec_p << '.' << dec_p << '.' << dec_p << '.' << dec_p )[
                karma::_1 = phx::static_cast_< uint8_t >( karma::_val >> 24 ),
                karma::_2 = phx::static_cast_< uint8_t >( karma::_val >> 16 ),
                karma::_3 =  phx::static_cast_< uint8_t >( karma::_val >> 8 ),
                karma::_4 =  phx::static_cast_< uint8_t >( karma::_val )
              ];
            }
          private:
            boost::spirit::karma::uint_generator<uint8_t, 10> dec_p;
            boost::spirit::karma::rule< Iterator, hermit::ipv4() > root;
        };

      template< typename Iterator >
        class ipv4segment : public boost::spirit::karma::grammar<
          Iterator,
          std::pair< hermit::ipv4, hermit::ipv4 >()
        > {
          public:
            ipv4segment() : ipv4segment::base_type( root ) {
              namespace karma = boost::spirit::karma;
              namespace phx = boost::phoenix;
              root = ( ipv4address << '/' << dec_p )[
                karma::_1 = phx::at_c< 0 >( karma::_val ),
                karma::_2 = phx::bind( &ipv4segment::count_bit, 0xFFFFFFFFul ^ phx::at_c< 1 >( karma::_val ) ),
                karma::_pass = karma::_2 != 0
              ];
            }
          private:
            static int count_bit( uint32_t src ) {
              int count;
              for( count = 0; src&1; ++count )
                src >>=1;
              if( src )
                return 0;
              else
                return 32 - count;
            }
            boost::spirit::karma::uint_generator<uint8_t, 10> dec_p;
            ipv4< Iterator > ipv4address;
            boost::spirit::karma::rule< Iterator, std::pair< hermit::ipv4, hermit::ipv4 >() > root;
        };
    }
  }
}

#endif
