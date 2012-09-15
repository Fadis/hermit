#include <iostream>
#include <iterator>
#include <string>
#include <cstdint>
#include <hermit/messagepack.hpp>
#include <hermit/format/write/messagepack.hpp>

int main() {
  hermit::spirit::karma::messagepack< std::back_insert_iterator< std::vector< uint8_t > > > grammar;
}
