#ifndef HERMIT_STREAM_BASE_NATIVE_PACK_OPERATOR_HPP
#define HERMIT_STREAM_BASE_NATIVE_PACK_OPERATOR_HPP

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/combine.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/algorithm.hpp>

#include <hermit/stream/native_pack.hpp>
#include <hermit/operator_traits.hpp>

namespace hermit {
  namespace stream {
#define HPP_STREAM_BASE_1OP_PRE( tag_name, oper ) \
    namespace native { \
      template< \
        typename source_element, size_t source_size \
      > \
      struct tag_name { \
        typedef \
        native_pack< \
          decltype( oper std::declval< source_element >() ), \
          source_size \
        > \
        dest_type; \
        typedef \
        native_pack< \
          source_element, \
          source_size \
        > \
        source_type; \
        static dest_type &run( \
          dest_type &dest, \
          const source_type &source \
        ) { \
          dest_type temp; \
          boost::transform( \
            source, \
            dest.begin(), \
            []( const source_element &value ) { \
              return oper value; \
            } \
          ); \
          return dest; \
        } \
      }; \
    } \
    template< \
      typename source_element, size_t source_size \
    > \
    typename native::tag_name < \
      source_element, source_size \
    >::dest_type \
    operator oper ( \
      const native_pack< source_element, source_size > &left \
    ) { \
      typename native::tag_name < \
        source_element, source_size \
      >::dest_type dest; \
      return native::tag_name < \
        source_element, source_size \
      >::run( dest, left ); \
    }

#define HPP_STREAM_BASE_1OP_PRE_ASSIGN( tag_name, oper ) \
    namespace native { \
      template< \
        typename source_element, size_t source_size \
      > \
      struct tag_name { \
        typedef \
        native_pack< \
          decltype( oper std::declval< source_element >() ), \
          source_size \
        > \
        dest_type; \
        typedef \
        native_pack< \
          source_element, \
          source_size \
        > \
        source_type; \
        static dest_type &run( \
          dest_type &dest, \
          source_type &source \
        ) { \
          for( size_t index = 0; index != source_size; ++index ) \
            dest[ index ] = oper source[ index ]; \
          return dest; \
        } \
      }; \
    } \
    template< \
      typename source_element, size_t source_size \
    > \
    typename native::tag_name < \
      source_element, source_size \
    >::dest_type \
    operator oper ( \
      native_pack< source_element, source_size > &left \
    ) { \
      typename native::tag_name < \
        source_element, source_size \
      >::dest_type dest; \
      return native::tag_name < \
        source_element, source_size \
      >::run( dest, left ); \
    }

#define HPP_STREAM_BASE_1OP_POST( tag_name, oper ) \
    namespace native { \
      template< \
        typename source_element, size_t source_size \
      > \
      struct tag_name { \
        typedef \
        native_pack< \
          decltype( oper std::declval< source_element >() ), \
          source_size \
        > \
        dest_type; \
        typedef \
        native_pack< \
          source_element, \
          source_size \
        > \
        source_type; \
        static dest_type &run( \
          dest_type &dest, \
          const source_type &source \
        ) { \
          dest_type temp; \
          boost::transform( \
            source, \
            dest.begin(), \
            []( const source_element &value ) { \
              return value oper; \
            } \
          ); \
          return dest; \
        } \
      }; \
    } \
    template< \
      typename source_element, size_t source_size \
    > \
    typename native::tag_name < \
      source_element, source_size \
    >::dest_type \
    operator oper ( \
      const native_pack< source_element, source_size > &left, \
      int \
    ) { \
      typename native::tag_name < \
        source_element, source_size \
      >::dest_type dest; \
      return native::tag_name < \
        source_element, source_size \
      >::run( dest, left ); \
    }

#define HPP_STREAM_BASE_1OP_POST_ASSIGN( tag_name, oper ) \
    namespace native { \
      template< \
        typename source_element, size_t source_size \
      > \
      struct tag_name { \
        typedef \
        native_pack< \
          decltype( oper std::declval< source_element >() ), \
          source_size \
        > \
        dest_type; \
        typedef \
        native_pack< \
          source_element, \
          source_size \
        > \
        source_type; \
        static dest_type &run( \
          dest_type &dest, \
          source_type &source \
        ) { \
          for( size_t index = 0; index != source_size; ++index ) \
            dest[ index ] = source[ index ] oper; \
          return dest; \
        } \
      }; \
    } \
    template< \
      typename source_element, size_t source_size \
    > \
    typename native::tag_name < \
      source_element, source_size \
    >::dest_type \
    operator oper ( \
      native_pack< source_element, source_size > &left, \
      int \
    ) { \
      typename native::tag_name < \
        source_element, source_size \
      >::dest_type dest; \
      return native::tag_name < \
        source_element, source_size \
      >::run( dest, left ); \
    }

#define HPP_STREAM_BASE_2OP( tag_name, oper ) \
    namespace native { \
      template< \
        typename LeftType, size_t left_size, \
        typename RightType, size_t right_size \
      > \
      struct tag_name { \
        template< \
          size_t left_size_ = left_size, \
          size_t right_size_ = right_size \
        > \
        static native_pack< \
          decltype( std::declval< LeftType >() oper std::declval< RightType >() ), \
          left_size \
        > \
        run( \
          const native_pack< LeftType, left_size > &left, \
          const native_pack< RightType, right_size > &right, \
          typename boost::enable_if< boost::mpl::greater< boost::mpl::size_t< left_size_ >, boost::mpl::size_t< right_size_ > > >::type* = 0 \
        ) { \
          native_pack< RightType, left_size > temp( right ); \
          return left oper temp; \
        } \
        template< \
          size_t left_size_ = left_size, \
          size_t right_size_ = right_size \
        > \
        static native_pack< \
          decltype( std::declval< LeftType >() oper std::declval< RightType >() ), \
          right_size \
        > \
        run( \
          const native_pack< LeftType, left_size > &left, \
          const native_pack< RightType, right_size > &right, \
          typename boost::enable_if< boost::mpl::less< boost::mpl::size_t< left_size_ >, boost::mpl::size_t< right_size_ > > >::type* = 0 \
        ) { \
          native_pack< LeftType, right_size > temp( left ); \
          return temp oper right; \
        } \
        template< \
          size_t left_size_ = left_size, \
          size_t right_size_ = right_size \
        > \
        static native_pack< \
          decltype( std::declval< LeftType >() oper std::declval< RightType >() ), \
          right_size \
        > \
        run( \
          const native_pack< LeftType, left_size > &left, \
          const native_pack< RightType, right_size > &right, \
          typename boost::enable_if< boost::mpl::equal_to< boost::mpl::size_t< left_size_ >, boost::mpl::size_t< right_size_ > > >::type* = 0 \
        ) { \
          native_pack< \
            decltype( std::declval< LeftType >() oper std::declval< RightType >() ), \
            right_size \
          > temp; \
          boost::transform( \
            boost::combine( left, right ), \
            temp.begin(), \
            []( const boost::tuple< LeftType, RightType > &value ) { \
              return boost::fusion::at_c< 0 >( value ) oper boost::fusion::at_c< 1 >( value ); \
            } \
          ); \
          return temp; \
        } \
      }; \
    } \
    template< \
      typename LeftType, size_t left_size, \
      typename RightType, size_t right_size \
    > \
    decltype( \
      native::tag_name < \
        LeftType, left_size, \
        RightType, right_size \
      >::run( \
        std::declval< native_pack< LeftType, left_size > >(), \
        std::declval< native_pack< RightType, right_size > >() \
      ) \
    ) \
    operator oper ( \
      const native_pack< LeftType, left_size > &left, \
      const native_pack< RightType, right_size > &right \
    ) { \
      return native::tag_name < \
        LeftType, left_size, \
        RightType, right_size \
      >::run( left, right ); \
    }

HPP_STREAM_BASE_1OP_PRE( unary_plus, + )
HPP_STREAM_BASE_1OP_PRE_ASSIGN( pre_inc, ++ )
HPP_STREAM_BASE_1OP_POST_ASSIGN( post_inc, ++ )
HPP_STREAM_BASE_2OP( shift_left, << )
HPP_STREAM_BASE_2OP( shift_right, >> )
HPP_STREAM_BASE_2OP( multiplies, * )
HPP_STREAM_BASE_2OP( divides, / )
HPP_STREAM_BASE_2OP( modulus, % )
HPP_STREAM_BASE_2OP( plus, + )
HPP_STREAM_BASE_2OP( minus, - )
  
  }
}

#endif
