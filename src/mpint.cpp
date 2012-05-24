/***************************************************************************
 * Copyright (C) 2012, Naomasa Matsubayashi
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#include <cstdint>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#include <tommath.h>

#include <hermit/mpint.hpp>

namespace hermit {
  boost::optional< std::string > is_hex( const std::string &temp ) {
    using boost::spirit::qi::parse;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::digit;
    auto iter = temp.begin();
    std::vector< char > result;
    if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> "0x" >> +( digit|char_( "a-fA-F" ) ), result ) && iter == temp.end() )
      return std::string( result.begin(), result.end() );
    return boost::optional< std::string >();
  }
  boost::optional< std::string > is_dec( const std::string temp ) {
    using boost::spirit::qi::parse;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::digit;
    auto iter = temp.begin();
    std::vector< char > result;
    if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> +digit, result ) && iter == temp.end() ) {
      return std::string( result.begin(), result.end() );
    }
    return boost::optional< std::string >();

  }
  boost::optional< std::string > is_oct( const std::string temp ) {
    using boost::spirit::qi::parse;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::digit;
    auto iter = temp.begin();
    std::vector< char > result;
    if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> '0' >> +char_( "0-7" ), result ) && iter == temp.end() )
      return std::string( result.begin(), result.end() );
    return boost::optional< std::string >();
  }

  mpint::mpint( const std::string &value_ ) {
    MPINT_SAFE_CALL( mp_init( &value ) );
    mp_zero( &value );
    boost::optional< std::string > transformed = is_oct( value_ );
    if( transformed ) {
      from_string( value_, 8 );
    }
    else {
      transformed = is_hex( value_ );
      if( transformed ) {
        from_string( *transformed, 16 );
      }
      else {
        transformed = is_dec( value_ );
        if( transformed ) {
          std::string foo = *transformed;
          from_string( *transformed, 10 );
        }
        else
          throw std::runtime_error( "Invalid integer format" );
      }
    }
  }
  std::string mpint::to_string( int radix ) const {
    int length;
    MPINT_SAFE_CALL( mp_radix_size( const_cast< mp_int* >( &value ), radix, &length ) );
    std::vector< char > temp( length );
    std::fill( temp.begin(), temp.end(), 0 );
    MPINT_SAFE_CALL( mp_toradix_n( const_cast< mp_int* >( &value ), &temp.front(), radix, length ) );
    std::string str( &temp.front() );
    return str;
  }

  std::ostream &operator<<( std::ostream &stream, const mpint &mp_value ) {
    if( stream.flags() & std::ios_base::dec )
      stream << mp_value.to_string( 10 );
    else if( stream.flags() & std::ios_base::hex )
      stream << mp_value.to_string( 16 );
    else if( stream.flags() & std::ios_base::oct )
      stream << mp_value.to_string( 8 );
    else
      stream << mp_value.to_string( 10 );
    return stream;
  }

  std::istream &operator>>( std::istream &stream, mpint &mp_value ) {
    using boost::spirit::qi::parse;
    using boost::spirit::ascii::char_;
    using boost::spirit::ascii::space;
    using boost::spirit::ascii::digit;
    if( stream.flags() & std::ios_base::dec ) {
      while( stream.good() && !stream.eof() ) {
        std::string temp;
        stream >> temp;
        auto iter = temp.begin();
        std::vector< char > result;
        if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> +digit, result ) && iter == temp.end() ) {
          mp_value.from_string( temp, 10 );
          break;
        }
      }
    }
    else if( stream.flags() & std::ios_base::hex ) {
      while( stream.good() && !stream.eof() ) {
        std::string temp;
        stream >> temp;
        auto iter = temp.begin();
        std::vector< char > result;
        if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> +( digit|char_( "a-fA-F" ) ), result ) && iter == temp.end() ) {
          mp_value.from_string( temp, 16 );
          break;
        }
      }
    }
    else if( stream.flags() & std::ios_base::oct ) {
      while( stream.good() && !stream.eof() ) {
        std::string temp;
        stream >> temp;
        auto iter = temp.begin();
        std::vector< char > result;
        if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> +char_( "0-7" ), result ) && iter == temp.end() ) {
          mp_value.from_string( temp, 8 );
          break;
        }
      }
    }
    else {
      while( stream.good() && !stream.eof() ) {
        std::string temp;
        stream >> temp;
        auto iter = temp.begin();
        std::vector< char > result;
        if( parse( iter, temp.end(), -( char_('-')|char_('+') ) >> +digit, result ) && iter == temp.end() ) {
          mp_value.from_string( temp, 10 );
          break;
        }
      }
    }
    return stream;
  }

  mpint sqrt( const mpint &value ) {
    mpint temp;
    MPINT_SAFE_CALL( mp_sqrt( const_cast< mp_int* >( &value.get_raw() ), const_cast< mp_int* >( &temp.get_raw() ) ) );
    return temp;
  }

  void swap( mpint &left, mpint &right ) {
    mp_exch( const_cast< mp_int* >( &left.get_raw() ), const_cast< mp_int* >( &right.get_raw() ) );
  }

}

