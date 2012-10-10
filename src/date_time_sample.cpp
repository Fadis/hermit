#include <string>
#include <iostream>
#include <hermit/spirit/qi/http_date.hpp>
#include <hermit/spirit/karma/rfc850_date.hpp>
#include <hermit/spirit/karma/rfc1123_date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main() {
  {
    std::string source( "Sun, 06 Nov 1994 08:49:37 GMT" );
    boost::posix_time::ptime decoded;
    {
      hermit::spirit::qi::http_date< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::rfc1123_date< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
  {
    std::string source( "Sunday, 06-Nov-94 08:49:37 GMT" );
    boost::posix_time::ptime decoded;
    {
      hermit::spirit::qi::http_date< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::cout << decoded << std::endl;
    std::string encoded;
    {
      typedef std::back_insert_iterator<std::string> sink_type;
      hermit::spirit::karma::rfc850_date< sink_type > rule;
      boost::spirit::karma::generate( sink_type( encoded ), rule, decoded );
    }
    std::cout << encoded << std::endl;
  }
  {
    std::string source( "Sun Nov  6 08:49:37 1994" );
    boost::posix_time::ptime decoded;
    {
      hermit::spirit::qi::http_date< std::string::iterator > rule;
      auto iter = source.begin();
      boost::spirit::qi::parse( iter, source.end(), rule, decoded );
      std::cout << std::distance( source.begin(), iter ) << std::endl;
    }
    std::cout << decoded << std::endl;
  }
}

