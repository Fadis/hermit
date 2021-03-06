#ifndef HERMIT_TASK_REMAPPER_HPP
#define HERMIT_TASK_REMAPPER_HPP

#include <boost/config.hpp>

#include <vector>
#include <exception>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#ifdef BOOST_ASIO_HAS_STD_CHRONO
#include <chrono>
#else
#include <boost/chrono.hpp>
#endif

#ifdef BOOST_NO_0X_HDR_FUTURE
#include <boost/tr1/functional.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#else
# ifdef BOOST_NO_LAMBDAS
#include <boost/tr1/functional.hpp>
# endif
#include <future>
#endif

namespace hermit {
  template< typename Time >
  class poller_core {
    public:
      template< typename Func >
        poller_core( boost::asio::io_service &service, Func func,
            Time interval_
            ) : timer( service ), task( func ), interval( interval_ ) {
#ifdef BOOST_ASIO_HAS_STD_CHRONO
          using std::chrono::high_resolution_clock; 
#else
          using boost::chrono::high_resolution_clock; 
#endif
          time_stamp = high_resolution_clock::now();
          timer.expires_from_now( interval_ );
          timer.async_wait( boost::bind( &poller_core::timeout, this, boost::asio::placeholders::error ) );
        }
      void timeout( const boost::system::error_code &error ) {
#ifdef BOOST_ASIO_HAS_STD_CHRONO
        using namespace std::chrono; 
#else
        using namespace boost::chrono; 
#endif
        if( !error ) {
          task();
          time_point<high_resolution_clock> new_time_stamp = high_resolution_clock::now();
          Time elapsed_time = duration_cast< Time >( new_time_stamp - time_stamp );
          time_stamp = new_time_stamp;
          auto next_sleep_time = interval + interval - elapsed_time;
          timer.expires_from_now( ( next_sleep_time > Time( 0 ) ) ? next_sleep_time : Time( 0 ) );
          timer.async_wait( boost::bind( &poller_core::timeout, this, boost::asio::placeholders::error ) );
        }
      }
    private:
      boost::asio::high_resolution_timer timer;
      std::function< void() > task;
      Time interval;
#ifdef BOOST_ASIO_HAS_STD_CHRONO
      std::chrono::time_point<std::chrono::high_resolution_clock> time_stamp;
#else
      boost::chrono::time_point<boost::chrono::high_resolution_clock> time_stamp;
#endif
  };

  class poller {
    public:
      template< typename Func, typename Time >
        poller( boost::asio::io_service &service_, Func func,
            Time interval
            ) : core( new poller_core< Time >( service_, func, interval ) ) {}
    private:
      std::shared_ptr< void > core;
  };

  class task_remapper {
    public:
      task_remapper( size_t size ) {
        work.reset( new boost::asio::io_service::work( task_queue ) );
        for( size_t count = 0; count != size; ++count ) {
#ifdef BOOST_NO_0X_HDR_FUTURE
          end_sync.create_thread( std::bind( &task_remapper::run, this ) );
#else
# ifdef BOOST_NO_LAMBDAS
          end_sync.push_back( std::async( std::launch::async, std::bind( &task_remapper::run, this ) ) );
# else
          end_sync.push_back( std::async( std::launch::async, [&]() { this->run(); } ) );
# endif
#endif
        }
      }
#ifdef BOOST_NO_DELETED_FUNCTIONS
    private:
      task_remapper( const task_remapper& );
      task_remapper& operator=( const task_remapper& );
    public:
#else
      task_remapper( const task_remapper& ) = delete;
      task_remapper& operator=( const task_remapper& ) = delete;
#endif 
      ~task_remapper() {
        work.reset();
#ifdef BOOST_NO_0X_HDR_FUTURE
        end_sync.join_all();
#else
        for(
          std::vector< std::future< void > >::iterator iter = end_sync.begin();
          iter != end_sync.end(); ++iter
        ) iter->get();
#endif
      }
      template< typename T >
        void post( T func ) {
          task_queue.post( func );
        }
      template< typename T, typename Time >
        poller post( T func, Time time ) {
          return poller( task_queue, func, time );
        }
      template< typename T >
        void set_epilogue( T func ) {
          epilogue.post( func );
        }
      void sync() {
#ifdef BOOST_NO_0X_HDR_FUTURE
        std::shared_ptr< boost::promise< void > > signal( new boost::promise< void >() );
#else
        std::shared_ptr< std::promise< void > > signal( new std::promise< void >() );
#endif
#ifdef BOOST_NO_LAMBDAS
# ifdef BOOST_NO_0X_HDR_FUTURE
        task_queue.post( std::bind( &boost::thread::promise< void >::set_value, signal ) );
# else
        task_queue.post( std::bind( &std::promise< void >::set_value, signal ) );
# endif
#else
        task_queue.post( [=](){ signal->set_value(); } );
#endif
        signal->get_future().wait();
      }
    private:
      static void null_epilogue() {}
#ifdef BOOST_NO_NOEXCEPT
      void run() {
#else
      void run() noexcept {
#endif
        try {
          task_queue.run();
          epilogue.run();
        } catch ( std::exception &e ) {
          std::cerr << "An exception was thrown from queued tasks. : " << e.what() << std::endl;
          std::cerr << "Aborting." << std::endl;
          std::abort();
        } catch ( ... ) {
          std::cerr << "An exception was thrown from queued tasks." << std::endl;
          std::cerr << "Aborting." << std::endl;
          std::abort();
        }
      }
      std::unique_ptr<boost::asio::io_service::work> work;
      boost::asio::io_service task_queue;
      boost::asio::io_service epilogue;
#ifdef BOOST_NO_0X_HDR_FUTURE
      boost::thread_group end_sync;
#else
      std::vector< std::future< void > > end_sync;
#endif
  };
}

#endif
