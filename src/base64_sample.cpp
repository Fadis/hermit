#include <string>
#include <algorithm>
#include <iostream>
#include <hermit/format/read/base64.hpp>
#include <hermit/format/write/utf16.hpp>

int main() {
  const std::string source( "SGVsbG8gV29ybGQhCg==" );
  const auto decoded = hermit::format::read_base64( source );
  if( !decoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  for( const auto elem : *decoded )
    std::cout << elem;
  std::cout << std::endl;
}

