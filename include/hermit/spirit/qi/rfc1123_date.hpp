#ifndef HERMIT_SPIRIT_QI_RFC1123_DATE_HPP
#define HERMIT_SPIRIT_QI_RFC1123_DATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian_calendar.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace qi {
      template <typename InputIterator>
        class rfc1123_date :
          public boost::spirit::qi::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              rfc1123_date() : rfc1123_date::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace dt = boost::date_time;
                namespace phx = boost::phoenix;
                root = ( wkday >> qi::omit[ *qi::standard::space ] >> ',' >> qi::omit[ +qi::standard::space ] >>
                         date >> qi::omit[ +qi::standard::space ] >> time >> qi::omit[ +qi::standard::space ] >>
                         zone )[
                  qi::_pass = qi::_1 == phx::bind( &boost::gregorian::date::day_of_week, &qi::_2 ),
                  qi::_val = phx::construct< boost::posix_time::ptime >( qi::_2,
                    qi::_3 + phx::construct< boost::posix_time::time_duration >( qi::_4, 0, 0 )
                  )
                ];
                date = ( dec2_p >> qi::omit[ +qi::standard::space ] >> month >> qi::omit[ +qi::standard::space ] >> dec4_p )[
                  qi::_pass = qi::_1 <= phx::bind( boost::gregorian::gregorian_calendar::end_of_month_day, qi::_3, qi::_2 ),
                  qi::_val = phx::construct< boost::gregorian::date >( qi::_3, qi::_2, qi::_1 )
                ];
                time = ( dec2_p >> qi::omit[ *qi::standard::space ] >> ':' >> qi::omit[ *qi::standard::space ] >>
                         dec2_p >> qi::omit[ *qi::standard::space ] >> ':' >> qi::omit[ *qi::standard::space ] >>
                         dec2_p )[
                  qi::_pass = qi::_1 < 24u && qi::_2 < 60u && qi::_3 < 60u,
                  qi::_val = phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, qi::_3 )
                ];
                wkday.add( "Mon", dt::Monday )( "Tue", dt::Tuesday )( "Wed", dt::Wednesday )( "Thu", dt::Thursday )
                         ( "Fri", dt::Friday )( "Sat", dt::Saturday )( "Sun", dt::Sunday );
                month.add( "Jan", dt::Jan )( "Feb", dt::Feb )( "Mar", dt::Mar )( "Apr", dt::Apr )
                         ( "May", dt::May )( "Jun", dt::Jun )( "Jul", dt::Jul )( "Aug", dt::Aug )
                         ( "Sep", dt::Sep )( "Oct", dt::Oct )( "Nov", dt::Nov )( "Dec", dt::Dec );
                zone.add( "UT", 0 )( "GMT", 0 )( "EST", -5 )( "EDT", -4 )( "CST", -6 )( "CDT", -5 )
                        ( "MST", -7 )( "MDT", -6 )( "PST", -8 )( "PDT", -7 )( "Z", 0 )( "A" -1 )
                        ( "B", -2 )( "C", -3 )( "D", -4 )( "E", -5 )( "F", -6 )( "G", -7 )
                        ( "H", -8 )( "I", -9 )( "K", -10 )( "L", -11 )( "M", -12 )( "N", 1 )
                        ( "O", 2 )( "P", 3 )( "Q", 4 )( "R", 5 )( "S", 6 )( "T", 7 )( "U", 8 )
                        ( "V", 9 )( "W", 10 )( "X", 11 )( "Y", 12 );
              }
            private:
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 2> dec2_p;
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 4> dec4_p;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::ptime() > root;
              boost::spirit::qi::rule< InputIterator, boost::gregorian::date() > date;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > time;
              boost::spirit::qi::symbols< char, boost::date_time::weekdays > wkday; 
              boost::spirit::qi::symbols< char, int > zone; 
              boost::spirit::qi::symbols< char, boost::date_time::months_of_year > month;
      };
    }
  }
}

#endif

