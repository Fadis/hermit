#include <hermit/uri.hpp>
#include <string>
#include <boost/spirit/include/qi.hpp>

int main() {
  hermit::authority_parser< std::string::iterator > foo;
  hermit::path_parser< std::string::iterator, true, false > bar;
  std::string str;
  std::cin >> str;
  std::string::iterator it = str.begin();
  hermit::authority result;
  if( boost::spirit::qi::parse(it, str.end(), foo, result ) & it == str.end() ) {
    std::cout << "Valid" << std::endl;
    if( result.get_userinfo() )
      std::cout << "username : " << (*result.get_userinfo())[ 0 ] << std::endl;
    if( result.get_host().which() == 0 )
      std::cout << "host : " << boost::get<hermit::ipv4>( result.get_host() ) << std::endl;
    else if( result.get_host().which() == 1 )
      std::cout << "host : " << boost::get<hermit::ipv6>( result.get_host() ) << std::endl;
    else if( result.get_host().which() == 2 )
      std::cout << "host : " << boost::get<std::string>( result.get_host() ) << std::endl;
    if( result.get_port() )
      std::cout << "port : " << *result.get_port() << std::endl;
  }
  else
    std::cout << "Invalid" << std::endl;
}


