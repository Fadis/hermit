#include "mpint.hpp"

#include <iostream>

#include <boost/rational.hpp>

typedef boost::rational< fadis::mpint > mpreal;

int main() {
  mpreal foo, bar;
  std::cin >> foo;
  std::cin >> bar;
  std::cout << boost::rational_cast<double>( foo / bar ) << std::endl;
}

