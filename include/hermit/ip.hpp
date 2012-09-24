#ifndef HERMIT_IP_HPP
#define HERMIT_IP_HPP

#include <cstdint>
#include <utility>

namespace hermit {
  typedef std::uint32_t ipv4;
  typedef std::pair< std::uint64_t, std::uint64_t > ipv6;
}

#endif
