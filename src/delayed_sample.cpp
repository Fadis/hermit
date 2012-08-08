#include <iostream>
#include <hermit/delayed.hpp>
#include <thread>

#include <unistd.h>

int main() {
  std::shared_ptr< hermit::delayed< int >::promise > promise( new hermit::delayed< int >::promise() );
  hermit::delayed< int > d( promise );
  std::thread bg( [promise](){ sleep( 1 ); promise->set_value( 3 );} );
  std::cout << *d << std::endl;
  std::cout << *d << std::endl;
  std::cout << *d << std::endl;
  bg.join();
}
