#ifndef HERMIT_SCHEDULER_HPP
#define HERMIT_SCHEDULER_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_0X_HDR_THREAD
#include <boost/thread.hpp>
#else
#include <thread>
#include <vector>
#endif

#ifdef BOOST_NO_LAMBDAS
#include <boost/bind.hpp>
#endif

#include <boost/asio.hpp>
#include <boost/tr1/memory.hpp>

#include <unistd.h>

namespace hermit {
  class scheduler {
    public:
      scheduler( size_t size ) {
        work.reset( new boost::asio::io_service::work( task_queue ) );
        for( size_t count = 0; count != size; ++count )
#ifdef BOOST_NO_0X_HDR_THREAD
# ifdef BOOST_NO_LAMBDAS
          threads.create_thread( boost::bind( &boost::asio::io_service::run, &task_queue ) );
# else
        threads.create_thread( [&](){ task_queue.run(); } );
# endif
#else
# ifdef BOOST_NO_LAMBDAS
        threads.emplace_thread( boost::bind( &boost::asio::io_service::run, &task_queue ) );
# else
        threads.emplace_back( [&](){ task_queue.run(); } );
# endif
#endif
      }
      ~scheduler() {
        work.reset();
#ifdef BOOST_NO_0X_HDR_THREAD
        threads.join_all();
#else
        for( auto &elem: threads )
          elem.join();
#endif
      }
      template< typename T >
        void post( T func ) {
          task_queue.post( func );
        }
    private:
      std::unique_ptr<boost::asio::io_service::work> work;
      boost::asio::io_service task_queue;
#ifdef BOOST_NO_0X_HDR_THREAD
      boost::thread_group threads;
#else
      std::vector< std::thread > threads;
#endif
  };
}

#endif
