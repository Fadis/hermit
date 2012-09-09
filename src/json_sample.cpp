#include <iostream>
#include <string>
#include <cstdint>
#include <hermit/json.hpp>

struct printer : boost::static_visitor<> {
  void operator()( std::nullptr_t value ) const {
    std::cout << "null";
  }
  void operator()( bool value ) const {
    std::cout << ( value ? "true" : "false" );
  }
  void operator()( double value ) const {
    std::cout << value;
  }
  void operator()( const std::string &value ) const {
    std::cout << value;
  }
  template< typename T >
  void operator()( const std::vector< T > &value ) const {
    std::cout << '[';
    for( auto elem: value ) {
      apply_visitor( printer(), elem );
      std::cout << ',';
    }
    std::cout << ']';
  }
  template< typename T >
  void operator()( const std::map< std::string, T > &value ) const {
    std::cout << '{';
    for( auto elem: value ) {
      std::cout << elem.first << ':';
      apply_visitor( printer(), elem.second );
      std::cout << ',';
    }
    std::cout << '}';
  }
};

int main() {
  std::string sample;
  std::getline( std::cin, sample );
  auto result = hermit::json::parse( sample );
 
  std::string result2; 
  hermit::json::gen< std::back_insert_iterator< std::string > > gen;
  boost::spirit::karma::generate(
    std::back_inserter( result2 ),
    gen,
    result
  );
  std::cout << result2 << std::endl;
  if( result ) {
    apply_visitor( printer(), *result );
    std::cout << std::endl;
  }
}
