#ifndef HERMIT_SPIRIT_QI_UTF8
#define HERMIT_SPIRIT_QI_UTF8

#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class utf8 :
          public boost::spirit::qi::grammar< InputIterator, char32_t() > {
            public:
              utf8() : utf8::base_type( root ) {
                namespace qi = boost::spirit::qi;
                continous = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xC0 ) == 0x80,
                  qi::_val = qi::_1 & 0x3F
                    ];
                block1_head = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xE0 ) == 0xC0,
                  qi::_val = qi::_1 & 0x1F
                    ];
                block2_head = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xF0 ) == 0xE0,
                  qi::_val = qi::_1 & 0x0F
                    ];
                block3_head = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xF8 ) == 0xF0,
                  qi::_val = qi::_1 & 0x07
                    ];
                block4_head = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xFC ) == 0xF8,
                  qi::_val = qi::_1 & 0x03
                    ];
                block5_head = qi::byte_[
                  qi::_pass = ( qi::_1 & 0xFE ) == 0xFC,
                  qi::_val = qi::_1 & 0x01
                    ];
                root =
                  qi::byte_[ qi::_pass = qi::_1 < 0x80, qi::_val = qi::_1 ]|
                  ( block1_head >> continous )[
                  qi::_val = boost::phoenix::static_cast_<char32_t>( qi::_1 ) << 6|
                  boost::phoenix::static_cast_<char32_t>( qi::_2 )
                  ]|
                  ( block2_head >> continous >> continous )[
                  qi::_val = boost::phoenix::static_cast_<char32_t>( qi::_1 ) << 12|
                  boost::phoenix::static_cast_<char32_t>( qi::_2 ) << 6|
                  boost::phoenix::static_cast_<char32_t>( qi::_3 )
                  ]|
                  ( block3_head >> continous >> continous >> continous )[
                  qi::_val = boost::phoenix::static_cast_<char32_t>( qi::_1 ) << 18|
                  boost::phoenix::static_cast_<char32_t>( qi::_2 ) << 12|
                  boost::phoenix::static_cast_<char32_t>( qi::_3 ) << 6|
                  boost::phoenix::static_cast_<char32_t>( qi::_4 )
                  ]|
                  ( block4_head >> continous >> continous >> continous >> continous )[
                  qi::_val = boost::phoenix::static_cast_<char32_t>( qi::_1 ) << 24|
                  boost::phoenix::static_cast_<char32_t>( qi::_2 ) << 18|
                  boost::phoenix::static_cast_<char32_t>( qi::_3 ) << 12|
                  boost::phoenix::static_cast_<char32_t>( qi::_4 ) << 6|
                  boost::phoenix::static_cast_<char32_t>( qi::_5 )
                  ]|
                  ( block5_head >> continous >> continous >> continous >> continous >> continous )[
                  qi::_val = boost::phoenix::static_cast_<char32_t>( qi::_1 ) << 30|
                  boost::phoenix::static_cast_<char32_t>( qi::_2 ) << 24|
                  boost::phoenix::static_cast_<char32_t>( qi::_3 ) << 18|
                  boost::phoenix::static_cast_<char32_t>( qi::_4 ) << 12|
                  boost::phoenix::static_cast_<char32_t>( qi::_5 ) << 6|
                  boost::phoenix::static_cast_<char32_t>( qi::_6 )
                  ];
              }
            private:
              boost::spirit::qi::rule< InputIterator, uint8_t() > continous;
              boost::spirit::qi::rule< InputIterator, uint8_t() > block1_head;
              boost::spirit::qi::rule< InputIterator, uint8_t() > block2_head;
              boost::spirit::qi::rule< InputIterator, uint8_t() > block3_head;
              boost::spirit::qi::rule< InputIterator, uint8_t() > block4_head;
              boost::spirit::qi::rule< InputIterator, uint8_t() > block5_head;
              boost::spirit::qi::rule< InputIterator, char32_t() > root;
      };
    }
  }
}

#endif

