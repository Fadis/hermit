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
    class delayed_internal {
      public:
        typedef
#ifdef BOOST_NO_0X_HDR_FUTURE
          boost::promise< T >
#else
          std::promise< T >
#endif
        promise;
        typedef
#ifdef BOOST_NO_0X_HDR_FUTURE
          boost::unique_future< T >
#else
          std::future< T >
#endif
        future;
        typedef
#ifdef BOOST_NO_0X_HDR_MUTEX
          boost::mutex
#else
          std::mutex
#endif
        mutex;
        delayed_internal( std::shared_ptr< promise > value_ ) : value( value_ ) {
          dereference = std::bind( &delayed_internal<T>::initial_access, this );
        }
        const T &operator*() const {
          return dereference();
        }
        operator T() const {
          return dereference();
        }
      private:
        const T &initial_access() {
#ifdef BOOST_NO_0X_HDR_MUTEX
          boost::mutex::scoped_lock lock( guard );
#else
          std::lock_guard< std::mutex > lock( guard );
#endif
          if( value.which() == 0 ) {
            value = boost::get< std::shared_ptr< promise > >( value )->get_future().get();
            dereference = std::bind( &delayed_internal<T>::cached_access, this );
          }
          return boost::get< T >( value );
        }
        const T &cached_access() {
          return boost::get< T >( value );
        }
        mutable std::function< const T&() > dereference;
        boost::variant< std::shared_ptr< promise >, T > value;
        mutex guard;
    };

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
        delayed( std::shared_ptr< promise > value_ ) : internal( new delayed_internal< T >( value_ ) ) {
        }
        const T &operator*() const {
          return **internal;
        }
        operator T() const {
          return **internal;
        }
      private:
        std::shared_ptr< delayed_internal< T > > internal;
    };
}

#endif

