#ifndef HERMIT_MESSAGEPACK_HPP
#define HERMIT_MESSAGEPACK_HPP

#include <utility>
#include <cstddef>
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/range_traits.hpp>

namespace hermit {
  namespace messagepack {
    typedef std::string string;
    
    typedef boost::make_recursive_variant<
      uint8_t,
      uint16_t,
      uint32_t,
      uint64_t,
      int8_t,
      int16_t,
      int32_t,
      int64_t,
      float,
      double,
      std::nullptr_t,
      bool,
      std::vector< uint8_t >,
      std::vector< boost::recursive_variant_ >,
      std::map< boost::recursive_variant_, boost::recursive_variant_ >
    >::type value;

    template< typename Iterator >
    class rule :
      public boost::spirit::qi::grammar<
        Iterator,
        value()
      > {
    public:
      rule() : rule::base_type( value_ ) {
        using namespace boost::spirit;
        uint8_ = byte_[ qi::_pass = qi::_1 < 0x80, qi::_val = qi::_1 ] | ( byte_( 0xcc ) >> byte_ );
        uint16_ = byte_( 0xcd ) >> big_word;
        uint32_ = byte_( 0xce ) >> big_dword;
        uint64_ = byte_( 0xcf ) >> big_qword;
        int8_ =
          byte_[ qi::_pass = ( qi::_1 & 0xE0 ) == 0xE0, qi::_val = -boost::phoenix::static_cast_<int8_t>( 0xFF - qi::_1 )-1 ] |
          ( byte_( 0xd0 ) >> byte_[ boost::phoenix::if_( qi::_1 < 0x80 )[
            qi::_val = qi::_1
          ].else_[
            qi::_val = -boost::phoenix::static_cast_<int8_t>( 0xFF - qi::_1 )-1
          ]
        ] );
        int16_ =
          byte_( 0xd1 ) >> byte_[ boost::phoenix::if_( qi::_1 < 0x8000 )[
            qi::_val = qi::_1
          ].else_[
            qi::_val = -boost::phoenix::static_cast_<int16_t>( 0xFFFF - qi::_1 )-1
          ]
        ];
        int32_ =
          byte_( 0xd2 ) >> byte_[ boost::phoenix::if_( qi::_1 < 0x80000000ul )[
            qi::_val = qi::_1
          ].else_[
            qi::_val = -boost::phoenix::static_cast_<int32_t>( 0xFFFFFFFFul - qi::_1 )-1
          ]
        ];
        int64_ =
          byte_( 0xd3 ) >> byte_[ boost::phoenix::if_( qi::_1 < 0x8000000000000000ul )[
            qi::_val = qi::_1
          ].else_[
            qi::_val = -boost::phoenix::static_cast_<int64_t>( 0xFFFFFFFFFFFFFFFFul - qi::_1 )-1
          ]
        ];
        null_ = byte_( 0xc0 )[ _val = nullptr ];
        bool_ = byte_( 0xc3 )[ qi::_val = true ]|byte_( 0xc2 )[ qi::_val = false ];
        float_ = byte_( 0xca ) >> big_bin_float;
        double_ = byte_( 0xcb ) >> big_bin_double;
        raw = omit[
          byte_[ qi::_pass = ( qi::_1 & 0xE0 ) == 0xC0, qi::_a = qi::_1 & 0x1F ]|
          ( byte_( 0xda ) >> big_word[ qi::_a = qi::_1 ] )|
          ( byte_( 0xdb ) >> big_dword[ qi::_a = qi::_1 ] )
        ] >> qi::repeat(qi::_a)[ byte_ ];
        array = omit[
          byte_[ qi::_pass = ( qi::_1 & 0xF0 ) == 0x90, qi::_a = qi::_1 & 0x0F ]|
          ( byte_( 0xdc ) >> big_word[ qi::_a = qi::_1 ] )|
          ( byte_( 0xdd ) >> big_dword[ qi::_a = qi::_1 ] )
        ] >> qi::repeat(qi::_a)[ value_ ];
        named_value = value_ >> value_;
        map = omit[
          byte_[ qi::_pass = ( qi::_1 & 0xF0 ) == 0x80, qi::_a = qi::_1 & 0x0F ]|
          ( byte_( 0xde ) >> big_word[ qi::_a = qi::_1 ] )|
          ( byte_( 0xdf ) >> big_dword[ qi::_a = qi::_1 ] )
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
      boost::spirit::qi::rule< Iterator, std::vector< value >(), boost::spirit::qi::locals< uint32_t > > array;
      boost::spirit::qi::rule< Iterator, std::pair< value, value >() > named_value;
      boost::spirit::qi::rule< Iterator, std::map< value, value >(), boost::spirit::qi::locals< uint32_t > > map;
      boost::spirit::qi::rule< Iterator, value() > value_;

    };

  }
}

#endif

