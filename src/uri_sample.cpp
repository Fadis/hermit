#include <string>
#include <iostream>
#include <hermit/format/read/uri.hpp>
#include <hermit/format/write/uri.hpp>

int main() {
  std::string source;
  std::cin >> source;
  const auto uri = hermit::format::read_uri( source );
  if( !uri ) {
    std::cerr << "Invalid URI" << std::endl;
    abort();
  }
  std::cout << "scheme : " << (*uri).scheme << std::endl;
  if( (*uri).authority ) {
    hermit::authority authority = *(*uri).authority;
    if( !authority.userinfo.empty() ) {
      std::cout << "userinfo : ";
      for( const auto &elem: authority.userinfo )
        std::cout << elem << " ";
      std::cout << std::endl;
    }
    std::cout << "host : ";
    if( authority.host.which() == 0 )
      std::cout << boost::get< hermit::ipv4 >( authority.host );
    else if( authority.host.which() == 1 )
      std::cout << boost::get< hermit::ipv6 >( authority.host ).first << "," << boost::get< hermit::ipv6 >( authority.host ).second;
    else if( authority.host.which() == 2 )
      std::cout << boost::get< std::string >( authority.host );
    std::cout << std::endl;
    if( authority.port )
      std::cout << "port : " << *authority.port << std::endl;
  }
  std::cout << "path : " << ( (*uri).path.absolute ? "(absolute)" : "(relative)" );
  for( const auto elem: (*uri).path.segments )
    std::cout << '/' << elem;
  std::cout << std::endl;
  if( (*uri).query )
    std::cout << "query : " << *(*uri).query << std::endl;
  if( (*uri).fragment )
    std::cout << "fragment : " << *(*uri).fragment << std::endl;
  const auto encoded = hermit::format::write_uri( *uri );
  if( encoded )
    std::cout << "uri : " << *encoded << std::endl;
}

