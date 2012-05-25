#include <hermit/ip.hpp>

int main() {

hermit::ip< 128 > foo;
  std::cin >> foo;
  std::cout << foo << std::endl;
  if( hermit::is_loopback( foo ) )
    std::cout << "This is a loopback address." << std::endl;
  if( hermit::is_local( foo ) )
    std::cout << "This is a local address." << std::endl;
  if( hermit::is_link_local( foo ) )
    std::cout << "This is a link local address." << std::endl;
/*
hermit::mpint hoge;
  std::cin >> hoge;
  std::cout << static_cast< uint32_t >( hoge ) << std::endl;
*/
}

