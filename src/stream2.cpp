#include <hermit/stream/native_pack.hpp>

int main() {
  hermit::stream::native_pack< int, 3 > a = { 1, 2, 3 };
  a[ 0 ];
}
