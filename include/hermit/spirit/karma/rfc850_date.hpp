#ifndef HERMIT_SPIRIT_KARMA_RFC850_DATE_HPP
#define HERMIT_SPIRIT_KARMA_RFC850_DATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian_calendar.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace hermit {
  namespace spirit {
    namespace karma {
      template <typename InputIterator>
        class rfc850_date :
          public boost::spirit::karma::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              rfc850_date() : rfc850_date::base_type( root ) {
                namespace karma = boost::spirit::karma;
                namespace dt = boost::date_time;
                namespace phx = boost::phoenix;
                /*root = ( weekday << ", " << date << ' ' << time << " GMT" )[
                  qi::_pass = qi::_1 == phx::bind( &boost::gregorian::date::day_of_week, &qi::_2 ),
                  qi::_val = phx::construct< boost::posix_time::ptime >( qi::_2,
                    qi::_3 + qi::_4
                  )
                ];
                date = ( dec2_p << '-' << month << '-' << dec2_p )[
                  qi::_pass = qi::_1 <= phx::bind( boost::gregorian::gregorian_calendar::end_of_month_day, qi::_3 + 1900, qi::_2 ),
                  qi::_val = phx::construct< boost::gregorian::date >( qi::_3 + 1900, qi::_2, qi::_1 )
                ];
                time = ( dec2_p >> ':' >> dec2_p >> ':' >> dec2_p )[
                  qi::_pass = qi::_1 < 24u && qi::_2 < 60u && qi::_3 < 60u,
                  qi::_val = phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, qi::_3 )
                ];*/
                weekday.add( dt::Monday, "Monday" )( dt::Tuesday, "Tuesday" )( dt::Wednesday, "Wednesday" )
                           ( dt::Thursday, "Thursday" )( dt::Friday, "Friday" )( dt::Saturday, "Saturday" )
                           ( dt::Sunday, "Sunday" );
                month.add( dt::Jan, "Jan" )( dt::Feb, "Feb" )( dt::Mar, "Mar" )( dt::Apr, "Apr" )
                         ( dt::May, "May" )( dt::Jun, "Jun" )( dt::Jul, "Jul" )( dt::Aug, "Aug" )
                         ( dt::Sep, "Sep" )( dt::Oct, "Oct" )( dt::Nov, "Nov" )( dt::Dec, "Dec" );
              }
            private:
              boost::spirit::karma::uint_generator<unsigned int, 10> dec_p;
              boost::spirit::karma::rule< InputIterator, boost::posix_time::ptime() > root;
              boost::spirit::karma::rule< InputIterator, boost::gregorian::date() > date;
              boost::spirit::karma::rule< InputIterator, boost::posix_time::time_duration() > time;
              boost::spirit::karma::rule< InputIterator, boost::posix_time::time_duration() > zone;
              boost::spirit::karma::rule< InputIterator, boost::posix_time::time_duration() > zone_pos;
              boost::spirit::karma::rule< InputIterator, boost::posix_time::time_duration() > zone_neg;
              boost::spirit::karma::symbols< boost::date_time::weekdays, char > weekday;
              boost::spirit::karma::symbols< boost::posix_time::time_duration, char > zone_name; 
              boost::spirit::karma::symbols< boost::date_time::months_of_year, char > month;
      };
    }
  }
}

#endif

