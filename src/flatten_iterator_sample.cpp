#include <hermit/flatten_iterator.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::vector< std::vector< int > > vec = { { 1, 2 }, { 3, 4 } };
  hermit::flatten_iterator< std::vector< std::vector< int > >::iterator > begin( vec.begin() );
  hermit::flatten_iterator< std::vector< std::vector< int > >::iterator > end( vec.end() );
  std::for_each( begin, end, []( int x ) { std::cout << x << std::endl; } );
}

