#include <string>
#include <iostream>
#include <hermit/http_pragma.hpp>
#include <hermit/spirit/qi/http_pragma.hpp>
#include <hermit/spirit/karma/http_pragma.hpp>

int main() {
  {
    std::string source( "Pragma: no-cache, hoge=fuga" );
    hermit::http::pragma decoded;
    {
      hermit::spirit::qi::http_pragma< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::http_pragma< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
}

