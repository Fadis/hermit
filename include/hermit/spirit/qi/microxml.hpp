#ifndef HERMIT_SPIRIT_QI_MICROXML_HPP
#define HERMIT_SPIRIT_QI_MICROXML_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/microxml.hpp>
#include <hermit/format/read/utf16.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template< typename Iterator >
      class microxml :
        public boost::spirit::qi::grammar<
          Iterator,
          hermit::microxml()
        > {
      public:
        microxml() : microxml::base_type( root ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          element = '<' >>  
        } 
      private:
        boost::spirit::qi::rule< Iterator, hermit::microxml() > root;
        boost::spirit::qi::rule< Iterator, hermit::microxml() > root;
      };
    }
  }
}

#endif

