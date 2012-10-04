#ifndef HERMIT_SPIRIT_QI_ASCTIME_HPP
#define HERMIT_SPIRIT_QI_ASCTIME_HPP

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
        class asctime :
          public boost::spirit::qi::grammar< InputIterator, boost::posix_time::ptime() > {
            public:
              asctime() : asctime::base_type( root ) {
                namespace qi = boost::spirit::qi;
                namespace dt = boost::date_time;
                namespace phx = boost::phoenix;
                root = ( wkday >> qi::omit[ +qi::standard::space ] >> date )[
                  qi::_pass = qi::_1 == phx::bind( &boost::gregorian::date::day_of_week, &phx::at_c< 0 >( qi::_2 ) ),
                  qi::_val = phx::construct< boost::posix_time::ptime >(
                    phx::at_c< 0 >( qi::_2 ), phx::at_c< 1 >( qi::_2 )
                  )
                ];
                date = ( month >> qi::omit[ +qi::standard::space ] >> dec2_p >> qi::omit[ +qi::standard::space ] >>
                         time >> qi::omit[ +qi::standard::space ] >> dec4_p )[
                  qi::_pass = qi::_2 <= phx::bind( boost::gregorian::gregorian_calendar::end_of_month_day, qi::_4, qi::_2 ),
                  phx::at_c< 0 >( qi::_val ) = phx::construct< boost::gregorian::date >( qi::_4, qi::_1, qi::_2 ),
                  phx::at_c< 1 >( qi::_val ) = qi::_3
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
              }
            private:
              boost::spirit::qi::uint_parser<unsigned int, 10, 2, 2> dece2_p;
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 2> dec2_p;
              boost::spirit::qi::uint_parser<unsigned int, 10, 1, 4> dec4_p;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::ptime() > root;
              boost::spirit::qi::rule< InputIterator, boost::fusion::vector< boost::gregorian::date, boost::posix_time::time_duration >() > date;
              boost::spirit::qi::rule< InputIterator, boost::posix_time::time_duration() > time;
              boost::spirit::qi::symbols< char, boost::date_time::weekdays > wkday;
              boost::spirit::qi::symbols< char, boost::posix_time::time_duration > zone_name; 
              boost::spirit::qi::symbols< char, boost::date_time::months_of_year > month;
      };
    }
  }
}

#endif

