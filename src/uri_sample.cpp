#include <hermit/uri.hpp>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/filesystem/path.hpp>

int main() {
  hermit::authority_parser< std::string::iterator > foo;
  hermit::path_parser< std::string::iterator, true, false > bar;
  hermit::query_parser< std::string::iterator > moo;
  hermit::fragment_parser< std::string::iterator > woo;
  hermit::scheme_parser< std::string::iterator > loo;
  hermit::uri_parser< std::string::iterator > goo;
  std::string str;
  std::cin >> str;
  std::string::iterator it = str.begin();
  hermit::uri result;
  if( boost::spirit::qi::parse(it, str.end(), goo, result ) & it == str.end() ) {
    std::cout << "Valid" << std::endl;
    std::cout << "scheme : " << result.get_scheme() << std::endl;
    if( result.get_authority() ) {
      hermit::authority authority_ = *result.get_authority();
      if( authority_.get_userinfo() )
        std::cout << "username : " << (*authority_.get_userinfo())[ 0 ] << std::endl;
      if( authority_.get_host().which() == 0 )
        std::cout << "host : " << boost::get<hermit::ipv4>( authority_.get_host() ) << std::endl;
      else if( authority_.get_host().which() == 1 )
        std::cout << "host : " << boost::get<hermit::ipv6>( authority_.get_host() ) << std::endl;
      else if( authority_.get_host().which() == 2 )
        std::cout << "host : " << boost::get<std::string>( authority_.get_host() ) << std::endl;
      if( authority_.get_port() )
        std::cout << "port : " << *authority_.get_port() << std::endl;
    }
    std::cout << "path : " << result.get_path().string() << " ";
    if( result.get_path().is_absolute() )
      std::cout << "(absolute)" << std::endl;
    else
      std::cout << "(relative)" << std::endl;
    if( result.get_query() )
      std::cout << "query : " << *result.get_query() << std::endl;
    if( result.get_fragment() )
      std::cout << "fragment : " << *result.get_fragment() << std::endl;
  }
  else
    std::cout << "Invalid" << std::endl;
}


