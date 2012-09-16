#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <hermit/format/write/messagepack.hpp>
#include <hermit/format/convert/json2messagepack.hpp>
#include <hermit/format/read/json.hpp>

int main() {
  std::string sample;
  sample.assign( std::istream_iterator<char>( std::cin ), std::istream_iterator<char>() );
  const auto json_value = hermit::format::read_json( sample );
  if( !json_value ) {
    std::cerr << "invalid json." << std::endl;
    std::abort();
  }
  const auto messagepack_value = apply_visitor( hermit::format::convert::json2messagepack(), *json_value );
  const auto result = hermit::format::write_messagepack( messagepack_value );
  if( !result ) {
    std::cerr << "serialization failed." << std::endl;
    std::abort();
  }
  std::fstream file( "sample.mp", std::ios::out|std::ios::binary );
  std::copy( (*result).begin(), (*result).end(), std::ostreambuf_iterator<char>( file ) );
}
