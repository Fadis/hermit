#ifndef HERMIT_SPIRIT_QI_UTF32
#define HERMIT_SPIRIT_QI_UTF32

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class utf32be :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf32be() : utf32be::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = qi::big_dword;
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
      template <typename InputIterator>
        class utf32le :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf32le() : utf32le::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = qi::little_dword;
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
      template <typename InputIterator>
        class utf32bestring :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf32bestring() : utf32bestring::base_type( root ) {
                root = *big_endian;
              }
            private:
              utf32be< InputIterator > big_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
      template <typename InputIterator>
        class utf32lestring :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf32lestring() : utf32lestring::base_type( root ) {
                root = *little_endian;
              }
            private:
              utf32le< InputIterator > little_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
      template <typename InputIterator>
        class utf32string :
          public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
            public:
              utf32string() : utf32string::base_type( root ) {
                namespace qi = boost::spirit::qi;
                root = ( qi::big_dword( 0xfeff ) >> big_endian )|( qi::little_dword( 0xfeff ) >> little_endian )|big_endian;
              }
            private:
              utf32bestring< InputIterator > big_endian;
              utf32lestring< InputIterator > little_endian;
              boost::spirit::qi::rule< InputIterator, std::u32string() > root;
      };
    }
  }
}

#endif

