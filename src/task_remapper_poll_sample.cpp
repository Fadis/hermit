#include <hermit/task_remapper.hpp>
#include <unistd.h>
#include <boost/optional.hpp>

hermit::poller hoge( hermit::task_remapper &sched ) {
#ifdef BOOST_ASIO_HAS_STD_CHRONO
  return sched.post( [](){ std::cout << "foo" << std::endl; }, std::chrono::milliseconds(200) );
#else
  return sched.post( [](){ std::cout << "foo" << std::endl; }, boost::chrono::milliseconds(200) );
#endif
}

int main() {
  hermit::task_remapper sched( 1 );
  boost::optional< hermit::poller > foo = hoge( sched );
  sleep( 1 );
  foo = boost::optional< hermit::poller >(); 
  sleep( 1 );
}

