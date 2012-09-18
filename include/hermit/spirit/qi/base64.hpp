#ifndef HERMIT_SPIRIT_QI_BASE64_HPP
#define HERMIT_SPIRIT_QI_BASE64_HPP

#include <vector>
#include <string>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace hermit {
namespace spirit {
namespace qi {
template <typename InputIterator>
class base64 :
  public boost::spirit::qi::grammar< InputIterator, std::vector< uint8_t >() > {
    public:
      base64() : base64::base_type( root ) {
        namespace qi = boost::spirit::qi;
        namespace phx = boost::phoenix;
        base64alpha.add( "A", 0 )( "B", 1 )( "C", 2 )( "D", 3 )( "E", 4 )
          ( "F", 5 )( "G", 6 )( "H", 7 )( "I", 8 )( "J", 9 )( "K", 10 )
          ( "L", 11 )( "M", 12 )( "N", 13 )( "O", 14 )( "P", 15 )
          ( "Q", 16 )( "R", 17 )( "S", 18 )( "T", 19 )( "U", 20 )
          ( "V", 21 )( "W", 22 )( "X", 23 )( "Y", 24 )( "Z", 25 )
          ( "a", 26 )( "b", 27 )( "c", 28 )( "d", 29 )( "e", 30 )
          ( "f", 31 )( "g", 32 )( "h", 33 )( "i", 34 )( "j", 35 )
          ( "k", 36 )( "l", 37 )( "m", 38 )( "n", 39 )( "o", 40 )
          ( "p", 41 )( "q", 42 )( "r", 43 )( "s", 44 )( "t", 45 )
          ( "u", 46 )( "v", 47 )( "w", 48 )( "x", 49 )( "y", 50 )
          ( "z", 51 )( "0", 52 )( "1", 53 )( "2", 54 )( "3", 55 )
          ( "4", 56 )( "5", 57 )( "6", 58 )( "7", 59 )( "8", 60 )
          ( "9", 61 )( "+", 62 )( "/", 63 )( "=", 0 );
        block = ( base64alpha >> base64alpha >> base64alpha >> base64alpha )[
          qi::_a = ( qi::_1 << 18 )|( qi::_2 << 12 )|( qi::_3 << 6 )|qi::_4,
          phx::push_back( qi::_val, qi::_a >> 16 ),
          phx::push_back( qi::_val, ( qi::_a >> 8 ) & 0xFF ),
          phx::push_back( qi::_val, qi::_a & 0xFF )
        ];
        root =
          qi::skip( qi::byte_ - base64alpha )[ *block[ phx::insert( qi::_val, phx::end( qi::_val ), phx::begin( qi::_1 ), phx::end( qi::_1 ) ) ] ];
      }
    private:
      boost::spirit::qi::rule< InputIterator, std::vector< uint8_t >(), boost::spirit::qi::locals< uint32_t > > block;
      boost::spirit::qi::rule< InputIterator, std::vector< uint8_t >() > root;
      boost::spirit::qi::symbols< char, uint32_t > base64alpha;
  };
}
}
}

#endif

