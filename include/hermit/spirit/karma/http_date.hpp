#ifndef HERMIT_SPIRIT_KARMA_HTTP_DATE_HPP
#define HERMIT_SPIRIT_KARMA_HTTP_DATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian_calendar.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <hermit/spirit/karma/rfc1123_date.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename Iterator>
        class http_date :
          public boost::spirit::karma::grammar< Iterator, boost::posix_time::ptime() > {
            public:
              http_date() : http_date::base_type( root ) {
                namespace karma = boost::spirit::karma;
                namespace phx = boost::phoenix;
                root = "Date: " << rfc1123;
              }
            private:
              rfc1123_date< Iterator > rfc1123;
              boost::spirit::karma::rule< Iterator, boost::posix_time::ptime() > root;
      };
    }
  }
}

#endif

