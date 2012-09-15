#include <string>
#include <iostream>
#include <hermit/spirit/utf8.hpp>

int main() {
  std::string source( "コンパイル時間がのびのびー" );
  std::u32string decoded;
  {
    hermit::spirit::qi::utf8< std::string::iterator > rule;
    auto iter = source.begin();
    boost::spirit::qi::parse( iter, source.end(), *rule, decoded );
  }
  for( auto &elem: decoded )
    std::cout << static_cast< uint32_t >( elem ) << ',';
  std::cout << std::endl;
  std::string encoded;
  {
    typedef std::back_insert_iterator<std::string> sink_type;
    hermit::spirit::karma::utf8< sink_type > rule;
    boost::spirit::karma::generate( sink_type( encoded ), *rule, decoded );
  }
  std::cout << encoded << std::endl;
}

