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
                root = surrogate_pairs|qi::big_word;
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > surrogate_pairs;
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
                root = surrogate_pairs|qi::little_word;
              }
            private:
              boost::spirit::qi::rule< InputIterator, char32_t() > surrogate_pairs;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
    }
  }
}

#endif

