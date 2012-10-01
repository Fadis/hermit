#ifndef HERMIT_FORMAT_CONVERT_JSON_TO_MICROXML_HPP
#define HERMIT_FORMAT_CONVERT_JSON_TO_MICROXML_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <hermit/json.hpp>
#include <hermit/microxml.hpp>
#include <boost/variant.hpp>
namespace hermit {
  namespace format {
    namespace convert {
      struct json2microxml : public boost::static_visitor<hermit::microxml> {
	hermit::microxml operator()( const std::u32string& ) const {
          throw boost::mpl::int_<1>();
        }
	hermit::microxml operator()( double ) const {
          throw boost::mpl::int_<2>();
        }
	hermit::microxml operator()( const std::map< std::u32string, hermit::json > & ) const {
          throw boost::mpl::int_<3>();
        }
	hermit::microxml operator()( const std::vector< hermit::json > &src ) const {
          if( src.size() < 2 ) {
            throw boost::mpl::int_<3>();
          }
          std::vector< hermit::json >::const_iterator iter = src.begin();
          if( iter->which() != 0 ) {
            throw boost::mpl::int_<4>();
          }
          hermit::microxml temp;
          temp.tag = boost::get< std::u32string >( *iter );
          ++iter;
          if( iter->which() != 2 ) {
            throw boost::mpl::int_<5>();
          }
          const std::map< std::u32string, hermit::json > &attrs = boost::get< std::map< std::u32string, hermit::json > >( *iter );
          for( const auto &elem : attrs ) {
            if( elem.second.which() != 0 ) {
              throw boost::mpl::int_<6>();
            }
            temp.attribute[ elem.first ] = boost::get< std::u32string >( elem.second );
          }
          for( ++iter; iter != src.end(); ++iter ) {
            if( iter->which() == 3 )
              temp.child.push_back( apply_visitor( json2microxml(), *iter ) );
            else if( iter->which() == 0 )
              temp.child.push_back( boost::get< std::u32string >( *iter ) );
            else
              throw boost::mpl::int_<7>();
          }
          return temp;
        }
	hermit::microxml operator()( bool ) const {
          throw boost::mpl::int_<8>();
        }
	hermit::microxml operator()( const hermit::none_type & ) const {
          throw boost::mpl::int_<9>();
        }
      };
    }
    hermit::microxml convert_json_to_microxml( const hermit::json &src ) {
      return apply_visitor( convert::json2microxml(), src );
    }
  }
}

#endif

