#ifndef HERMIT_SPIRIT_KARMA_UTF32_HPP
#define HERMIT_SPIRIT_KARMA_UTF32_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename OutputIterator >
        class utf32be :
          public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
            public:
              utf32be() : utf32be::base_type( root ) {
                namespace karma = boost::spirit::karma;
                root = karma::big_dword[ karma::_pass = karma::_val <= 0x10FFFFul, karma::_1 = karma::_val ];
              }
            private:
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
      };
      template< typename OutputIterator >
        class utf32le :
          public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
            public:
              utf32le() : utf32le::base_type( root ) {
                namespace karma = boost::spirit::karma;
                root = karma::little_dword[ karma::_pass = karma::_val <= 0x10FFFFul, karma::_1 = karma::_val ];
              }
            private:
              boost::spirit::karma::rule< OutputIterator, char32_t() > root;
      };
      template< typename OutputIterator >
        class utf32bestring :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf32bestring() : utf32bestring::base_type( root ) {
                root = *big_endian;
              }
            private:
              utf32be< OutputIterator > big_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
      template< typename OutputIterator >
        class utf32lestring :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf32lestring() : utf32lestring::base_type( root ) {
                root = *little_endian;
              }
            private:
              utf32le< OutputIterator > little_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
      template< typename OutputIterator >
        class utf32string :
          public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
            public:
              utf32string() : utf32string::base_type( root ) {
                namespace karma = boost::spirit::karma;
                root = karma::big_dword( 0xfeff ) << big_endian;
              }
            private:
              utf32be< OutputIterator > big_endian;
              boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
      };
    }
  }
}

#endif

