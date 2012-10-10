#ifndef HERMIT_SPIRIT_KARMA_RFC1123_DATE_HPP
#define HERMIT_SPIRIT_KARMA_RFC1123_DATE_HPP

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
        class rfc1123_date :
          public boost::spirit::karma::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              rfc1123_date() : rfc1123_date::base_type( root ) {
                namespace karma = boost::spirit::karma;
                namespace dt = boost::date_time;
                namespace phx = boost::phoenix;
                root = ( wkday << ", " << date << ' ' << time << " GMT" )[
                  karma::_2 = phx::bind( &boost::posix_time::ptime::date, &karma::_val ),
                  karma::_1 = phx::bind( &boost::gregorian::greg_weekday::as_enum,
                    phx::bind( &boost::gregorian::date::day_of_week, &karma::_2 ) ),
                  karma::_3 = phx::bind( &boost::posix_time::ptime::time_of_day, &karma::_val )
                ];
                date = ( -karma::char_ << dec_p << ' ' << month << ' ' << dec_p )[
                  karma::_2 = phx::bind( &boost::gregorian::date::day, &karma::_val ),
                  phx::if_( karma::_2 < 10ul )[
                    karma::_1 = '0'
                  ],
                  karma::_3 = phx::bind( &boost::gregorian::greg_month::as_enum,
                    phx::bind( &boost::gregorian::date::month, &karma::_val ) ),
                  karma::_4 = phx::bind( &boost::gregorian::date::year, &karma::_val )
                ];
                time = ( dec_p << ':' << dec_p << ':' << dec_p )[
                  karma::_1 = phx::bind( &boost::posix_time::time_duration::hours, &karma::_val ),
                  karma::_2 = phx::bind( &boost::posix_time::time_duration::minutes, &karma::_val ),
                  karma::_3 = phx::bind( &boost::posix_time::time_duration::seconds, &karma::_val )
                ];
                wkday.add( dt::Monday, "Mon" )( dt::Tuesday, "Tue" )( dt::Wednesday, "Wed" )
                           ( dt::Thursday, "Thu" )( dt::Friday, "Fri" )( dt::Saturday, "Sat" )
                           ( dt::Sunday, "Sun" );
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
              boost::spirit::karma::symbols< boost::date_time::weekdays, std::string > wkday;
              boost::spirit::karma::symbols< boost::date_time::months_of_year, std::string > month;
      };
    }
  }
}

#endif

