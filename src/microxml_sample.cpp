#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <hermit/microxml.hpp>
#include <hermit/spirit/qi/microxml.hpp>
#include <hermit/format/convert/microxml2json.hpp>
#include <hermit/format/convert/json2microxml.hpp>
#include <hermit/format/write/json.hpp>

int main() {
  std::vector< uint8_t > sample;
  std::fstream file( "sample.xml", std::ios::in|std::ios::binary );
  sample.assign( std::istreambuf_iterator<char>( file.rdbuf() ), std::istreambuf_iterator<char>() );
  std::cout << sample.size() << std::endl;
  hermit::spirit::qi::microxml< std::vector< uint8_t >::iterator > rule;
  hermit::microxml xml;
  auto iter = sample.begin();
  boost::spirit::qi::parse( iter, sample.end(), rule, xml );
  std::cout << std::distance( sample.begin(), iter ) << std::endl;
  const auto json = hermit::format::convert_microxml_to_json( xml );
  const auto json_str = hermit::format::write_json( json );
  if( json_str ) {
    std::cout << *json_str << std::endl;
  }
  const auto xml2 = hermit::format::convert_json_to_microxml( json );
  const auto json2 = hermit::format::convert_microxml_to_json( xml2 );
  const auto json_str2 = hermit::format::write_json( json2 );
  if( json_str2 ) {
    std::cout << *json_str2 << std::endl;
  }
}

