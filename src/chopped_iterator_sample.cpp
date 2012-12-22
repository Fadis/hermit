#include <hermit/chopped_iterator.hpp>
#include <boost/range/algorithm.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() { 
  std::vector< int > vec = { 1,2,3,4,5,6,7,8 };
  hermit::chopped_iterator< std::vector< int >::iterator > begin( vec.begin(), 2 );
  hermit::chopped_iterator< std::vector< int >::iterator > end( vec.end(), 2 );
  std::for_each( begin, end, []( std::pair< std::vector< int >::iterator, std::vector< int >::iterator > range ) {
      boost::for_each( range, []( int value ) { std::cout << value << " "; } );
      std::cout << std::endl;
  } );
}

