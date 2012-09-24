#include <iostream>
#include <hermit/format/read/ipv4.hpp>
#include <hermit/format/write/ipv4.hpp>

int main() {
  std::string address;
  std::cin >> address;
  const auto ipv4seg = hermit::format::read_ipv4segment( address );
  if( ipv4seg ) {
    std::cout << (*ipv4seg).first << std::endl;
    std::cout << (*ipv4seg).second << std::endl;
    const auto encoded = hermit::format::write_ipv4segment( *ipv4seg );
    if( encoded )
      std::cout << *encoded << std::endl;
  }
}

