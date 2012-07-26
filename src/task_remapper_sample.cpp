#include <hermit/task_remapper.hpp>
#include <unistd.h>

int main() {
  hermit::task_remapper sched( 4 );
  sched.set_epilogue( [](){ std::cout << "end." << std::endl; } );
  for( int count = 0; count != 16; ++count )
    sched.post( [count](){ std::cout << count << std::endl; sleep( 1 ); } );
}

