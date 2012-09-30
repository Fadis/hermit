#ifndef HERMIT_FORMAT_CONVERT_MESSAGEPACK_TO_JSON_HPP
#define HERMIT_FORMAT_CONVERT_MESSAGEPACK_TO_JSON_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <hermit/json.hpp>
#include <hermit/messagepack.hpp>
#include <boost/variant.hpp>
#include <hermit/format/read/utf32.hpp>

namespace hermit {
  namespace format {
    namespace convert {
      struct messagepack2json : public boost::static_visitor<hermit::json> {
	hermit::json operator()( const std::vector< uint8_t > &src ) const {
          const auto str = hermit::format::read_utf32be( src );
          if( !str ) {
            return "";
          }
          return *str;
        }
	hermit::json operator()( uint8_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( int8_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( uint16_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( int16_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( uint32_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( int32_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( uint64_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( int64_t src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( float src ) const {
          return static_cast< double >( src );
        }
	hermit::json operator()( double src ) const {
          return src;
        }
	hermit::json operator()( const std::map< hermit::messagepack, hermit::messagepack > &src ) const {
          std::map< std::u32string, hermit::json > temp;
          std::transform( src.begin(), src.end(), std::inserter( temp, temp.end() ),
            []( const std::pair< hermit::messagepack, hermit::messagepack > &elem ) -> std::pair< std::u32string, hermit::json > {
              const auto name = hermit::format::read_utf32be( boost::get< std::vector< uint8_t > >( elem.first ) );
              if( !name ) {
                return std::make_pair( std::u32string(), apply_visitor( messagepack2json(), elem.second ) );
              }
              return std::make_pair( *name, apply_visitor( messagepack2json(), elem.second ) );
            }
          );
          return temp;
        }
	hermit::json operator()( const std::vector< hermit::messagepack > &src ) const {
          std::vector< hermit::json > temp;
          std::transform( src.begin(), src.end(), std::back_inserter( temp ),
            []( const hermit::messagepack &elem ) {
              return apply_visitor( messagepack2json(), elem );
            }
          );
          return temp;
        }
	hermit::json operator()( bool src ) const {
          return src;
        }
	hermit::json operator()( const hermit::none_type &src ) const {
          return src;
        }
      };
    }
    hermit::json convert_messagepack_to_json( const hermit::messagepack &src ) {
      return apply_visitor( convert::messagepack2json(), src );
    }
  }
}

#endif

