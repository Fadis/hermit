#ifndef HERMIT_HTTP_PRAGMA_HPP
#define HERMIT_HTTP_PRAGMA_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <hermit/uri.hpp>

namespace hermit {
  namespace http {
    struct pragma {
      typedef std::map< std::string, boost::optional< std::string > > extension_type;
      pragma() : no_cache( false ) {}
      bool no_cache;
      extension_type extension;
    };
  }
}
BOOST_FUSION_ADAPT_STRUCT(
  hermit::http::pragma,
  (bool, no_cache)
  (hermit::http::pragma::extension_type,extension)
)

#endif

