#ifndef HERMIT_SPIRIT_KARMA_MICROXML_HPP
#define HERMIT_SPIRIT_KARMA_MICROXML_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/microxml.hpp>
#include <hermit/spirit/qi/utf8.hpp>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template< typename Iterator >
      class microxml :
        public boost::spirit::karma::grammar<
          Iterator,
          hermit::microxml()
        > {
      public:
        microxml() : microxml::base_type( root ) {
          namespace karma = boost::spirit::karma;
          namespace phx = boost::phoenix;
          root = element;
          element = nonEmptyElementTag|emptyElementTag;
          nonEmptyElementTag = ( '<' << name << attributeList << '>' << contents << "</" << name << '>' )[
            karma::_1 = phx::at_c< 0 >( karma::_val ),
            karma::_2 = phx::at_c< 1 >( karma::_val ),
            karma::_3 = phx::at_c< 2 >( karma::_val ),
            karma::_4 = phx::at_c< 0 >( karma::_val )
          ];
          contents = *content;
          content = element|( +( dataChar|charRef ) );
          emptyElementTag = '<' << name << attributeList << "/>";
          dataChar = char_[
            karma::_pass = karma::_val != U'<' && karma::_val != U'>' && karma::_val != U'&',
            karma::_1 = karma::_val
          ];
          attributeList = *( ' ' << attribute );
          attribute = attributeName <<  '=' << attributeValue;
          attributeValue = ( '"' << *( ( attributeValueChar )|charRef ) << '"' );
          attributeValueChar = char_[
            karma::_pass = karma::_val != U'<' && karma::_val != U'>' && karma::_val != U'&' && karma::_val != U'"',
            karma::_1 = karma::_val
          ];
          attributeName = name[
            karma::_pass = karma::_val != phx::construct< std::u32string >( U"xmlns" ),
            karma::_1 = karma::_val
          ];
          charRef = numericCharRef | namedCharRef;
          numericCharRef = "&#x" << karma::hex << ';';
          namedCharRef = '&' << charName << ';';
          charName.add( U'&', "amp" )( U'<', "lt" )( U'>', "gt" )( U'"', "quot" )( U'\'', "apos" );
          name = nameStartChar << *nameChar;
          nameStartChar = utf8_[
            karma::_pass = ( karma::_val >= 0x41ul && karma::_val <= 0x5Aul ) ||
                        karma::_val == 0x5Ful ||
                        ( karma::_val >= 0x61ul && karma::_val <= 0x7Aul ) ||
                        ( karma::_val >= 0xC0ul && karma::_val <= 0xD6ul ) ||
                        ( karma::_val >= 0xD8ul && karma::_val <= 0xF6ul ) ||
                        ( karma::_val >= 0xF8ul && karma::_val <= 0x2FFul ) ||
                        ( karma::_val >= 0x370ul && karma::_val <= 0x37Dul ) ||
                        ( karma::_val >= 0x37Ful && karma::_val <= 0x1FFFul ) ||
                        ( karma::_val >= 0x200Cul && karma::_val <= 0x200Dul ) ||
                        ( karma::_val >= 0x2070ul && karma::_val <= 0x218Ful ) ||
                        ( karma::_val >= 0x2C00ul && karma::_val <= 0x2FEFul ) ||
                        ( karma::_val >= 0x3001ul && karma::_val <= 0xD7FFul ),
            karma::_1 = karma::_val
          ]|
          utf8_[
            karma::_pass = ( karma::_val >= 0xF900ul && karma::_val <= 0xEFFFFul ),
            karma::_1 = karma::_val
          ];
          nameChar = nameStartChar|nameNonStartChar;
          nameNonStartChar = utf8_[
            karma::_pass = ( karma::_val >= 0x30ul && karma::_val <= 0x39ul ) ||
                        karma::_val == 0x2Dul ||
                        karma::_val == 0x2Eul ||
                        karma::_val == 0xB7ul ||
                        ( karma::_val >= 0x300ul && karma::_val <= 0x36Ful ) ||
                        ( karma::_val >= 0x203Ful && karma::_val <= 0x2040ul ),
            karma::_1 = karma::_val
          ];
          char_ = space_|non_space;
          non_space = utf8_[
            karma::_pass = karma::_val > 0x1Ful && ( karma::_val < 0x7Ful || karma::_val > 0x9Ful ),
            karma::_1 = karma::_val
          ];
          space_ = utf8_[
            karma::_pass = karma::_val == 0x9ul || karma::_val == 0xAul || karma::_val == 0x20ul,
            karma::_1 = karma::_val
          ];
        } 
      private:
        utf8< Iterator > utf8_;
        boost::spirit::karma::rule< Iterator, hermit::microxml() > root;
        boost::spirit::karma::rule< Iterator, hermit::microxml() > element;
        boost::spirit::karma::rule< Iterator, hermit::microxml() > nonEmptyElementTag;
        boost::spirit::karma::rule< Iterator, std::vector< hermit::microxml::child_t >() > contents;
        boost::spirit::karma::rule< Iterator, hermit::microxml::child_t() > content;
        boost::spirit::karma::rule< Iterator, char32_t() > dataChar;
        boost::spirit::karma::rule< Iterator, hermit::microxml() > emptyElementTag;
        boost::spirit::karma::rule< Iterator, hermit::microxml::attr_t() > attributeList;
        boost::spirit::karma::rule< Iterator, std::pair< std::u32string, std::u32string >() > attribute;
        boost::spirit::karma::rule< Iterator, std::u32string() > attributeValue;
        boost::spirit::karma::rule< Iterator, char32_t() > attributeValueChar;
        boost::spirit::karma::rule< Iterator, std::u32string() > attributeName;
        boost::spirit::karma::rule< Iterator, char32_t() > charRef;
        boost::spirit::karma::rule< Iterator, char32_t() > numericCharRef;
        boost::spirit::karma::rule< Iterator, char32_t() > namedCharRef;
        boost::spirit::karma::symbols< char32_t, std::string > charName;
        boost::spirit::karma::rule< Iterator, std::u32string() > name;
        boost::spirit::karma::rule< Iterator, char32_t() > nameStartChar;
        boost::spirit::karma::rule< Iterator, char32_t() > nameNonStartChar;
        boost::spirit::karma::rule< Iterator, char32_t() > nameChar;
        boost::spirit::karma::rule< Iterator, char32_t() > char_;
        boost::spirit::karma::rule< Iterator, char32_t() > space_;
        boost::spirit::karma::rule< Iterator, char32_t() > non_space;
      };
    }
  }
}

#endif

