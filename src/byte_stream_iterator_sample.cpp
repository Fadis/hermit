#include <iostream>
#include <vector>
#include <hermit/byte_stream_iterator.hpp>

int main() {
  std::vector< int > hoge = { 1, 2, 3, 4, 5 };
  {
    typedef hermit::little_byte_stream_iterator< std::vector< int >::iterator > adapted_iterator;
    for( adapted_iterator iter( hoge.begin() ); iter != adapted_iterator( hoge.end() ); ++iter  )
      std::cout << static_cast< int >( *iter ) << " ";
    std::cout << std::endl;
  }
  {
    typedef hermit::big_byte_stream_iterator< std::vector< int >::iterator > adapted_iterator;
    for( adapted_iterator iter( hoge.begin() ); iter != adapted_iterator( hoge.end() ); ++iter  )
      std::cout << static_cast< int >( *iter ) << " ";
    std::cout << std::endl;
  }
}

