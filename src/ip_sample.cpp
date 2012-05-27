#include <hermit/ip.hpp>

int main() {

hermit::ip_segment< 128 > segment;
  std::cin >> segment;
hermit::ip< 128 > address;
  std::cin >> address;
if( segment.contains( address ) )
  std::cout << "foo" << std::endl;
/*
hermit::mpint hoge;
  std::cin >> hoge;
  std::cout << static_cast< uint32_t >( hoge ) << std::endl;
*/
}

