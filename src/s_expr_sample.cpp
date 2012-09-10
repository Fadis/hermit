#include <iostream>
#include <string>
#include <cstdint>
#include <hermit/s_expr.hpp>

struct print : boost::static_visitor<void> {
  void operator()( hermit::s_expr::integer value ) const {
    std::cout << "integer:" << value;
  }
  void operator()( hermit::s_expr::string value ) const {
    std::cout << "string:" << value;
  }
  void operator()( hermit::s_expr::symbol value ) const {
    std::cout << "symbol:" << value.get_name();
  }
  void operator()( hermit::s_expr::node value ) const {
    std::cout << "node:(";
    for( auto elem: value ) {
      apply_visitor( print(), elem );
      std::cout << ",";
    }
    std::cout << ")";
  }
};

int main() {
  hermit::s_expr::gen< std::back_insert_iterator< std::string > > gen;
  std::string sample;
  std::getline( std::cin, sample );
  auto result = hermit::s_expr::parse( sample );
  if( result ) {
    std::cout << "node:(";
    for( auto elem: *result ) {
      apply_visitor( print(), elem );
      std::cout << ",";
    }
    std::cout << ")" << std::endl;
  }
}
