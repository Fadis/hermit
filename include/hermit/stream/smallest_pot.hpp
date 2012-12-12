#ifndef HERMIT_STREAM_SMALLEST_POT_HPP
#define HERMIT_STREAM_SMALLEST_POT_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/shift_left.hpp>

namespace hermit {
  namespace stream {
    template< typename T, typename U = boost::mpl::int_< 1 > >
    struct smallest_pot
    : public boost::mpl::if_<
      boost::mpl::bool_< T::value <= U::value >,
      U,
      smallest_pot< T, boost::mpl::shift_left< U, boost::mpl::int_< 1 > > >
    >::type {};
  }
}

#endif

