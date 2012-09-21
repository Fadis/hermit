#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdint>
#include <hermit/json.hpp>
#include <hermit/format/json.hpp>

int main() {
  std::vector< uint8_t > sample;
  std::fstream file( "sample.json", std::ios::in|std::ios::binary );
  sample.assign( std::istreambuf_iterator<char>( file.rdbuf() ), std::istreambuf_iterator<char>() );
  const auto decoded = hermit::format::read_json( sample );
  if( decoded ) {
    const auto encoded = hermit::format::write_json( *decoded );
    if( encoded )
      std::cout << *encoded << std::endl;
  }
}
