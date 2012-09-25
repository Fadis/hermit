#ifndef HERMIT_SPIRIT_KARMA_UTF16_HPP
#define HERMIT_SPIRIT_KARMA_UTF16_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename OutputIterator >
        class utf16be :
          public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
            public:
              utf16be() : utf16be::base_type( root ) {
                namespace karma = boost::spirit::karma;
                surrogate_pairs = ( karma::big_word << karma::big_word )[
                  karma::_pass = karma::_val > 0xFFFFul && karma::_val <= 0x10FFFFul,
                  karma::_1 = 0xD800|( ( ( karma::_val - 0x10000 ) >> 10 ) & 0x03FF ),
                  karma::_2 = 0xDC00|( karma::_val & 0x03FF )
                ];
                single_word = karma::big_word[
                  karma::_pass = karma::_val <= 0xFFFFul,
                  karma::_1 = karma::_val
                ];
                root = surrogate_pairs|single_word;
              }
            private:
              boost::spirit::karma::rule< OutputIterator, char32_t() > surrogate_pairs;
              boost::spirit::karma::rule< OutputIterator, char32_t() > single_word;
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
      };
      template< typename OutputIterator >
        class utf16le :
          public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
            public:
              utf16le() : utf16le::base_type( root ) {
                namespace karma = boost::spirit::karma;
                surrogate_pairs = ( karma::little_word << karma::little_word )[
                  karma::_pass = karma::_val > 0xFFFFul && karma::_val <= 0x10FFFFul,
                  karma::_1 = 0xD800|( ( ( karma::_val - 0x10000ul ) >> 10 ) & 0x03FFul ),
                  karma::_2 = 0xDC00|( karma::_val & 0x03FFul )
                ];
                single_word = karma::little_word[
                  karma::_pass = karma::_val <= 0xFFFFul,
                  karma::_1 = karma::_val
                ];
                root = surrogate_pairs|single_word;
              }
            private:
              boost::spirit::karma::rule< OutputIterator, char32_t() > surrogate_pairs;
              boost::spirit::karma::rule< OutputIterator, char32_t() > single_word;
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
      };
      template< typename OutputIterator >
        class utf16bestring :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf16bestring() : utf16bestring::base_type( root ) {
                root = *big_endian;
              }
            private:
              utf16be< OutputIterator > big_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
      template< typename OutputIterator >
        class utf16lestring :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf16lestring() : utf16lestring::base_type( root ) {
                root = *little_endian;
              }
            private:
              utf16le< OutputIterator > little_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
      template< typename OutputIterator >
        class utf16string :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf16string() : utf16string::base_type( root ) {
                namespace karma = boost::spirit::karma;
                root = karma::big_word( 0xfeff ) << big_endian;
              }
            private:
              utf16be< OutputIterator > big_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
    }
  }
}

#endif

