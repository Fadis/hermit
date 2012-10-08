#include <string>
#include <iostream>
#include <hermit/http_cache_control.hpp>
#include <hermit/spirit/qi/http_cache_control.hpp>
#include <hermit/spirit/karma/http_cache_control.hpp>

int main() {
  {
    std::string source( "Cache-Control: no-cache, max-age=2592000, hoge=\"fu ga\"" );
    hermit::http::cache_control_request decoded;
    {
      hermit::spirit::qi::http_cache_control_request< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::http_cache_control_request< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
  {
    std::string source( "Cache-Control: private=\"hoge,fuga,piyo\", max-age=2592000" );
    hermit::http::cache_control_response decoded;
    {
      hermit::spirit::qi::http_cache_control_response< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::http_cache_control_response< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
}

