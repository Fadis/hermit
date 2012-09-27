#ifndef HERMIT_SPIRIT_QI_MICROXML_HPP
#define HERMIT_SPIRIT_QI_MICROXML_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/microxml.hpp>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template< typename Iterator >
      class microxml :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::microxml()
        > {
      public:
        microxml() : microxml::base_type( element ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          element = nonEmptyElementTag|emptyElementTag;
          nonEmptyElementTag = ( qi::big_dword( 0x3Cul ) >> name >> attributeList >>
            qi::omit[ *space_ ] >> qi::big_dword( 0x3Eul ) >> contents >> 
            qi::big_dword( 0x3Cul ) >> qi::big_dword( 0x2Ful ) >>
            name >> qi::omit[ *space_ ] >> qi::big_dword( 0x3Eul ) )[
              qi::_pass = qi::_1 == qi::_4,
              phx::at_c< 0 >( qi::_val ) = qi::_1,
              phx::at_c< 1 >( qi::_val ) = qi::_2,
              phx::at_c< 2 >( qi::_val ) = qi::_3
            ];
          contents = *content;
          content = ( element|comment|( +( dataChar|charRef ) ) );
          emptyElementTag = qi::big_dword( 0x3Cul ) >> qi::omit[ *space_ ] >>
            name >> attributeList >> qi::big_dword( 0x2Ful ) >>
            qi::big_dword( 0x3Eul );
          dataChar = char_ - (
            qi::big_dword( 0x3Cul )|qi::big_dword( 0x3Eul )|
            qi::big_dword( 0x26ul )
          );
          attributeList = *( qi::omit[ +space_ ] >> attribute );
          attribute = attributeName >> qi::omit[ *space_ ]
            >> qi::big_dword( 0x3Dul ) >> qi::omit[ *space_ ] >>
            attributeValue;
          attributeValue = (
            qi::big_dword( 0x22ul ) >>
            *( ( attributeValueChar - qi::big_dword( 0x22ul ) )|charRef ) >>
            qi::big_dword( 0x22ul )
          )
          |(
            qi::big_dword( 0x27ul ) >>
            *( ( attributeValueChar - qi::big_dword( 0x27ul ) )|charRef ) >>
            qi::big_dword( 0x27ul )
          );
          attributeValueChar = char_ - (
            qi::big_dword( 0x3Cul )|qi::big_dword( 0x3Eul )|
            qi::big_dword( 0x26ul )
          );
          attributeName = (
            qi::big_dword( 0x78ul ) >> qi::big_dword( 0x6Cul ) >> qi::big_dword( 0x6Dul ) >> qi::big_dword( 0x6Eul ) >> qi::big_dword( 0x73ul )
          );
          comment = qi::omit[ (
            qi::big_dword( 0x3Cul ) >> qi::big_dword( 0x21ul ) >>
            qi::big_dword( 0x2Dul ) >> qi::big_dword( 0x2Dul )
          ) >>
          *(
            char_ - ( qi::big_dword( 0x2Dul ) >> qi::big_dword( 0x2Dul ) )
          ) >>
          (
            qi::big_dword( 0x2Dul ) >> qi::big_dword( 0x2Dul ) >>
            qi::big_dword( 0x3Eul )
          ) ];
          charRef = numericCharRef | namedCharRef;
          numericCharRef = qi::big_dword( 0x26ul ) >>
            qi::big_dword( 0x23ul ) >> qi::big_dword( 0x78ul ) >>
            charNumber >> qi::big_dword( 0x3Bul );
          charNumber = +( qi::big_dword[
            phx::if_( qi::_1 >= 0x30ul && qi::_1 <= 0x39ul )[
              qi::_pass = true,
              qi::_val <<= 4,
              qi::_val |= qi::_1 - 0x30ul
            ].else_[
              phx::if_( qi::_1 >= 0x41ul && qi::_1 <= 0x46ul )[
                qi::_pass = true,
                qi::_val <<= 4,
                qi::_val |= qi::_1 - 0x41ul + 0xAul
              ].else_[
                phx::if_( qi::_1 >= 0x61ul && qi::_1 <= 0x66ul )[
                  qi::_pass = true,
                  qi::_val <<= 4,
                  qi::_val |= qi::_1 - 0x61ul + 0xAul
                ].else_[
                  qi::_pass = false
                ]
              ]
            ]
          ] );
          namedCharRef = qi::big_dword( 0x26ul ) >> charName >> qi::big_dword( 0x3Bul );
          charName = ( qi::big_dword( 0x61ul ) >> qi::big_dword( 0x6dul ) >> qi::big_dword( 0x70ul ) )[
            qi::_val = 0x26ul
          ]|
          ( qi::big_dword( 0x6Cul ) >> qi::big_dword( 0x74ul ) )[
            qi::_val = 0x3Cul
          ]|
          ( qi::big_dword( 0x67ul ) >> qi::big_dword( 0x74ul ) )[
            qi::_val = 0x3Eul
          ]|
          ( qi::big_dword( 0x71ul ) >> qi::big_dword( 0x75ul ) >> qi::big_dword( 0x6Ful ) >> qi::big_dword( 0x74ul ) )[
            qi::_val = 0x22ul
          ]|
          ( qi::big_dword( 0x61ul ) >> qi::big_dword( 0x70ul ) >> qi::big_dword( 0x6Ful ) >> qi::big_dword( 0x74ul ) )[
            qi::_val = 0x27ul
          ];
          name = nameStartChar >> *nameChar;
          nameStartChar = qi::big_dword[
            qi::_pass = ( qi::_1 >= 0x41ul && qi::_1 <= 0x5Aul ) ||
                        qi::_1 == 0x5Ful ||
                        ( qi::_1 >= 0x61ul && qi::_1 <= 0x7Aul ) ||
                        ( qi::_1 >= 0xC0ul && qi::_1 <= 0xD6ul ) ||
                        ( qi::_1 >= 0xD8ul && qi::_1 <= 0xF6ul ) ||
                        ( qi::_1 >= 0xF8ul && qi::_1 <= 0x2FFul ) ||
                        ( qi::_1 >= 0x370ul && qi::_1 <= 0x37Dul ) ||
                        ( qi::_1 >= 0x37Ful && qi::_1 <= 0x1FFFul ) ||
                        ( qi::_1 >= 0x200Cul && qi::_1 <= 0x200Dul ) ||
                        ( qi::_1 >= 0x2070ul && qi::_1 <= 0x218Ful ) ||
                        ( qi::_1 >= 0x2C00ul && qi::_1 <= 0x2FEFul ) ||
                        ( qi::_1 >= 0x3001ul && qi::_1 <= 0xD7FFul ),
            qi::_val = qi::_1
          ]|
          ( qi::big_dword - nonCharacterCodePoint )[
            qi::_pass = ( qi::_1 >= 0xF900ul && qi::_1 <= 0xEFFFFul ),
            qi::_val = qi::_1
          ];
          nameChar = nameStartChar|qi::big_dword[
            qi::_pass = ( qi::_1 >= 0x30ul && qi::_1 <= 0x39ul ) ||
                        qi::_1 == 0x2Dul ||
                        qi::_1 == 0x2Eul ||
                        qi::_1 == 0xB7ul ||
                        ( qi::_1 >= 0x300ul && qi::_1 <= 0x36Ful ) ||
                        ( qi::_1 >= 0x203Ful && qi::_1 <= 0x2040ul ),
            qi::_val = qi::_1
          ];
          char_ = space_|( qi::big_dword[
            qi::_pass = qi::_1 <= 0x10FFFFul,
            qi::_val = qi::_1
          ] - forbiddenCodePoint );
          space_ = qi::big_dword[
            qi::_pass = qi::_1 == 0x9ul || qi::_1 == 0xAul || qi::_1 == 0x20ul,
            qi::_val = qi::_1
          ];
          forbiddenCodePoint = controlCodePoint | surrogateCodePoint | nonCharacterCodePoint;
          controlCodePoint = qi::big_dword[
            qi::_pass = qi::_1 <= 0x1Ful ||
                        ( qi::_1 >= 0x7Ful && qi::_1 <= 0x9Ful ),
            qi::_val = qi::_1
          ];
          surrogateCodePoint = qi::big_dword[
            qi::_pass = qi::_1 >= 0xD800ul && qi::_1 <= 0xDFFFul,
            qi::_val = qi::_1
          ];
          nonCharacterCodePoint = qi::big_dword[
            qi::_pass = ( qi::_1 >= 0xFDD0ul && qi::_1 <= 0xFDEFul ) ||
                        ( ( qi::_1 & 0xFFFFul ) >= 0xFFFEul &&
                          ( qi::_1 & 0xFFFFul ) <= 0xFFFFul ),
            qi::_val = qi::_1
          ];
        } 
      private:
        boost::spirit::qi::rule< Iterator, hermit::microxml() > element;
        boost::spirit::qi::rule< Iterator, hermit::microxml() > nonEmptyElementTag;
        boost::spirit::qi::rule< Iterator, std::vector< hermit::microxml::child_t >() > contents;
        boost::spirit::qi::rule< Iterator, hermit::microxml::child_t() > content;
        boost::spirit::qi::rule< Iterator, char32_t() > dataChar;
        boost::spirit::qi::rule< Iterator, hermit::microxml() > emptyElementTag;
        boost::spirit::qi::rule< Iterator, hermit::microxml::attr_t() > attributeList;
        boost::spirit::qi::rule< Iterator, std::pair< std::u32string, std::u32string >() > attribute;
        boost::spirit::qi::rule< Iterator, std::u32string() > attributeValue;
        boost::spirit::qi::rule< Iterator, char32_t() > attributeValueChar;
        boost::spirit::qi::rule< Iterator, std::u32string() > attributeName;
        boost::spirit::qi::rule< Iterator, boost::spirit::unused_type() > comment;
        boost::spirit::qi::rule< Iterator, char32_t() > charRef;
        boost::spirit::qi::rule< Iterator, char32_t() > numericCharRef;
        boost::spirit::qi::rule< Iterator, char32_t() > charNumber;
        boost::spirit::qi::rule< Iterator, char32_t() > namedCharRef;
        boost::spirit::qi::rule< Iterator, char32_t() > charName;
        boost::spirit::qi::rule< Iterator, std::u32string() > name;
        boost::spirit::qi::rule< Iterator, char32_t() > nameStartChar;
        boost::spirit::qi::rule< Iterator, char32_t() > nameChar;
        boost::spirit::qi::rule< Iterator, char32_t() > char_;
        boost::spirit::qi::rule< Iterator, char32_t() > space_;
        boost::spirit::qi::rule< Iterator, char32_t() > forbiddenCodePoint;
        boost::spirit::qi::rule< Iterator, char32_t() > controlCodePoint;
        boost::spirit::qi::rule< Iterator, char32_t() > surrogateCodePoint;
        boost::spirit::qi::rule< Iterator, char32_t() > nonCharacterCodePoint;
      };
    }
  }
}

#endif

