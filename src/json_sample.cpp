#include <iostream>
#include <string>
#include <cstdint>
#include <hermit/json.hpp>


int main() {
  std::string sample( std::istream_iterator<char>( std::cin ), std::istream_iterator<char>() );
  const auto decoded = hermit::json::parse( sample );
  if( decoded ) {
    const auto encoded = hermit::json::generate( *decoded );
    if( encoded )
      std::cout << *encoded << std::endl;
  }
}
