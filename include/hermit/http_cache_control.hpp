#ifndef HERMIT_HTTP_CACHE_CONTROL_HPP
#define HERMIT_HTTP_CACHE_CONTROL_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <hermit/uri.hpp>

namespace hermit {
  namespace http {
    struct cache_control_request {
      typedef std::map< std::string, boost::optional< std::string > > extension_type;
      cache_control_request() :
        no_cache( false ), no_store( false ),
        max_stale( false ), no_transform( false ),
        only_if_cached( false ) {}
      bool no_cache;
      bool no_store;
      boost::optional< unsigned int > max_age;
      bool max_stale;
      boost::optional< unsigned int > max_stale_seconds;
      boost::optional< unsigned int > min_fresh;
      bool no_transform;
      bool only_if_cached;
      extension_type extension;
    };

    struct cache_control_response {
      typedef std::map< std::string, boost::optional< std::string > > extension_type;
      cache_control_response() :
        public_( false ), private_( false ),
        no_cache( false ), no_store( false ),
        no_transform( false ), must_revalidate( false ),
        proxy_revalidate( false ) {}
      bool public_;
      bool private_;
      std::vector< std::string > private_name;
      bool no_cache;
      std::vector< std::string > no_cache_name;
      bool no_store;
      bool no_transform;
      bool must_revalidate;
      bool proxy_revalidate;
      boost::optional< unsigned int > max_age;
      boost::optional< unsigned int > s_maxage;
      extension_type extension;
    };

  }
}
BOOST_FUSION_ADAPT_STRUCT(
  hermit::http::cache_control_request,
  (bool, no_cache)
  (bool, no_store)
  (boost::optional< unsigned int >, max_age)
  (bool, max_stale)
  (boost::optional< unsigned int >, max_stale_seconds)
  (boost::optional< unsigned int >, min_fresh)
  (bool, no_transform)
  (bool, only_if_cached)
  (hermit::http::cache_control_request::extension_type,extension)
)

BOOST_FUSION_ADAPT_STRUCT(
  hermit::http::cache_control_response,
  (bool, public_)
  (bool, private_)
  (std::vector< std::string >, private_name)
  (bool, no_cache)
  (std::vector< std::string >, no_cache_name)
  (bool, no_store)
  (bool, no_transform)
  (bool, must_revalidate)
  (bool, proxy_revalidate)
  (boost::optional< unsigned int >, max_age)
  (boost::optional< unsigned int >, s_maxage)
  (hermit::http::cache_control_response::extension_type,extension)
)
#endif

