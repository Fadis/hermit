#ifndef HERMIT_JSON_HPP
#define HERMIT_JSON_HPP

#include <utility>
#include <cstddef>
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <hermit/range_traits.hpp>

namespace hermit {
  namespace json {
    boost::spirit::qi::uint_parser<uint8_t, 16, 1, 2> hex2_p;
    typedef std::string string;
    typedef double number;
    
    typedef boost::make_recursive_variant<
      string,
      number,
      std::map< std::string, boost::recursive_variant_ >,
      std::vector< boost::recursive_variant_ >,
      bool,
      std::nullptr_t
    >::type value;

    template< typename Iterator >
    class rule :
      public boost::spirit::qi::grammar<
        Iterator,
        value()
      > {
    public:
      rule() : rule::base_type( root ) {
        using namespace boost::spirit;
        using namespace boost::spirit::ascii;
        null_ = lit( "null" )[ _val = nullptr ];
        root = object|array;
        value_ = string_|double_|object|array|bool_|null_;
        escape_sequence = '\\' >> (
          char_( '"' )[ _val = '"' ] |
          char_( '\\' )[ _val = '\\' ] |
          char_( '/' )[ _val = '/' ] |
          char_( 'b' )[ _val = '\b' ] |
          char_( 'n' )[ _val = '\n' ] |
          char_( 'f' )[ _val = '\f' ] |
          char_( 'r' )[ _val = '\r' ] |
          char_( 't' )[ _val = '\t' ] |
          hex2_p[ _val = _1 ]
        );
        string_ = '"' >> *( ( char_ - cntrl - '\\' - '"' )|escape_sequence ) >> '"';
        array = skip( blank )[ '[' >> -( value_ % ',' ) >>  ']' ];
        named_value = skip( blank )[ ( string_ >> ':' >> value_ ) ];
        object = skip( blank )[ '{' >> -( named_value % ',' ) >> '}' ];
      } 
    private:
      boost::spirit::qi::rule< Iterator, value() > root;
      boost::spirit::qi::rule< Iterator, char() > escape_sequence;
      boost::spirit::qi::rule< Iterator, string() > string_;
      boost::spirit::qi::rule< Iterator, std::nullptr_t() > null_;
      boost::spirit::qi::rule< Iterator, value() > value_;
      boost::spirit::qi::rule< Iterator, std::vector< value >() > array;
      boost::spirit::qi::rule< Iterator, std::pair< string, value >() > named_value;
      boost::spirit::qi::rule< Iterator, std::map< string, value >() > object;

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
    boost::optional< value > parse(
      const T &str
    ) {
      rule< typename range_iterator< const T >::type > grammar;
      typename range_iterator< const T >::type iter = str.begin();
      value result;
      if( boost::spirit::qi::parse(iter, str.end(), grammar, result ) && iter == str.end() )
        return result;
      else
        return boost::optional< value >();
    }

    typedef boost::make_recursive_variant<
      string,
      double
    >::type value2;
    
    template <typename Iterator>
    class gen :
      public boost::spirit::karma::grammar< Iterator, value() > {
      public:
        gen() : gen::base_type( value_ ) {
          using namespace boost::spirit;
          null_ = karma::string[ _1 = boost::phoenix::val( "null" ) ];
          root = object|array;
          value_ = string_|karma::double_|object|array|karma::bool_|null_;
          escape_sequence.add( '"', "\\\"" )( '\\', "\\\\" )( '/', "\\/" )
              ( '\b', "\\b" )( '\n', "\\n" )( '\f', "\\f" )
              ( '\r', "\\r" )( '\t', "\\t" );
          string_ = '"' << *( escape_sequence | karma::print | "\\" << karma::hex ) << '"';
          array = '[' << -( value_ % ',' ) << ']';
          named_value = ( string_ << ':' << value_ );
          object = '{' << -( named_value % ',' ) << '}';
        }
      private:
        boost::spirit::karma::rule< Iterator, value() > root;
        boost::spirit::karma::symbols<char, std::string> escape_sequence;
        boost::spirit::karma::rule<Iterator, std::string()> string_;
        boost::spirit::karma::rule< Iterator, std::nullptr_t> null_;
        boost::spirit::karma::rule< Iterator, value() > value_;
        boost::spirit::karma::rule< Iterator, std::vector< value >() > array;
        boost::spirit::karma::rule< Iterator, std::pair< string, value >() > named_value;
        boost::spirit::karma::rule< Iterator, std::map< string, value >() > object;
    };

    boost::optional< std::string > generate(
      const value &source
    ) {
      std::string result;
      hermit::json::gen< std::back_insert_iterator< std::string > > gen;
      if( boost::spirit::karma::generate(
        std::back_inserter( result ),
        gen,
        source
      ) )
        return result;
      else
        return boost::optional< std::string >();
    }


  }
}

#endif

