#include <string>
#include <algorithm>
#include <iostream>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf16.hpp>

int main() {
  const std::vector< uint8_t > source = { 0xfe, 0xff, 0xd8, 0x67, 0xde, 0x3d };
  const auto decoded = hermit::format::read_utf16( source );
  if( !decoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  for( const auto elem : *decoded )
    std::cout << static_cast< int >( elem ) << " ";
  std::cout << std::endl;
  const auto encoded = hermit::format::write_utf16( *decoded );
  if( !encoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  if( ( source.size() == (*encoded).size() ) &&
      std::equal( source.begin(), source.end(), (*encoded).begin() ) )
    std::cout << "match" << std::endl;
}

