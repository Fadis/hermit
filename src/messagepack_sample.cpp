#include <iostream>
#include <iterator>
#include <string>
#include <cstdint>
#include <hermit/messagepack.hpp>
#include <hermit/format/read/messagepack.hpp>

int main() {
  hermit::spirit::qi::messagepack< std::string::iterator > grammar;
}
