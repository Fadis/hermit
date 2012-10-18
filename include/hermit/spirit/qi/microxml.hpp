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
#include <hermit/spirit/qi/utf8.hpp>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      void foo() { std::cout << "foo" << std::endl; }
      template< typename Iterator >
      class microxml :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::microxml()
        > {
      public:
        microxml() : microxml::base_type( root ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          root = -comment >> element >> -comment;
          element = nonEmptyElementTag|emptyElementTag;
          nonEmptyElementTag = ( '<' >> name >> attributeList >>
            qi::omit[ *space_ ] >> '>' >> contents >> 
            "</" >> name >> qi::omit[ *space_ ] >> '>' )[
              qi::_pass = qi::_1 == qi::_4,
              phx::at_c< 0 >( qi::_val ) = qi::_1,
              phx::at_c< 1 >( qi::_val ) = qi::_2,
              phx::at_c< 2 >( qi::_val ) = qi::_3
            ];
          contents = *content;
          content = -comment >> ( element|( +( dataChar|charRef ) ) ) >> -comment;
          emptyElementTag = '<' >> qi::omit[ *space_ ] >>
            name >> attributeList >> qi::omit[ *space_ ] >> "/>";
          dataChar = char_ - qi::standard::char_( "<>&" );
          attributeList = *( qi::omit[ +space_ ] >> attribute );
          attribute = attributeName >> qi::omit[ *space_ ] 
            >> '=' >> qi::omit[ *space_ ] >>
            attributeValue;
          attributeValue = (
            '"' >>
            *( ( attributeValueChar - '"' )|charRef ) >>
            '"'
          )
          |(
            '\'' >>
            *( ( attributeValueChar - '\'' )|charRef ) >>
            '\''
          );
          attributeValueChar = char_ - qi::standard::char_( "<>&" );
          attributeName = name - "xmlns";
          comment = qi::omit[ "<!--" >> *( char_ - "--" ) >> "-->" ];
          charRef = numericCharRef | namedCharRef;
          numericCharRef = "&#x" >>
            qi::hex >> ';';
          namedCharRef = '&' >> charName >> ';';
          charName.add( "amp", '&' )( "lt", '<' )( "gt", '>' )( "quot", '"' )( "apos", '\'' );
          name = nameStartChar >> *nameChar;
          nameStartChar = utf8_[
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
          utf8_[
            qi::_pass = ( qi::_1 >= 0xF900ul && qi::_1 <= 0xEFFFFul ),
            qi::_val = qi::_1
          ];
          nameChar = nameStartChar|nameNonStartChar;
          nameNonStartChar = utf8_[
            qi::_pass = ( qi::_1 >= 0x30ul && qi::_1 <= 0x39ul ) ||
                        qi::_1 == 0x2Dul ||
                        qi::_1 == 0x2Eul ||
                        qi::_1 == 0xB7ul ||
                        ( qi::_1 >= 0x300ul && qi::_1 <= 0x36Ful ) ||
                        ( qi::_1 >= 0x203Ful && qi::_1 <= 0x2040ul ),
            qi::_val = qi::_1
          ];
          char_ = space_|non_space;
          non_space = ( utf8_ - utf8control_ )[
            qi::_pass = qi::_1 != 0x20ul,
            qi::_val = qi::_1
          ];
          space_ = utf8_[
            qi::_pass = qi::_1 == 0x9ul || qi::_1 == 0xAul || qi::_1 == 0x20ul,
            qi::_val = qi::_1
          ];
        } 
      private:
        utf8< Iterator > utf8_;
        utf8control< Iterator > utf8control_;
        boost::spirit::qi::rule< Iterator, hermit::microxml() > root;
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
        boost::spirit::qi::rule< Iterator, char32_t() > namedCharRef;
        boost::spirit::qi::symbols< char, char32_t > charName;
        boost::spirit::qi::rule< Iterator, std::u32string() > name;
        boost::spirit::qi::rule< Iterator, char32_t() > nameStartChar;
        boost::spirit::qi::rule< Iterator, char32_t() > nameNonStartChar;
        boost::spirit::qi::rule< Iterator, char32_t() > nameChar;
        boost::spirit::qi::rule< Iterator, char32_t() > char_;
        boost::spirit::qi::rule< Iterator, char32_t() > space_;
        boost::spirit::qi::rule< Iterator, char32_t() > non_space;
      };
    }
  }
}

#endif

