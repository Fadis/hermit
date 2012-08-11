#include <hermit/task_remapper.hpp>
#include <unistd.h>
#include <boost/optional.hpp>

hermit::poller hoge( hermit::task_remapper &sched ) {
  return sched.post( [](){ std::cout << "foo" << std::endl; }, boost::posix_time::milliseconds(200) );
}

int main() {
  hermit::task_remapper sched( 1 );
  boost::optional< hermit::poller > foo = hoge( sched );
  sleep( 1 );
  foo = boost::optional< hermit::poller >(); 
  sleep( 1 );
}

