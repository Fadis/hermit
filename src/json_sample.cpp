#include <iostream>
#include <iterator>
#include <string>
#include <cstdint>
#include <hermit/json.hpp>
#include <hermit/format/json.hpp>

int main() {
  std::string sample;
  sample.assign( std::istream_iterator<char>( std::cin ), std::istream_iterator<char>() );
  const auto decoded = hermit::format::read_json( sample );
  if( decoded ) {
    const auto encoded = hermit::format::write_json( *decoded );
    if( encoded )
      std::cout << *encoded << std::endl;
  }
}
