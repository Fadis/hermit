#include <array>
#include <chrono>
#include <iostream>
#include <hermit/simd.hpp>

void a() {
  hermit::simd::vector< float, 64000, 8 > hoge;
  std::fill( hoge.begin(), hoge.end(), 1 );
  hermit::simd::vector< float, 64000, 8 > fuga;
  std::fill( fuga.begin(), fuga.end(), 2 );
  auto begin = std::chrono::high_resolution_clock::now();
  hermit::simd::vector< float, 64000, 8 > piyo = hoge + fuga * hoge * hoge;
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << piyo[ 0 ] << " " << piyo[ 1 ] << " " << piyo[ 2 ] << std::endl;
  std::cout << std::chrono::duration< double >( end - begin ).count() << std::endl;
}

void b() {
  std::array< float, 64000 > hoge;
  std::fill( hoge.begin(), hoge.end(), 1 );
  std::array< float, 64000 > fuga;
  std::fill( fuga.begin(), fuga.end(), 2 );
  auto begin = std::chrono::high_resolution_clock::now();
  std::array< float, 64000 > piyo;
  for( int index = 0; index != 64000; ++index )
    piyo[ index ] = hoge[ index ] + fuga[ index ] * hoge[ index ] * hoge[ index ];
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << piyo[ 0 ] << " " << piyo[ 1 ] << " " << piyo[ 2 ] << std::endl;
  std::cout << std::chrono::duration< double >( end - begin ).count() << std::endl;
}

int main() {
  a();
  b();
}
