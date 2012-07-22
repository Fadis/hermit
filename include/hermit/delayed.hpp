#ifndef HERMIT_DELAYED_HPP
#define HERMIT_DELAYED_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_0X_HDR_MUTEX
#include <boost/thread/mutex.hpp>
#else
#include <mutex>
#endif

#ifdef BOOST_NO_0X_HDR_FUTURE
#include <boost/thread/future.hpp>
#else
#include <future>
#endif

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>
#include <boost/variant.hpp>

namespace hermit {
  template< typename T >
    class delayed {
      public:
        typedef
#ifdef BOOST_NO_0X_HDR_FUTURE
          boost::promise< T >
#else
          std::promise< T >
#endif
        promise;
        typedef
#ifdef BOOST_NO_0X_HDR_MUTEX
          boost::mutex
#else
          std::mutex
#endif
        mutex;
        typedef std::shared_ptr< promise > promise_ptr;
        delayed() : value( promise_ptr( new promise ) ), guard( new mutex ) {
          dereference = std::bind( &delayed<T>::initial_access, this );
        }
        T &operator*() {
          return dereference();
        }
        const T &operator*() const {
          return dereference();
        }
        operator T() const {
          return dereference();
        }
        promise_ptr get_promise() {
#ifdef BOOST_NO_0X_HDR_MUTEX
          boost::mutex::scoped_lock lock( *guard );
#else
          std::lock_guard< std::mutex > lock( *guard );
#endif
          return boost::get< promise_ptr >( value );
        }
      private:
        T &initial_access() {
#ifdef BOOST_NO_0X_HDR_MUTEX
          boost::mutex::scoped_lock lock( *guard );
#else
          std::lock_guard< std::mutex > lock( *guard );
#endif
          if( value.which() == 0 ) {
            value = boost::get< promise_ptr >( value )->get_future().get();
            dereference = std::bind( &delayed<T>::cached_access, this );
          }
          return boost::get< T >( value );
        }
        T &cached_access() {
          return boost::get< T >( value );
        }
        mutable std::function< T&() > dereference;
        boost::variant< promise_ptr, T > value;
        boost::shared_ptr< mutex > guard;
    };
}

#endif

