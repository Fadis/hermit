#ifndef HERMIT_SPIRIT_MESSAGEPACK_HPP
#define HERMIT_SPIRIT_MESSAGEPACK_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/messagepack.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template< typename Iterator >
      class messagepack :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::messagepack()
        > {
      public:
        messagepack() : messagepack::base_type( value_ ) {
          namespace qi = boost::spirit::qi;
          uint8_ = qi::byte_[ qi::_pass = qi::_1 < 0x80, qi::_val = qi::_1 ] | ( qi::byte_( 0xcc ) >> qi::byte_ );
          uint16_ = qi::byte_( 0xcd ) >> qi::big_word;
          uint32_ = qi::byte_( 0xce ) >> qi::big_dword;
          uint64_ = qi::byte_( 0xcf ) >> qi::big_qword;
          int8_ =
            qi::byte_[ qi::_pass = ( qi::_1 & 0xE0 ) == 0xE0, qi::_val = -boost::phoenix::static_cast_<int8_t>( 0xFF - qi::_1 )-1 ] |
            ( qi::byte_( 0xd0 ) >> qi::byte_[ boost::phoenix::if_( qi::_1 < 0x80 )[
              qi::_val = qi::_1
            ].else_[
              qi::_val = -boost::phoenix::static_cast_<int8_t>( 0xFF - qi::_1 )-1
            ]
          ] );
          int16_ =
            qi::byte_( 0xd1 ) >> qi::big_word[ boost::phoenix::if_( qi::_1 < 0x8000 )[
              qi::_val = qi::_1
            ].else_[
              qi::_val = -boost::phoenix::static_cast_<int16_t>( 0xFFFF - qi::_1 )-1
            ]
          ];
          int32_ =
            qi::byte_( 0xd2 ) >> qi::big_dword[ boost::phoenix::if_( qi::_1 < 0x80000000ul )[
              qi::_val = qi::_1
            ].else_[
              qi::_val = -boost::phoenix::static_cast_<int32_t>( 0xFFFFFFFFul - qi::_1 )-1
            ]
          ];
          int64_ =
            qi::byte_( 0xd3 ) >> qi::big_qword[ boost::phoenix::if_( qi::_1 < 0x8000000000000000ull )[
              qi::_val = qi::_1
            ].else_[
              qi::_val = -boost::phoenix::static_cast_<int64_t>( 0xFFFFFFFFFFFFFFFFull - qi::_1 )-1
            ]
          ];
          null_ = qi::byte_( 0xc0 )[ qi::_val = nullptr ];
          bool_ = qi::byte_( 0xc3 )[ qi::_val = true ]|qi::byte_( 0xc2 )[ qi::_val = false ];
          float_ = qi::byte_( 0xca ) >> qi::big_bin_float;
          double_ = qi::byte_( 0xcb ) >> qi::big_bin_double;
          raw = qi::omit[
            qi::byte_[ qi::_pass = ( qi::_1 & 0xE0 ) == 0xC0, qi::_a = qi::_1 & 0x1F ]|
            ( qi::byte_( 0xda ) >> qi::big_word[ qi::_a = qi::_1 ] )|
            ( qi::byte_( 0xdb ) >> qi::big_dword[ qi::_a = qi::_1 ] )
          ] >> qi::repeat(qi::_a)[ qi::byte_ ];
          array = qi::omit[
            qi::byte_[ qi::_pass = ( qi::_1 & 0xF0 ) == 0x90, qi::_a = qi::_1 & 0x0F ]|
            ( qi::byte_( 0xdc ) >> qi::big_word[ qi::_a = qi::_1 ] )|
            ( qi::byte_( 0xdd ) >> qi::big_dword[ qi::_a = qi::_1 ] )
          ] >> qi::repeat(qi::_a)[ value_ ];
          named_value = value_ >> value_;
          map = qi::omit[
            qi::byte_[ qi::_pass = ( qi::_1 & 0xF0 ) == 0x80, qi::_a = qi::_1 & 0x0F ]|
            ( qi::byte_( 0xde ) >> qi::big_word[ qi::_a = qi::_1 ] )|
            ( qi::byte_( 0xdf ) >> qi::big_dword[ qi::_a = qi::_1 ] )
          ] >> qi::repeat(qi::_a)[ named_value ];
          value_ = uint8_|uint16_|uint32_|uint64_|int8_|int16_|int32_|int64_|null_|bool_|float_|double_|raw|array|map;
        } 
      private:
        boost::spirit::qi::rule< Iterator, uint8_t() > uint8_;
        boost::spirit::qi::rule< Iterator, uint16_t() > uint16_;
        boost::spirit::qi::rule< Iterator, uint32_t() > uint32_;
        boost::spirit::qi::rule< Iterator, uint64_t() > uint64_;
        boost::spirit::qi::rule< Iterator, int8_t() > int8_;
        boost::spirit::qi::rule< Iterator, int16_t() > int16_;
        boost::spirit::qi::rule< Iterator, int32_t() > int32_;
        boost::spirit::qi::rule< Iterator, int64_t() > int64_;
        boost::spirit::qi::rule< Iterator, std::nullptr_t() > null_;
        boost::spirit::qi::rule< Iterator, bool() > bool_;
        boost::spirit::qi::rule< Iterator, float() > float_;
        boost::spirit::qi::rule< Iterator, double() > double_;
        boost::spirit::qi::rule< Iterator, std::vector< uint8_t >(), boost::spirit::qi::locals< uint32_t > > raw;
        boost::spirit::qi::rule< Iterator, std::vector< hermit::messagepack >(), boost::spirit::qi::locals< uint32_t > > array;
        boost::spirit::qi::rule< Iterator, std::pair< hermit::messagepack, hermit::messagepack >() > named_value;
        boost::spirit::qi::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >(), boost::spirit::qi::locals< uint32_t > > map;
        boost::spirit::qi::rule< Iterator, hermit::messagepack() > value_;
 
      };
    }
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
/*          raw_short = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x1F,
            karma::_1 = 0xC0|boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ] << karma::repeat( karma::_a )[ karma::byte_ ];
          raw_16 = ( karma::byte_( 0xda ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ] << karma::repeat( karma::_a )[ karma::byte_ ];
          raw_32 =( karma::byte_( 0xdb ) << karma::big_dword )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ] << karma::repeat( karma::_a )[ karma::byte_ ];
          raw = raw_short|raw_16|raw_32;
          array = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x0F,
            karma::_1 = 0x90|boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ]|
          ( karma::byte_( 0xdc ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ]|
          ( karma::byte_( 0xdd ) << karma::big_dword )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ] << karma::repeat( karma::_a )[ value_ ];
          named_value = value_ << value_;
          map = karma::byte_[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0x0F,
            karma::_1 = 0x80|boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ]|
          ( karma::byte_( 0xde ) << karma::big_word )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFF,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ]|
          ( karma::byte_( 0xdf ) << karma::big_dword )[
            karma::_pass = boost::phoenix::size( karma::_val ) <= 0xFFFFFFFFul,
            karma::_1 = boost::phoenix::size( karma::_val ),
            karma::_a = boost::phoenix::size( karma::_val )
          ] << karma::repeat( karma::_a )[ named_value ];
          value_ = uint8_|uint16_|uint32_|uint64_|int8_|int16_|int32_|int64_|null_|bool_|float_|double_|raw|array|map;
  */      }
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
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > raw_short;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > raw_16;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >(), boost::spirit::karma::locals< uint32_t > > raw_32;
        boost::spirit::karma::rule< Iterator, std::vector< uint8_t >() > raw;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::messagepack >(), boost::spirit::karma::locals< uint32_t > > array;
        boost::spirit::karma::rule< Iterator, std::pair< hermit::messagepack, hermit::messagepack >() > named_value;
        boost::spirit::karma::rule< Iterator, std::map< hermit::messagepack, hermit::messagepack >(), boost::spirit::karma::locals< uint32_t > > map;
        boost::spirit::karma::rule< Iterator, hermit::messagepack() > value_;
      };
    }
  }
}

#endif

