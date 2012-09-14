#ifndef HERMIT_UTF8_HPP
#define HERMIT_UTF8_HPP

#include <string>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace hermit {
  namespace grammar {
    template <typename InputIterator>
    class  :
      public boost::spirit::qi::grammar< InputIterator, char32_t() > {
      public:
      parser_rule() : paeser_rule::base_type( root ) {
        using namespace boost::spirit;
        continous = byte_[
          qi::_pass = ( qi::_1 & 0xC0 ) == 0x80,
          qi::_val = qi::_1 & 0x3F
            ];
        block1_head = byte_[
          qi::_pass = ( qi::_1 & 0xE0 ) == 0xC0,
          qi::_val = qi::_1 & 0x1F
            ];
        block2_head = byte_[
          qi::_pass = ( qi::_1 & 0xF0 ) == 0xE0,
          qi::_val = qi::_1 & 0x0F
            ];
        block3_head = byte_[
          qi::_pass = ( qi::_1 & 0xF8 ) == 0xF0,
          qi::_val = qi::_1 & 0x07
            ];
        block4_head = byte_[
          qi::_pass = ( qi::_1 & 0xFC ) == 0xF8,
          qi::_val = qi::_1 & 0x03
            ];
        block5_head = byte_[
          qi::_pass = ( qi::_1 & 0xFE ) == 0xFC,
          qi::_val = qi::_1 & 0x01
            ];
        root =
          byte_[ qi::_pass = qi::_1 < 0x80, qi::_val = qi::_1 ]|
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


template <typename InputIterator>
class utf8string_to_u32string :
  public boost::spirit::qi::grammar< InputIterator, std::u32string() > {
    public:
      utf8string_to_u32string() : utf8string_to_u32string::base_type( root ) {
        using namespace boost::spirit;
        root = *char32_;
      }
    private:
      utf8_to_ucs4< InputIterator > char32_;
      boost::spirit::qi::rule< InputIterator, std::u32string() > root;
};
template< typename OutputIterator >
class ucs4_to_utf8 :
  public boost::spirit::karma::grammar< OutputIterator, char32_t() > {
    public:
      ucs4_to_utf8() : ucs4_to_utf8::base_type( root ) {
        using namespace boost::spirit;
        ascii_ = byte_[ karma::_1 = karma::_val ];
        block1 = ( byte_ << byte_ )[
          karma::_1 = 0xC0|( karma::_val >> 6 ),
          karma::_2 = 0x80|( karma::_val & 0x3F )
        ];
        block2 = ( byte_ << byte_ << byte_ )[
          karma::_1 = 0xE0|( karma::_val >> 12 ),
          karma::_2 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
          karma::_3 = 0x80|( karma::_val & 0x3F )
        ];
        block3 = ( byte_ << byte_ << byte_ << byte_ )[
          karma::_1 = 0xF0|( karma::_val >> 18 ),
          karma::_2 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
          karma::_3 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
          karma::_4 = 0x80|( ( karma::_val ) & 0x3F )
        ];
        block4 = ( byte_ << byte_ << byte_ << byte_ << byte_ )[
          karma::_1 = 0xF8|( karma::_val >> 24 ),
          karma::_2 = 0x80|( ( karma::_val >> 18 ) & 0x3F ),
          karma::_3 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
          karma::_4 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
          karma::_5 = 0x80|( ( karma::_val ) & 0x3F )
        ];
        block5 = ( byte_ << byte_ << byte_ << byte_ << byte_ << byte_ )[
          karma::_1 = 0xFC|( karma::_val >> 30 ),
          karma::_2 = 0x80|( ( karma::_val >> 24 ) & 0x3F ),
          karma::_3 = 0x80|( ( karma::_val >> 18 ) & 0x3F ),
          karma::_4 = 0x80|( ( karma::_val >> 12 ) & 0x3F ),
          karma::_5 = 0x80|( ( karma::_val >> 6 ) & 0x3F ),
          karma::_6 = 0x80|( ( karma::_val ) & 0x3F )
        ];
        root = ascii_[
          karma::_pass = karma::_val <= 0x7F,
          karma::_1 = karma::_val
        ]|
        block1[
          karma::_pass = karma::_val <= 0x7FF,
          karma::_1 = karma::_val
        ]|
        block2[
          karma::_pass = karma::_val <= 0xFFFF,
          karma::_1 = karma::_val
        ]|
        block3[
          karma::_pass = karma::_val <= 0x1FFFFF,
          karma::_1 = karma::_val
        ]|
        block4[
          karma::_pass = karma::_val <= 0x3FFFFFF,
          karma::_1 = karma::_val
        ]|
        block5[
          karma::_pass = karma::_val <= 0x7FFFFFFF,
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
class u32string_to_utf8string :
  public boost::spirit::karma::grammar< OutputIterator, std::u32string() > {
    public:
      u32string_to_utf8string() : u32string_to_utf8string::base_type( root ) {
        using namespace boost::spirit;
        root = *char32_;
      }
    private:
      ucs4_to_utf8< OutputIterator > char32_;
      boost::spirit::karma::rule< OutputIterator, std::u32string() > root;
};

int main() {
  std::string source( "UTF8で表現された\n何か" );
  utf8string_to_u32string< std::string::iterator > rule;
  auto iter = source.begin();
  std::u32string result;
  boost::spirit::qi::parse( iter, source.end(), rule, result );
  for( auto &elem: result )
    std::cout << static_cast< uint32_t >( elem ) << ',';
  std::cout << std::endl;
  typedef std::back_insert_iterator<std::string> sink_type;
  u32string_to_utf8string< sink_type > rrule;
  std::string result2;
  boost::spirit::karma::generate( sink_type( result2 ), rrule, result );
  std::cout << result2 << std::endl;
}
