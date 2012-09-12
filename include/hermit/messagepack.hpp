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
      double,
      float,
      uint64_t,
      uint32_t,
      uint64_t,
      uint8_t,
      int64_t,
      int32_t,
      int64_t,
      int8_t,
      bool,
      std::map< std::string, boost::recursive_variant_ >,
      std::vector< boost::recursive_variant_ >,
      std::nullptr_t
    >::type value;

    template< typename Iterator >
    class rule :
      public boost::spirit::qi::grammar<
        Iterator,
        value()
      > {
    public:
      rule() : rule::base_type( root ) {
        using namespace boost::spirit;
        null_ = byte_( 0xc0 )[ _val = nullptr ];
      //  root = object|array;
      //  value_ = string_|double_|object|array|bool_|null_;
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
        bool_ = byte_( 0xc3 )[ qi::_val = true ]|byte_( 0xc2 )[ qi::_val = false ];
        float_ = byte_( 0xca ) >> big_bin_float;
        double_ = byte_( 0xcb ) >> big_bin_double;
//        array = omit[ byte_[ qi::_pass = ( qi::_1 & 0xF0 ) == 0x90, qi::_val = 0 ] ] >> repeat(_a)[ value_ ];
          /*( byte_[ qi::_pass = ( qi::_1 & 0xE0 ) == 0xC0, qi::_a = qi::_1 & 0x1F ] >> qi::repeat(qi::_a)[ byte_ ] )|
          ( omit[ byte_( 0xda ) >> big_word[ qi::_a = qi::_1 ] ] >> repeat(qi::_a)[ byte_ ] )|
          ( omit[ byte_( 0xdb ) >> big_dword[ qi::_a = qi::_1 ] ] >> repeat(qi::_a)[ byte_ ] );*/
      //  string_ = '"' >> *( ( char_ - cntrl - '\\' - '"' )|escape_sequence ) >> '"';
      //  array = skip( blank )[ '[' >> -( value_ % ',' ) >>  ']' ];
      //  named_value = skip( blank )[ ( string_ >> ':' >> value_ ) ];
      //  object = skip( blank )[ '{' >> -( named_value % ',' ) >> '}' ];
      } 
    private:
      boost::spirit::qi::rule< Iterator, value() > root;
      boost::spirit::qi::rule< Iterator, char() > escape_sequence;
      boost::spirit::qi::rule< Iterator, string() > string_;
      boost::spirit::qi::rule< Iterator, std::nullptr_t() > null_;
      boost::spirit::qi::rule< Iterator, uint8_t() > uint8_;
      boost::spirit::qi::rule< Iterator, uint16_t() > uint16_;
      boost::spirit::qi::rule< Iterator, uint32_t() > uint32_;
      boost::spirit::qi::rule< Iterator, uint64_t() > uint64_;
      boost::spirit::qi::rule< Iterator, int8_t() > int8_;
      boost::spirit::qi::rule< Iterator, int16_t() > int16_;
      boost::spirit::qi::rule< Iterator, int32_t() > int32_;
      boost::spirit::qi::rule< Iterator, int64_t() > int64_;
      boost::spirit::qi::rule< Iterator, bool() > bool_;
      boost::spirit::qi::rule< Iterator, float() > float_;
      boost::spirit::qi::rule< Iterator, double() > double_;
      boost::spirit::qi::rule< Iterator, uint8_t() > bin_block_;
      boost::spirit::qi::rule< Iterator, value() > value_;
      boost::spirit::qi::rule< Iterator, std::vector< value >() > array;
      boost::spirit::qi::rule< Iterator, std::pair< string, value >() > named_value;
      boost::spirit::qi::rule< Iterator, std::map< string, value >() > object;

    };

  }
}

#endif

