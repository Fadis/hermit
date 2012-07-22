#include <hermit/scheduler.hpp>
#include <unistd.h>

int main() {
  hermit::scheduler sched( 4 );
  for( int count = 0; count != 16; ++count )
    sched.post( [](){ std::cout << "a" << std::endl; sleep( 1 ); } );
}

