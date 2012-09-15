#ifndef HERMIT_SPIRIT_KARMA_MESSAGEPACK_HPP
#define HERMIT_SPIRIT_KARMA_MESSAGEPACK_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/messagepack.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename Iterator >
      class messagepack :
        public boost::spirit::karma::grammar<
          Iterator,
          hermit::messagepack()
        > {
      public:
        messagepack() : messagepack::base_type( value_ ) {
          namespace karma = boost::spirit::karma;
          uint8_short = karma::byte_[
            karma::_pass = karma::_val <= 0x7F,
            karma::_1 = karma::_val
          ];
          uint8_long = ( karma::byte_( 0xcc ) << karma::byte_ );
          uint8_ = uint8_short|uint8_long;
          uint16_ = karma::byte_( 0xcd ) << karma::big_word;
          uint32_ = karma::byte_( 0xce ) << karma::big_dword;
          uint64_ = karma::byte_( 0xcf ) << karma::big_qword;
          int8_short = karma::byte_[
            karma::_pass = karma::_val < 0 && karma::_val > -32,
            karma::_1 = boost::phoenix::static_cast_<uint8_t>( karma::_val )
          ];
          int8_long = karma::byte_( 0xd0 ) << karma::byte_[
            karma::_1 = boost::phoenix::static_cast_<uint8_t>( karma::_val )
          ];
          int8_ = int8_short|int8_long;
          int16_ = karma::byte_( 0xd1 ) << karma::big_word[
            karma::_1 = boost::phoenix::static_cast_<uint16_t>( karma::_val )
          ];
          int32_ = karma::byte_( 0xd2 ) << karma::big_dword[
            karma::_1 = boost::phoenix::static_cast_<uint32_t>( karma::_val )
          ];
          int64_ = karma::byte_( 0xd3 ) << karma::big_qword[
            karma::_1 = boost::phoenix::static_cast_<uint64_t>( karma::_val )
          ];
          null_ = karma::byte_( 0xc0 );
          bool_ = karma::byte_( 0xc3 )[ karma::_pass = karma::_val ]|karma::byte_( 0xc2 );
          float_ = karma::byte_( 0xca ) << karma::big_bin_float;
          double_ = karma::byte_( 0xcb ) << karma::big_bin_double;
          raw_short = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x1F,
            karma::_1 = 0xC0|boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ karma::byte_ ][
            karma::_1 = karma::_val
          ];
          raw_16 = ( karma::byte_( 0xda ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ karma::byte_ ][
            karma::_1 = karma::_val
          ];
          raw_32 = ( karma::byte_( 0xdb ) << karma::big_dword )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ karma::byte_ ][
            karma::_1 = karma::_val
          ];
          raw = raw_short|raw_16|raw_32;
          array_short = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x0F,
            karma::_1 = 0x90|boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ value_ ][
            karma::_1 = karma::_val
          ];
          array_16 = ( karma::byte_( 0xdc ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ value_ ][
            karma::_1 = karma::_val
          ];
          array_32 = ( karma::byte_( 0xdd ) << karma::big_dword )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val )
          ] << karma::repeat( boost::phoenix::size( karma::_val ) )[ value_ ][
            karma::_1 = karma::_val
          ];
          array = array_short|array_16|array_32;
          named_value = value_ << value_;
          map_short = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x0F,
            karma::_1 = 0x80|boost::phoenix::size( karma::_val )
          ];
          map_16 = ( karma::byte_( 0xde ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val )
          ];
          map_32 = ( karma::byte_( 0xdf ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val )
          ];
          map_data = karma::repeat( boost::phoenix::size( karma::_val ) )[ named_value ];
          map = ( ( map_short|map_16|map_32 ) << map_data )[ karma::_1 = karma::_val, karma::_2 = karma::_val ];
          value_ = uint8_|uint16_|uint32_|uint64_|int8_|int16_|int32_|int64_|null_|bool_|float_|double_|raw|array|map;
      }
      private: 
        boost::spirit::karma::rule< Iterator, uint8_t() > uint8_short;
        boost::spirit::karma::rule< Iterator, uint8_t() > uint8_long;
        boost::spirit::karma::rule< Iterator, uint8_t() > uint8_;
        boost::spirit::karma::rule< Iterator, uint16_t() > uint16_;
        boost::spirit::karma::rule< Iterator, uint32_t() > uint32_;
        boost::spirit::karma::rule< Iterator, uint64_t() > uint64_;
        boost::spirit::karma::rule< Iterator, int8_t() > int8_short;
        boost::spirit::karma::rule< Iterator, int8_t() > int8_long;
        boost::spirit::karma::rule< Iterator, int8_t() > int8_;
        boost::spirit::karma::rule< Iterator, int16_t() > int16_;
        boost::spirit::karma::rule< Iterator, int32_t() > int32_;
        boost::spirit::karma::rule< Iterator, int64_t() > int64_;
        boost::spirit::karma::rule< Iterator, std::nullptr_t() > null_;
        boost::spirit::karma::rule< Iterator, bool() > bool_;
        boost::spirit::karma::rule< Iterator, float() > float_;
        boost::spirit::karma::rule< Iterator, double() > double_;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >() > raw_short;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >() > raw_16;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >() > raw_32;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >() > raw;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::messagepack >() > array_short;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::messagepack >() > array_16;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::messagepack >() > array_32;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::messagepack >() > array;
        boost::spirit::karma::rule< Iterator, std::pair< hermit::messagepack, hermit::messagepack >() > named_value;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >() > map_short;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >() > map_data;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >() > map_16;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >() > map_32;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >() > map;
        boost::spirit::karma::rule< Iterator, hermit::messagepack() > value_;
      };
    }
  }
}

#endif

