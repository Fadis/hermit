#ifndef HERMIT_SPIRIT_QI_RFC850_DATE_HPP
#define HERMIT_SPIRIT_QI_RFC850_DATE_HPP

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
        class rfc850_date :
          public boost::spirit::qi::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              rfc850_date() : rfc850_date::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace dt = boost::date_time;
                namespace phx = boost::phoenix;
                root = ( weekday >> ',' >> qi::omit[ +qi::standard::space ] >>
                         date >> qi::omit[ +qi::standard::space ] >> time >> qi::omit[ +qi::standard::space ] >>
                         zone )[
                  qi::_pass = qi::_1 == phx::bind( &boost::gregorian::date::day_of_week, &qi::_2 ),
                  qi::_val = phx::construct< boost::posix_time::ptime >( qi::_2,
                    qi::_3 + qi::_4
                  )
                ];
                date = ( dec2_p >> '-' >> month >> '-' >> dec2_p )[
                  qi::_pass = qi::_1 <= phx::bind( boost::gregorian::gregorian_calendar::end_of_month_day, qi::_3 + 1900, qi::_2 ),
                  qi::_val = phx::construct< boost::gregorian::date >( qi::_3 + 1900, qi::_2, qi::_1 )
                ];
                time = ( dec2_p >> ':' >> dec2_p >> ':' >> dec2_p )[
                  qi::_pass = qi::_1 < 24u && qi::_2 < 60u && qi::_3 < 60u,
                  qi::_val = phx::construct< boost::posix_time::time_duration >( qi::_1, qi::_2, qi::_3 )
                ];
                weekday.add( "Monday", dt::Monday )( "Tuesday", dt::Tuesday )( "Wednesday", dt::Wednesday )
                           ( "Thursday", dt::Thursday )( "Friday", dt::Friday )( "Saturday", dt::Saturday )
                           ( "Sunday", dt::Sunday );
                month.add( "Jan", dt::Jan )( "Feb", dt::Feb )( "Mar", dt::Mar )( "Apr", dt::Apr )
                         ( "May", dt::May )( "Jun", dt::Jun )( "Jul", dt::Jul )( "Aug", dt::Aug )
                         ( "Sep", dt::Sep )( "Oct", dt::Oct )( "Nov", dt::Nov )( "Dec", dt::Dec );
                zone_name.add( "GMT", boost::posix_time::time_duration( 0, 0, 0 ) );
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
              boost::spirit::qi::symbols< char, boost::date_time::weekdays > weekday;
              boost::spirit::qi::symbols< char, boost::posix_time::time_duration > zone_name; 
              boost::spirit::qi::symbols< char, boost::date_time::months_of_year > month;
      };
    }
  }
}

#endif

