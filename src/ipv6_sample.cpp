#include <iostream>
#include <hermit/format/read/ipv6.hpp>
#include <hermit/format/write/ipv6.hpp>

int main() {
  std::string address;
  std::cin >> address;
  const auto ipv6 = hermit::format::read_ipv6( address );
  if( ipv6 ) {
    std::cout << (*ipv6).first << "," << (*ipv6).second << std::endl;
    const auto encoded = hermit::format::write_ipv6( *ipv6 );
    if( encoded )
      std::cout << *encoded << std::endl;
  }
}

