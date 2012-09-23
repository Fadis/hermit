#include <string>
#include <iostream>
#include <hermit/spirit/qi/uri.hpp>

int main() {
  std::string source;
  std::cin >> source;
  hermit::uri uri;
  hermit::spirit::qi::uri< std::string::iterator > rule;
  auto iter = source.begin();
  if( boost::spirit::qi::parse( iter, source.end(), rule, uri ) && iter == source.end() ) {
    std::cout << "scheme : " << uri.scheme << std::endl;
    if( uri.authority ) {
      hermit::authority authority = *uri.authority;
      std::cout << "userinfo : ";
      for( const auto &elem: authority.userinfo )
        std::cout << elem << " ";
      std::cout << std::endl;
      std::cout << "host : ";
      if( authority.host.which() == 0 )
        std::cout << boost::get< hermit::ipv4 >( authority.host );
      else if( authority.host.which() == 1 )
        std::cout << boost::get< hermit::ipv6 >( authority.host );
      else if( authority.host.which() == 2 )
        std::cout << boost::get< std::string >( authority.host );
      std::cout << std::endl;
      if( authority.port )
        std::cout << "port : " << *authority.port << std::endl;
    }
    std::cout << "path : " << ( uri.path.absolute ? "(absolute)" : "(relative)" );
    for( const auto elem: uri.path.segments )
      std::cout << '/' << elem;
    std::cout << std::endl;
    if( uri.query )
      std::cout << "query : " << *uri.query << std::endl;
    if( uri.fragment )
      std::cout << "fragment : " << *uri.fragment << std::endl;
  }
}

