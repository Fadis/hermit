#include <string>
#include <algorithm>
#include <iostream>
#include <hermit/format/read/utf32.hpp>
#include <hermit/format/write/utf32.hpp>

int main() {
  const std::vector< uint8_t > source = {
    0xff, 0xfe, 0x00, 0x00, 0x6b, 0x30, 0x00, 0x00,
    0x83, 0x30, 0x00, 0x00, 0xfc, 0x30, 0x00, 0x00,
    0x93, 0x30, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00
  };
  const auto decoded = hermit::format::read_utf32( source );
  if( !decoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  for( const auto elem : *decoded )
    std::cout << static_cast< int >( elem ) << " ";
  std::cout << std::endl;
  const auto encoded = hermit::format::write_utf32( *decoded );
  if( !encoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  if( ( source.size() == (*encoded).size() ) &&
      std::equal( source.begin(), source.end(), (*encoded).begin() ) )
    std::cout << "match" << std::endl;
}

