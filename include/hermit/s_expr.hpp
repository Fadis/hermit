#ifndef HERMIT_S_EXPR_HPP
#define HERMIT_S_EXPR_HPP

#include <string>
#include <cstdint>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/range.hpp>
#include <boost/optional.hpp>

#include <hermit/range_traits.hpp>

namespace hermit {
  namespace s_expr {
    typedef int64_t integer;
    typedef std::string string;
 
    struct symbol {
    public:
      symbol(){}
      symbol( const std::string &name_ ) : name( name_ ) {}
      const std::string &get_name() const { return name; }
    private:
      std::string name;
    };
 
    typedef boost::make_recursive_variant<
      std::vector< boost::recursive_variant_ >,
      integer,
      string,
      symbol
    >::type node_internal;
 
    typedef std::vector< node_internal > node;
 
    template< typename Iterator >
    class rule :
      public boost::spirit::qi::grammar<
        Iterator,
        node()
      > {
    public:
      rule() : rule::base_type( root ) {
        using namespace boost::spirit;
        using namespace boost::spirit::ascii;
        root = *space >> list >> *space;
        list =
          ( omit[ '(' >> *space ] >> ( element >> *( omit[ +space ] >> element ) ) >> omit[ *space >> ')' ] ) |
          ( omit[ '[' >> *space ] >> ( element >> *( omit[ +space ] >> element ) ) >> omit[ *space >> ']' ] ) |
          ( omit[ '{' >> *space ] >> ( element >> *( omit[ +space ] >> element ) ) >> omit[ *space >> '}' ] );
        element = list|integer_|string_|symbol_;
        integer_ = long_long;
        escape = '\\' >> char_;
        reserved = char_( "()[]{}\\\"'" );
        string_ =
          ( '"' >> *( ( char_ - reserved )|escape ) >> '"' )|
          ( '\'' >> *( ( char_ - reserved )|escape ) >> '\'' );
        symbol_ = as_string[ alpha >> *( ( graph - reserved )|escape ) ];
      } 
    private:
      boost::spirit::qi::rule< Iterator, node() > root;
      boost::spirit::qi::rule< Iterator, node() > list;
      boost::spirit::qi::rule< Iterator, node_internal() > element;
      boost::spirit::qi::rule< Iterator, integer() > integer_;
      boost::spirit::qi::rule< Iterator, char() > escape;
      boost::spirit::qi::rule< Iterator, char() > reserved;
      boost::spirit::qi::rule< Iterator, string() > string_;
      boost::spirit::qi::rule< Iterator, symbol() > symbol_;
    };

    template<
      typename T,
      typename Concept = typename boost::enable_if<
        boost::mpl::and_<
          boost::is_convertible<
            typename range_value< const T >::type,
            const char
          >,
          is_forward_traversal_range< const T >,
          is_readable_range< const T >
        >
      >::type
    >
    boost::optional< node > parse(
      const T &str
    ) {
      rule< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      node result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< node >();
    }
  }
}

#endif

