#include <hermit/ip.hpp>

int main() {

hermit::ip< 128 > foo;
  std::cin >> foo;
  std::cout << boost::format("%X") % foo << std::endl;
  if( hermit::is_loopback( foo ) )
    std::cout << "This is a loopback address." << std::endl;
/*
hermit::mpint hoge;
  std::cin >> hoge;
  std::cout << static_cast< uint32_t >( hoge ) << std::endl;
*/
}

