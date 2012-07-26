#ifndef HERMIT_TASK_REMAPPER_HPP
#define HERMIT_TASK_REMAPPER_HPP

#include <boost/config.hpp>

#include <vector>
#include <exception>
#include <boost/asio.hpp>
#include <boost/tr1/memory.hpp>

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
  class task_remapper {
    public:
      task_remapper( size_t size ) {
        work.reset( new boost::asio::io_service::work( task_queue ) );
        for( int count = 0; count != size; ++count ) {
#ifdef BOOST_NO_0X_HDR_FUTURE
          pts.push_back( boost::packaged_task< void >( std::bind( &task_remapper::run, this ) ) );
          end_sync.push_back( pts.back().get_future() );
          boost::thread thr( boost::ref( pts.back() ) );
#else
# ifdef BOOST_NO_LAMBDAS
          end_sync.push_back( std::async( std::bind( &task_remapper::run, this ) ) );
# else
          end_sync.push_back( std::async( [&]() { this->run(); } ) );
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
        for( auto &elem: end_sync )
          elem.get();
      }
      template< typename T >
        void post( T func ) {
          task_queue.post( func );
        }
      template< typename T >
        void set_epilogue( T func ) {
          epilogue.post( func );
        }
    private:
      static void null_epilogue() {}
      void run() noexcept {
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
      std::vector< boost::packaged_task< void > > pts;
      std::vector< boost::unique_future< void > > end_sync;
#else
      std::vector< std::future< void > > end_sync;
#endif
  };
}

#endif
