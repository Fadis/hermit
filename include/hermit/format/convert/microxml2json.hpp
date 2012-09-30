
#include <string>
#include <vector>
#include <hermit/json.hpp>
#include <hermit/microxml.hpp>
#include <boost/variant.hpp>
#include <hermit/format/write/utf8.hpp>

namespace hermit {
  namespace format {
    namespace convert {
      struct microxml2json : public boost::static_visitor<hermit::json> {
        hermit::json operator()( const boost::recursive_wrapper< hermit::microxml > &src ) const {
          std::vector< hermit::json > temp;
          temp.push_back( src.get().tag );
          std::map< std::u32string, hermit::json > attrs;
          for( const auto elem : src.get().attribute )
            attrs[ elem.first ] = elem.second;
          temp.push_back( attrs );
          for( const auto elem : src.get().child )
            temp.push_back( boost::apply_visitor( microxml2json(), elem ) );
          return temp;
        }
        hermit::json operator()( const std::u32string &src ) const {
          return src;
        }
      };
    }
    hermit::json convert_microxml_to_json( const hermit::microxml &src ) {
      std::vector< hermit::json > temp;
      temp.push_back( src.tag );
      std::map< std::u32string, hermit::json > attrs;
      for( const auto elem : src.attribute )
        attrs[ elem.first ] = elem.second;
      temp.push_back( attrs );
      for( const auto elem : src.child )
        temp.push_back( boost::apply_visitor( convert::microxml2json(), elem ) );
      return temp;
    }
  }
}

