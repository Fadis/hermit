#ifndef HERMIT_URI_HPP
#define HERMIT_URI_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <hermit/integer.hpp>
#include <hermit/ip.hpp>

namespace hermit {
  typedef boost::variant< ipv4, ipv6, std::string > host;
  struct authority {
    std::vector< std::string > userinfo;
    hermit::host host;
    boost::optional< uint16_t > port;
  };
  struct path {
    bool absolute;
    std::vector< std::string > segments;
  };
  struct uri {
    std::string scheme;
    boost::optional< hermit::authority > authority;
    hermit::path path;
    boost::optional< std::string > query;
    boost::optional< std::string > fragment;
  };
}

BOOST_FUSION_ADAPT_STRUCT(
  hermit::authority,
  (std::vector< std::string >, userinfo)
  (hermit::host, host)
  (boost::optional< uint16_t >, port)
)

BOOST_FUSION_ADAPT_STRUCT(
  hermit::path,
  (bool, absolute)
  (std::vector< std::string >, segments)
)

BOOST_FUSION_ADAPT_STRUCT(
  hermit::uri,
  (std::string, scheme)
  (boost::optional< hermit::authority >, authority)
  (hermit::path, path)
  (boost::optional< std::string >, query)
  (boost::optional< std::string >, fragment)
)

#endif

