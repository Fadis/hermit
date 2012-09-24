#include <hermit/ip.hpp>
#include <hermit/spirit/qi/ipv6.hpp>


int main() {
  std::string address;
  std::cin >> address;
  hermit::ipv6 result;
  {
    hermit::spirit::qi::ipv6< std::string::iterator > rule;
    auto iter = address.begin();
    if( boost::spirit::qi::parse( iter, address.end(), rule, result ) && iter == address.end() ) {
      std::cout << std::hex << result.first << " " << result.second << std::endl;
    }
  }
}

