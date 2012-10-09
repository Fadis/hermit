#ifndef HERMIT_SPIRIT_QI_UTF16
#define HERMIT_SPIRIT_QI_UTF16

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class utf16be :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf16be() : utf16be::base_type( root ) {
                namespace qi = boost::spirit::qi;
                surrogate_pairs = ( qi::big_word >> qi::big_word )[
                  qi::_pass = ( ( qi::_1 & 0xFC00 ) == 0xD800 ) && ( ( qi::_2 & 0xFC00 ) == 0xDC00 ),
                  qi::_val =
                    ( ( boost::phoenix::static_cast_< uint32_t >( qi::_1 & 0x03FF ) << 10 )|
                        boost::phoenix::static_cast_< uint32_t >( qi::_2 & 0x03FF ) ) + 0x10000
                ];
                char_raw = surrogate_pairs|qi::big_word;
                root = char_raw[
                  qi::_pass = ( qi::_1 < 0xD800ul || qi::_1 > 0xDFFFul ) &&
                    ( qi::_1 < 0xFDD0ul || qi::_1 > 0xFDEFul ) &&
                    ( ( qi::_1 & 0xFFFFul ) < 0xFFFEul ),
                  qi::_val = qi::_1
                ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > surrogate_pairs;
              boost::spirit::qi::rule< InputIterator, char32_t() > char_raw;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
      template <typename InputIterator>
        class utf16le :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf16le() : utf16le::base_type( root ) {
                namespace qi = boost::spirit::qi;
                surrogate_pairs = ( qi::little_word >> qi::little_word )[
                  qi::_pass = ( ( qi::_1 & 0xFC00 ) == 0xD800 ) && ( ( qi::_2 & 0xFC00 ) == 0xDC00 ),
                  qi::_val =
                    ( ( boost::phoenix::static_cast_< uint32_t >( qi::_1 & 0x03FF ) << 10 )|
                        boost::phoenix::static_cast_< uint32_t >( qi::_2 & 0x03FF ) ) + 0x10000
                ];
                char_raw = surrogate_pairs|qi::little_word;
                root = char_raw[
                  qi::_pass = ( qi::_1 < 0xD800ul || qi::_1 > 0xDFFFul ) &&
                    ( qi::_1 < 0xFDD0ul || qi::_1 > 0xFDEFul ) &&
                    ( ( qi::_1 & 0xFFFFul ) < 0xFFFEul ),
                  qi::_val = qi::_1
                ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > surrogate_pairs;
              boost::spirit::qi::rule< InputIterator, char32_t() > char_raw;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };


      template <typename InputIterator>
        class utf16becontrol :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf16becontrol() : utf16becontrol::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = utf16be_char[
                  qi::_pass = qi::_1 <= 0x1Ful ||
                       ( qi::_1 >= 0x7Ful && qi::_1 <= 0x9Ful ),
                  qi::_val = qi::_1
                ];
              }
            private:
              utf16be< InputIterator > utf16be_char;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
      
      template <typename InputIterator>
        class utf16lecontrol :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf16lecontrol() : utf16lecontrol::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = utf16le_char[
                  qi::_pass = qi::_1 <= 0x1Ful ||
                       ( qi::_1 >= 0x7Ful && qi::_1 <= 0x9Ful ),
                  qi::_val = qi::_1
                ];
              }
            private:
              utf16le< InputIterator > utf16le_char;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
      
      template <typename InputIterator>
        class utf16bestring :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf16bestring() : utf16bestring::base_type( root ) {
                root = *big_endian;
              }
            private:
              utf16be< InputIterator > big_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
      template <typename InputIterator>
        class utf16lestring :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf16lestring() : utf16lestring::base_type( root ) {
                root = *little_endian;
              }
            private:
              utf16le< InputIterator > little_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
      template <typename InputIterator>
        class utf16string :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf16string() : utf16string::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = ( qi::big_word( 0xfeff ) >> big_endian )|( qi::little_word( 0xfeff ) >> little_endian )|big_endian;
              }
            private:
              utf16bestring< InputIterator > big_endian;
              utf16lestring< InputIterator > little_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
    }
  }
}

#endif

