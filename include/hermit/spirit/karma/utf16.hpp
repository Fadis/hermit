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
                  karma::_pass = karma::_val > 0xFFFF && karma::_val <= 0x10FFFF,
                  karma::_1 = 0xD800|( ( ( karma::_val - 0x10000 ) >> 10 ) & 0x03FF ),
                  karma::_2 = 0xDC00|( karma::_val & 0x03FF )
                ];
                single_word = karma::big_word[
                  karma::_pass = karma::_val <= 0xFFFF,
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
                  karma::_pass = karma::_val > 0xFFFF && karma::_val <= 0x10FFFF,
                  karma::_1 = 0xD800|( ( ( karma::_val - 0x10000 ) >> 10 ) & 0x03FF ),
                  karma::_2 = 0xDC00|( karma::_val & 0x03FF )
                ];
                single_word = karma::little_word[
                  karma::_pass = karma::_val <= 0xFFFF,
                  karma::_1 = karma::_val
                ];
                root = surrogate_pairs|single_word;
              }
            private:
              boost::spirit::karma::rule< OutputIterator, char32_t() > surrogate_pairs;
              boost::spirit::karma::rule< OutputIterator, char32_t() > single_word;
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
      };
    }
  }
}

#endif

