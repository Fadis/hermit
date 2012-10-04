#ifndef HERMIT_SPIRIT_QI_HTTP_DATE_HPP
#define HERMIT_SPIRIT_QI_HTTP_DATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian_calendar.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <hermit/spirit/qi/rfc1123_date.hpp>
#include <hermit/spirit/qi/rfc850_date.hpp>
#include <hermit/spirit/qi/asctime.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class http_date :
          public boost::spirit::qi::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              http_date() : http_date::base_type( root ) {
                root = rfc1123|rfc850|ascii;
              }
            private:
              rfc1123_date< InputIterator > rfc1123;
              rfc850_date< InputIterator > rfc850;
              asctime< InputIterator > ascii;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::ptime() > root;
      };
    }
  }
}

#endif

