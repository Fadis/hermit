#include <iostream>
#include <iterator>
#include <string>
#include <cstdint>
#include <algorithm>
#include <hermit/format/read/messagepack.hpp>
#include <hermit/format/convert/messagepack2json.hpp>
#include <hermit/format/write/json.hpp>

int main() {
  std::vector< uint8_t > sample;
  sample.assign( std::istream_iterator<uint8_t>( std::cin ), std::istream_iterator< uint8_t >() );
  const auto messagepack_value = hermit::format::read_messagepack( sample );
  if( !messagepack_value ) {
    std::cerr << "invalid messagepack." << std::endl;
    std::abort();
  }
  const auto json_value = apply_visitor( hermit::format::convert::messagepack2json(), *messagepack_value );
  const auto result = hermit::format::write_json( json_value );
  if( !result ) {
    std::cerr << "serialization failed." << std::endl;
    std::abort();
  }
  std::copy( (*result).begin(), (*result).end(), std::ostream_iterator<char>( std::cout ) );
}
