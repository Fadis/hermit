#include <iostream>
#include <hermit/stream/native_pack.hpp>
#include <hermit/stream/native_pack_operator.hpp>
#include <hermit/operator_traits.hpp>

int main() {
  hermit::stream::native_pack< int, 3 > a = { 1, 2, 3 };
  hermit::stream::native_pack< int, 3 > b = a + +a;
  std::cout << b[ 0 ] << std::endl;
}
