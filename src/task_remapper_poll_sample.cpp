#include <hermit/task_remapper.hpp>
#include <unistd.h>

int main() {
  hermit::task_remapper sched( 1 );
  auto foo = sched.post( [](){ std::cout << "foo" << std::endl; }, boost::chrono::milliseconds(200) );
  auto bar = sched.post( [](){ std::cout << "bar" << std::endl; }, boost::chrono::milliseconds(300) );
  sleep( 5 );
}

