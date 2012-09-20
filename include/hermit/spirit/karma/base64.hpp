#ifndef HERMIT_SPIRIT_KARMA_BASE64
#define HERMIT_SPIRIT_KARMA_BASE64

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename OutputIterator>
      class base64 :
      public boost::spirit::karma::grammar< OutputIterator, std::vector< uint8_t >() > {
        public:
          base64() : base64::base_type( root ) {
            namespace karma = boost::spirit::karma;
            namespace phx = boost::phoenix;
            base64alpha.add( 0, 'A' )( 1, 'B' )( 2, 'C' )( 3, 'D' )( 4, 'E' )
              ( 5, 'F' )( 6, 'G' )( 7, 'H' )( 8, 'I' )( 9, 'J' )( 10, 'K' )
              ( 11, 'L' )( 12, 'M' )( 13, 'N' )( 14, 'O' )( 15, 'P' )
              ( 16, 'Q' )( 17, 'R' )( 18, 'S' )( 19, 'T' )( 20, 'U' )
              ( 21, 'V' )( 22, 'W' )( 23, 'X' )( 24, 'Y' )( 25, 'Z' )
              ( 26, 'a' )( 27, 'b' )( 28, 'c' )( 29, 'd' )( 30, 'e' )
              ( 31, 'f' )( 32, 'g' )( 33, 'h' )( 34, 'i' )( 35, 'j' )
              ( 36, 'k' )( 37, 'l' )( 38, 'm' )( 39, 'n' )( 40, 'o' )
              ( 41, 'p' )( 42, 'q' )( 43, 'r' )( 44, 's' )( 45, 't' )
              ( 46, 'u' )( 47, 'v' )( 48, 'w' )( 49, 'x' )( 50, 'y' )
              ( 51, 'z' )( 52, '0' )( 53, '1' )( 54, '2' )( 55, '3' )
              ( 56, '4' )( 57, '5' )( 58, '6' )( 59, '7' )( 60, '8' )
              ( 61, '9' )( 62, '+' )( 63, '/' );
            block_3 = ( ( base64alpha << base64alpha << base64alpha << base64alpha )[
              phx::if_( phx::size( karma::_val ) == 3 )[
                karma::_pass = true,
                karma::_a =
                  ( phx::static_cast_<uint32_t>( phx::at( karma::_val, 0 ) ) << 16 )|
                  ( phx::static_cast_<uint32_t>( phx::at( karma::_val, 1 ) ) << 8 )|
                  phx::static_cast_<uint32_t>( phx::at( karma::_val, 2 ) ),
                karma::_1 = karma::_a >> 18,
                karma::_2 = ( karma::_a >> 12 ) & 0x3F,
                karma::_3 = ( karma::_a >> 6 ) & 0x3F,
                karma::_4 = karma::_a & 0x3F
              ].else_[
                karma::_pass = false
              ]
            ] );
            block_2 = ( ( base64alpha << base64alpha << base64alpha << '=' )[
              phx::if_( phx::size( karma::_val ) == 2 )[
                karma::_pass = true,
                karma::_a =
                  ( phx::static_cast_<uint32_t>( phx::at( karma::_val, 0 ) ) << 16 )|
                  ( phx::static_cast_<uint32_t>( phx::at( karma::_val, 1 ) ) << 8 ),
                karma::_1 = karma::_a >> 18,
                karma::_2 = ( karma::_a >> 12 ) & 0x3F,
                karma::_3 = ( karma::_a >> 6 ) & 0x3F
              ].else_[
                karma::_pass = false
              ]
            ] );
            block_1 = ( ( base64alpha << base64alpha << '=' << '=' )[
              phx::if_( phx::size( karma::_val ) == 1 )[
                karma::_pass = true,
                karma::_a =
                  ( phx::static_cast_<uint32_t>( phx::at( karma::_val, 0 ) ) << 16 ),
                karma::_1 = karma::_a >> 18,
                karma::_2 = ( karma::_a >> 12 ) & 0x3F
              ].else_[
                karma::_pass = false
              ]
            ] );
            block = block_3|block_2|block_1;
            line = karma::eps[ karma::_a = phx::begin( karma::_val ) ] <<
            *block[
              karma::_pass = karma::_a != phx::end( karma::_val ),
              phx::clear( karma::_1 ),
              phx::push_back( karma::_1, *( karma::_a++ ) ),
              phx::if_(
                karma::_a != phx::end( karma::_val )
              )[
                phx::push_back( karma::_1, *( karma::_a++ ) ),
                phx::if_(
                  karma::_a != phx::end( karma::_val )
                )[
                  phx::push_back( karma::_1, *( karma::_a++ ) )
                ]
              ]
            ];
            root = line;
          }
        private:
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >(), boost::spirit::karma::locals< std::vector< uint8_t >::const_iterator > > line;
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >() > block;
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > block_3;
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > block_2;
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > block_1;
          boost::spirit::karma::rule< OutputIterator, std::vector< uint8_t >() > root;
          boost::spirit::karma::symbols< uint8_t, char > base64alpha;
      };
    }
  }
}

#endif

