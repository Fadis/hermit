#include <string>
#include <iostream>
#include <hermit/http_cache_control.hpp>
#include <hermit/spirit/qi/http_trailer.hpp>
#include <hermit/spirit/karma/http_trailer.hpp>

int main() {
  {
    std::string source( "Trailer: hoge, fuga, piyo" );
    std::vector< std::string > decoded;
    {
      hermit::spirit::qi::http_trailer< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::http_trailer< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
}

