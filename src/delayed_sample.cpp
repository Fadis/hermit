#include <iostream>
#include <hermit/delayed.hpp>
#include <thread>

#include <unistd.h>

int main() {
  hermit::delayed< int > d;
  auto promise = d.get_promise();
  std::thread bg( [promise](){ sleep( 1 ); promise->set_value( 3 );} );
  std::cout << d << std::endl;
  std::cout << d << std::endl;
  std::cout << d << std::endl;
  bg.join();
}
