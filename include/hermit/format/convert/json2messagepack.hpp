
#include <string>
#include <vector>
#include <cstdint>
#include <hermit/json.hpp>
#include <hermit/messagepack.hpp>
#include <boost/variant.hpp>

namespace hermit {
  namespace format {
    namespace convert {
      struct json2messagepack : public boost::static_visitor<hermit::messagepack> {
	hermit::messagepack operator()( const std::string& src ) const {
          std::vector< uint8_t > temp( src.begin(), src.end() );
          return temp;
        }
	hermit::messagepack operator()( double src ) const {
          if( static_cast<uint8_t>( src ) == src )
            return static_cast<uint8_t>( src );
          else if( static_cast<int8_t>( src ) == src )
            return static_cast<int8_t>( src );
          else if( static_cast<uint16_t>( src ) == src )
            return static_cast<uint16_t>( src );
          else if( static_cast<int16_t>( src ) == src )
            return static_cast<int16_t>( src );
          else if( static_cast<uint32_t>( src ) == src )
            return static_cast<uint32_t>( src );
          else if( static_cast<int32_t>( src ) == src )
            return static_cast<int32_t>( src );
          else if( static_cast<uint64_t>( src ) == src )
            return static_cast<uint64_t>( src );
          else if( static_cast<int64_t>( src ) == src )
            return static_cast<int64_t>( src );
          else if( static_cast<float>( src ) == src )
            return static_cast<float>( src );
          else
            return src;
        }
	hermit::messagepack operator()( const std::map< std::string, hermit::json > &src ) const {
          std::map< hermit::messagepack, hermit::messagepack > temp;
          std::transform( src.begin(), src.end(), std::inserter( temp, temp.end() ),
            []( const std::pair< std::string, hermit::json > &elem ) {
              return std::make_pair( hermit::messagepack( std::vector< uint8_t >( elem.first.begin(), elem.first.end() ) ), apply_visitor( json2messagepack(), elem.second ) );
            }
          );
          return temp;
        }
	hermit::messagepack operator()( const std::vector< hermit::json > &src ) const {
          std::vector< hermit::messagepack > temp;
          std::transform( src.begin(), src.end(), std::back_inserter( temp ),
            []( const hermit::json &elem ) {
              return apply_visitor( json2messagepack(), elem );
            }
          );
          return temp;
        }
	hermit::messagepack operator()( bool src ) const {
          return src;
        }
	hermit::messagepack operator()( const hermit::none_type &src ) const {
          return src;
        }
      };
    }
  }
}

