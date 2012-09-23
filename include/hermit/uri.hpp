#ifndef HERMIT_URI_HPP
#define HERMIT_URI_HPP

#include <string>
#include <vector>
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
  struct uri {
    std::string scheme;
    boost::optional< authority > authority;
    boost::filesystem::path path;
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
  hermit::uri,
  (std::string, scheme)
  (boost::optional< hermit::authority >, authority)
  (boost::filesystem::path, path)
  (boost::optional< std::string >, query)
  (boost::optional< std::string >, fragment)
)

#endif

