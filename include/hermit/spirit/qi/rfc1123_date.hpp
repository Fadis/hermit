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
                root = ( wkday >> ',' >> qi::omit[ +qi::standard::space ] >>
                         date >> qi::omit[ +qi::standard::space ] >> time >> qi::omit[ +qi::standard::space ] >>
                         zone )[
                  qi::_pass = qi::_1 == phx::bind( &boost::gregorian::date::day_of_week, &qi::_2 ),
                  qi::_val = phx::construct< boost::posix_time::ptime >( qi::_2,
                    qi::_3 + qi::_4
                  )
                ];
                date = ( dec2_p >> qi::omit[ +qi::standard::space ] >> month >> qi::omit[ +qi::standard::space ] >> dec4_p )[
                  qi::_pass = qi::_1 <= phx::bind( boost::gregorian::gregorian_calendar::end_of_month_day, qi::_3, qi::_2 ),
                  qi::_val = phx::construct< boost::gregorian::date >( qi::_3, qi::_2, qi::_1 )
                ];
                time = ( dec2_p >> ':' >> dec2_p >> ':' >> dec2_p )[
                  qi::_pass = qi::_1 < 24u && qi::_2 < 60u && qi::_3 < 60u,
                  qi::_val = phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, qi::_3 )
                ];
                wkday.add( "Mon", dt::Monday )( "Tue", dt::Tuesday )( "Wed", dt::Wednesday )( "Thu", dt::Thursday )
                         ( "Fri", dt::Friday )( "Sat", dt::Saturday )( "Sun", dt::Sunday );
                month.add( "Jan", dt::Jan )( "Feb", dt::Feb )( "Mar", dt::Mar )( "Apr", dt::Apr )
                         ( "May", dt::May )( "Jun", dt::Jun )( "Jul", dt::Jul )( "Aug", dt::Aug )
                         ( "Sep", dt::Sep )( "Oct", dt::Oct )( "Nov", dt::Nov )( "Dec", dt::Dec );
                zone_name.add( "UT", boost::posix_time::time_duration( 0, 0, 0 ) )
                             ( "GMT", boost::posix_time::time_duration( 0, 0, 0 ) )
                             ( "EST", boost::posix_time::time_duration( -5, 0, 0 ) )
                             ( "EDT", boost::posix_time::time_duration( -4, 0, 0 ) )
                             ( "CST", boost::posix_time::time_duration( -6, 0, 0 ) )
                             ( "CDT", boost::posix_time::time_duration( -5, 0, 0 ) )
                             ( "MST", boost::posix_time::time_duration( -7, 0, 0 ) )
                             ( "MDT", boost::posix_time::time_duration( -6, 0, 0 ) )
                             ( "PST", boost::posix_time::time_duration( -8, 0, 0 ) )
                             ( "PDT", boost::posix_time::time_duration( -7, 0, 0 ) )
                             ( "Z", boost::posix_time::time_duration( 0, 0, 0 ) )
                             ( "A", boost::posix_time::time_duration( -1, 0, 0 ) )
                             ( "B", boost::posix_time::time_duration( -2, 0, 0 ) )
                             ( "C", boost::posix_time::time_duration( -3, 0, 0 ) )
                             ( "D", boost::posix_time::time_duration( -4, 0, 0 ) )
                             ( "E", boost::posix_time::time_duration( -5, 0, 0 ) )
                             ( "F", boost::posix_time::time_duration( -6, 0, 0 ) )
                             ( "G", boost::posix_time::time_duration( -7, 0, 0 ) )
                             ( "H", boost::posix_time::time_duration( -8, 0, 0 ) )
                             ( "I", boost::posix_time::time_duration( -9, 0, 0 ) )
                             ( "K", boost::posix_time::time_duration( -10, 0, 0 ) )
                             ( "L", boost::posix_time::time_duration( -11, 0, 0 ) )
                             ( "M", boost::posix_time::time_duration( -12, 0, 0 ) )
                             ( "N", boost::posix_time::time_duration( 1, 0, 0 ) )
                             ( "O", boost::posix_time::time_duration( 2, 0, 0 ) )
                             ( "P", boost::posix_time::time_duration( 3, 0, 0 ) )
                             ( "Q", boost::posix_time::time_duration( 4, 0, 0 ) )
                             ( "R", boost::posix_time::time_duration( 5, 0, 0 ) )
                             ( "S", boost::posix_time::time_duration( 6, 0, 0 ) )
                             ( "T", boost::posix_time::time_duration( 7, 0, 0 ) )
                             ( "U", boost::posix_time::time_duration( 8, 0, 0 ) )
                             ( "V", boost::posix_time::time_duration( 9, 0, 0 ) )
                             ( "W", boost::posix_time::time_duration( 10, 0, 0 ) )
                             ( "X", boost::posix_time::time_duration( 11, 0, 0 ) )
                             ( "Y", boost::posix_time::time_duration( 12, 0, 0 ) );
                zone_pos = ( '+' >> qi::omit[ *qi::standard::space ] >> dece2_p >> dece2_p )[
                  qi::_pass = qi::_2 < 60u,
                  qi::_val = phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, 0 )
                ];
                zone_neg = ( '-' >> qi::omit[ *qi::standard::space ] >> dece2_p >> dece2_p )[
                  qi::_pass = qi::_2 < 60u,
                  qi::_val = -phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, 0 )
                ];
                zone = zone_name|zone_pos|zone_neg;
              }
            private:
              boost::spirit::qi::uint_parser<unsigned int, 10, 2, 2> dece2_p;
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 2> dec2_p;
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 4> dec4_p;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::ptime() > root;
              boost::spirit::qi::rule< InputIterator, boost::gregorian::date() > date;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > time;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > zone;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > zone_pos;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > zone_neg;
              boost::spirit::qi::symbols< char, boost::date_time::weekdays > wkday;
              boost::spirit::qi::symbols< char, boost::posix_time::time_duration > zone_name; 
              boost::spirit::qi::symbols< char, boost::date_time::months_of_year > month;
      };
    }
  }
}

#endif

