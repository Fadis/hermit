#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <hermit/microxml.hpp>
#include <hermit/spirit/qi/microxml.hpp>

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
}