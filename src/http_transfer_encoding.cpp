#include <string>
#include <iostream>
#include <hermit/http_transfer_encoding.hpp>
#include <hermit/spirit/qi/http_transfer_encoding.hpp>

int main() {
  {
    std::string source( "Transfer-Encoding: hoge, fuga;a=b;c=d, piyo" );
    std::vector< hermit::http::transfer_encoding > decoded;
    {
      hermit::spirit::qi::http_transfer_encoding< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
/*    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::http_connection< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }*/
    std::cout << encoded << std::endl;
  }
}

