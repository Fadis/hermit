#ifndef HERMIT_SPIRIT_QI_IPV4_HPP
#define HERMIT_SPIRIT_QI_IPV4_HPP

#include <cstdint>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/ip.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template< typename Iterator >
        class ipv4 : public boost::spirit::qi::grammar<
          Iterator,
          hermit::ipv4()
        > {
          public:
            ipv4() : ipv4::base_type( root ) {
              namespace qi = boost::spirit::qi;
              namespace phx = boost::phoenix;
              root = ( dec3_p >> '.' >> dec3_p >> '.' >> dec3_p >> '.' >> dec3_p )[
                qi::_val =
                  phx::static_cast_< uint32_t >( qi::_1 ) << 24|
                  phx::static_cast_< uint32_t >( qi::_2 ) << 16|
                  phx::static_cast_< uint32_t >( qi::_3 ) << 8|
                  phx::static_cast_< uint32_t >( qi::_4 )
                ];
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 10, 1, 3> dec3_p; 
            boost::spirit::qi::rule< Iterator, hermit::ipv4() > root;
        };

      template< typename Iterator >
        class ipv4segment : public boost::spirit::qi::grammar<
          Iterator,
          std::pair< hermit::ipv4, hermit::ipv4 >()
        > {
          public:
            ipv4segment() : ipv4segment::base_type( root ) {
              namespace qi = boost::spirit::qi;
              namespace phx = boost::phoenix;
              root = ( ipv4address >> '/' >> dec2_p )[
                phx::if_( qi::_2 <= 32 && qi::_2 > 0 )[
                  phx::at_c< 1 >( qi::_val ) = 0xFFFFFFFFul ^ ( ( 1 << ( 32 - qi::_2 ) ) - 1 ),
                  phx::if_( ( phx::at_c< 1 >( qi::_val ) & qi::_1 ) == qi::_1 )[
                    phx::at_c< 0 >( qi::_val ) = qi::_1,
                    qi::_pass = true
                  ].else_[
                    qi::_pass = false
                  ]
                ].else_[
                  qi::_pass = false
                ]
              ];
            }
          private:
            boost::spirit::qi::uint_parser<uint8_t, 10, 1, 2> dec2_p; 
            ipv4< Iterator > ipv4address;
            boost::spirit::qi::rule< Iterator, std::pair< hermit::ipv4, hermit::ipv4 >() > root;
        };
    }
  }
}

#endif
