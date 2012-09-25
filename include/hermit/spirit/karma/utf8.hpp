#ifndef HERMIT_SPIRIT_KARMA_UTF8_HPP
#define HERMIT_SPIRIT_KARMA_UTF8_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename OutputIterator >
        class utf8 :
          public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
            public:
              utf8() : utf8::base_type( root ) {
                namespace karma = boost::spirit::karma;
                ascii_ = karma::byte_[ karma::_1 = karma::_val ];
                block1 = ( karma::byte_ << karma::byte_ )[
                  karma::_1 = 0xC0|( karma::_val >> 6 ),
                  karma::_2 = 0x80|( karma::_val & 0x3F )
                    ];
                block2 = ( karma::byte_ << karma::byte_ << karma::byte_ )[
                  karma::_1 = 0xE0|( karma::_val >> 12 ),
                  karma::_2 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
                  karma::_3 = 0x80|( karma::_val & 0x3F )
                    ];
                block3 = ( karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ )[
                  karma::_1 = 0xF0|( karma::_val >> 18 ),
                  karma::_2 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
                  karma::_3 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
                  karma::_4 = 0x80|( ( karma::_val ) & 0x3F )
                    ];
                block4 = ( karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ )[
                  karma::_1 = 0xF8|( karma::_val >> 24 ),
                  karma::_2 = 0x80|( ( karma::_val >> 18 ) & 0x3F ),
                  karma::_3 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
                  karma::_4 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
                  karma::_5 = 0x80|( ( karma::_val ) & 0x3F )
                    ];
                block5 = ( karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ << karma::byte_ )[
                  karma::_1 = 0xFC|( karma::_val >> 30 ),
                  karma::_2 = 0x80|( ( karma::_val >> 24 ) & 0x3F ),
                  karma::_3 = 0x80|( ( karma::_val >> 18 ) & 0x3F ),
                  karma::_4 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
                  karma::_5 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
                  karma::_6 = 0x80|( ( karma::_val ) & 0x3F )
                    ];
                root = ascii_[
                  karma::_pass = karma::_val <= 0x7Ful,
                  karma::_1 = karma::_val
                    ]|
                    block1[
                    karma::_pass = karma::_val <= 0x7FFul,
                  karma::_1 = karma::_val
                    ]|
                    block2[
                    karma::_pass = karma::_val <= 0xFFFFul,
                  karma::_1 = karma::_val
                    ]|
                    block3[
                    karma::_pass = karma::_val <= 0x1FFFFFul,
                  karma::_1 = karma::_val
                    ]|
                    block4[
                    karma::_pass = karma::_val <= 0x3FFFFFFul,
                  karma::_1 = karma::_val
                    ]|
                    block5[
                    karma::_pass = karma::_val <= 0x7FFFFFFFul,
                  karma::_1 = karma::_val
                    ];
              }
            private:
              boost::spirit::karma::rule< OutputIterator, uint32_t() > ascii_;
              boost::spirit::karma::rule< OutputIterator, uint32_t() > block1;
              boost::spirit::karma::rule< OutputIterator, uint32_t() > block2;
              boost::spirit::karma::rule< OutputIterator, uint32_t() > block3;
              boost::spirit::karma::rule< OutputIterator, uint32_t() > block4;
              boost::spirit::karma::rule< OutputIterator, uint32_t() > block5;
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
          };
      template< typename OutputIterator >
        class utf8string :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf8string() : utf8string::base_type( root ) {
                root = *utf8char;
              }
            private:
              utf8< OutputIterator > utf8char;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
    }
  }
}

#endif

