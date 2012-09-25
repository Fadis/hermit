#ifndef HERMIT_SPIRIT_QI_IPV6_HPP
#define HERMIT_SPIRIT_QI_IPV6_HPP

#include <vector>
#include <utility>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/optional.hpp>
#include <hermit/spirit/qi/ipv4.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/ip.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
    template< typename Iterator >
      class ipv6 : public boost::spirit::qi::grammar<
        Iterator,
        hermit::ipv6()
        > {
      public:
        ipv6() : ipv6::base_type( root ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          ipv6part = ( hex4_p - ipv4address ) % ( qi::lit(':') - "::" );
          ipv6_type1 = ( ipv6part >> "::" >> ipv6part >> ':' >> ipv4address )[
            qi::_pass = phx::size( qi::_1 ) + phx::size( qi::_2 ) < 6ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1,
            phx::at_c< 1 >( qi::_val ) = qi::_2,
            phx::push_back( phx::at_c< 1 >( qi::_val ), ( qi::_3 >> 16 ) & 0xFFFF ),
            phx::push_back( phx::at_c< 1 >( qi::_val ), qi::_3 & 0xFFFF )
          ];
          ipv6_type2 = ( "::" >> ipv6part >> ':' >> ipv4address )[
            qi::_pass = phx::size( qi::_1 ) < 6ul,
            phx::at_c< 1 >( qi::_val ) = qi::_1,
            phx::push_back( phx::at_c< 1 >( qi::_val ), ( qi::_2 >> 16 ) & 0xFFFF ),
            phx::push_back( phx::at_c< 1 >( qi::_val ), qi::_2 & 0xFFFF )
          ];
          ipv6_type3 = ( ipv6part >> "::" >> ipv4address )[
            qi::_pass = phx::size( qi::_1 ) < 6ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1,
            phx::push_back( phx::at_c< 1 >( qi::_val ), ( qi::_2 >> 16 ) & 0xFFFF ),
            phx::push_back( phx::at_c< 1 >( qi::_val ), qi::_2 & 0xFFFF )
          ];
          ipv6_type4 = ( ipv6part >> ":" >> ipv4address )[
            qi::_pass = phx::size( qi::_1 ) == 6ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1,
            phx::push_back( phx::at_c< 1 >( qi::_val ), ( qi::_2 >> 16 ) & 0xFFFF ),
            phx::push_back( phx::at_c< 1 >( qi::_val ), qi::_2 & 0xFFFF )
          ];
          ipv6_type5 = ( "::" >> ipv4address )[
            phx::push_back( phx::at_c< 1 >( qi::_val ), ( qi::_1 >> 16 ) & 0xFFFF ),
            phx::push_back( phx::at_c< 1 >( qi::_val ), qi::_1 & 0xFFFF )
          ];
          ipv6_type6 = ( ipv6part >> "::" >> ipv6part )[
            qi::_pass = phx::size( qi::_1 ) + phx::size( qi::_2 ) < 8ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1,
            phx::at_c< 1 >( qi::_val ) = qi::_2
          ];
          ipv6_type7 = ( "::" >> ipv6part )[
            qi::_pass = phx::size( qi::_1 ) < 8ul,
            phx::at_c< 1 >( qi::_val ) = qi::_1
          ];
          ipv6_type8 = ( ipv6part >> "::" )[
            qi::_pass = phx::size( qi::_1 ) < 8ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1
          ];
          ipv6_type9 = ipv6part[
            qi::_pass = phx::size( qi::_1 ) == 8ul,
            phx::at_c< 0 >( qi::_val ) = qi::_1
          ];
          ipv6_type0 = qi::lit( "::" )[
            qi::_pass =true
          ];
          ipv6address = (ipv6_type1|ipv6_type2|ipv6_type3|ipv6_type4|ipv6_type5|ipv6_type6|ipv6_type7|ipv6_type8|ipv6_type9|ipv6_type0 )[
            phx::resize( qi::_a, 8ul ),
            phx::fill( qi::_a, 0u ),
            phx::copy( phx::at_c< 0 >( qi::_1 ), phx::begin( qi::_a ) ),
            phx::reverse_copy( phx::at_c< 1 >( qi::_1 ), phx::rbegin( qi::_a ) ),
            phx::at_c< 0 >( qi::_val ) = phx::at( qi::_a, 0ul ),
            phx::at_c< 0 >( qi::_val ) <<= 16,
            phx::at_c< 0 >( qi::_val ) |= phx::at( qi::_a, 1ul ),
            phx::at_c< 0 >( qi::_val ) <<= 16,
            phx::at_c< 0 >( qi::_val ) |= phx::at( qi::_a, 2ul ),
            phx::at_c< 0 >( qi::_val ) <<= 16,
            phx::at_c< 0 >( qi::_val ) |= phx::at( qi::_a, 3ul ),
            phx::at_c< 1 >( qi::_val ) = phx::at( qi::_a, 4ul ),
            phx::at_c< 1 >( qi::_val ) <<= 16,
            phx::at_c< 1 >( qi::_val ) |= phx::at( qi::_a, 5ul ),
            phx::at_c< 1 >( qi::_val ) <<= 16,
            phx::at_c< 1 >( qi::_val ) |= phx::at( qi::_a, 6ul ),
            phx::at_c< 1 >( qi::_val ) <<= 16,
            phx::at_c< 1 >( qi::_val ) |= phx::at( qi::_a, 7ul )
          ];
          root = ipv6address;
        }
      private:
        boost::spirit::qi::uint_parser<uint16_t, 16, 1, 4> hex4_p;
        ipv4< Iterator > ipv4address;
        boost::spirit::qi::rule< Iterator, std::vector< uint16_t >() > ipv6part;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type1;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type2;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type3;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type4;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type5;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type6;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type7;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type8;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type9;
        boost::spirit::qi::rule< Iterator, std::pair< std::vector< uint16_t >, std::vector< uint16_t > >() > ipv6_type0;
        boost::spirit::qi::rule< Iterator, std::pair< std::uint64_t, std::uint64_t >(), boost::spirit::qi::locals< std::vector< uint16_t > > > ipv6address;
        boost::spirit::qi::rule< Iterator, hermit::ipv6() > root;
      };
    }
  }
}

#endif
