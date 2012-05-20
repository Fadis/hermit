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

#ifndef HERMIT_MPINT_HPP
#define HERMIT_MPINT_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/integer.hpp>
#include <tommath.h>


#define MPINT_SAFE_CALL( expr ) \
  { \
    int mp_op_result = expr; \
    if( mp_op_result != MP_OKAY ) \
      throw std::runtime_error( mp_error_to_string( mp_op_result ) ); \
  }

namespace hermit {
  class mpint {
  public:
    mpint() {
      MPINT_SAFE_CALL( mp_init( &value ) );
      std::cout << __FILE__ << " " << __LINE__ << " " << "init mp " << this << " " << value.dp << std::endl;
      mp_zero( &value );
    }
    mpint( mpint &&src_ ) : value( std::move( src_.value ) ) {
    }
    mpint( const mpint &src_ ) {
      MPINT_SAFE_CALL( mp_init_copy( &value, const_cast< mp_int* >( &src_.value ) ) );
      std::cout << __FILE__ << " " << __LINE__ << " " << "copy init mp " << this << " " << value.dp << std::endl;
    }
    mpint &operator=( mpint &&src_ ) {
      std::cout << __FILE__ << " " << __LINE__ << " " << "move init mp " << this << " " << value.dp << std::endl;
      value = std::move( src_.value );
      return *this;
    }
    mpint &operator=( const mpint &src_ ) {
      MPINT_SAFE_CALL( mp_init_copy( &value, const_cast< mp_int* >( &src_.value ) ) );
      std::cout << __FILE__ << " " << __LINE__ << " " << "copy init mp " << this << " " << value.dp << std::endl;
      return *this;
    }
    template< typename T >
    mpint( T value_,
          typename boost::enable_if<
            typename boost::is_integral<
              typename boost::remove_cv< T >::type
            >::type
          >::type* = NULL
    ) {
      MPINT_SAFE_CALL( mp_init( &value ) );
      std::cout << __FILE__ << " " << __LINE__ << " " << "init mp " << this << " " << value.dp << std::endl;
      mp_zero( &value );
      from_string( boost::lexical_cast< std::string >( value_ ), 10 );
    }
    mpint( const std::string &value_ ) {
      MPINT_SAFE_CALL( mp_init( &value ) );
      std::cout << __FILE__ << " " << __LINE__ << " " << "init mp " << this << " " << value.dp << std::endl;
      mp_zero( &value );
      from_string( value_, 10 );
    }
    ~mpint() {
      std::cout << __FILE__ << " " << __LINE__ << " " << "delete mp " << this << " " << value.dp << std::endl;
      mp_clear( &value );
    }
    void swap( mpint &target ) {
      mp_exch( &value, &target.value );
    }
    void shrink_to_fit() {
      MPINT_SAFE_CALL( mp_shrink( &value ) );
    }
    void reserve( int size_ ) {
      MPINT_SAFE_CALL( mp_grow( &value, size_ ) );
    }
    void clear() {
      mp_zero( &value );
    }
    int size() const {
      return mp_count_bits( const_cast< mp_int* >( &value ) );
    }
    operator uint64_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< uint64_t >( temp );
    }
    operator int64_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< int64_t >( temp );
    }
    operator uint32_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< uint32_t >( temp );
    }
    operator int32_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< int32_t >( temp );
    }
    operator uint16_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< uint16_t >( temp );
    }
    operator int16_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< int16_t >( temp );
    }
    operator uint8_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< uint8_t >( temp );
    }
    operator int8_t() const {
      std::string temp = to_string( 10 );
      return boost::lexical_cast< int8_t >( temp );
    }
    mpint &operator>>=( int right ) {
      mp_rshd( &value, right );
    }
    mpint &operator<<=( int right ) {
      MPINT_SAFE_CALL( mp_lshd( &value, right ) );
    }
    mpint operator>>( int right ) const {
      mpint result = *this;
      result >>= right;
      return result;
    }
    mpint operator<<( int right ) const {
      mpint result = *this;
      result <<= right;
      return result;
    }
    mpint operator^( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_xor( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator^=( const mpint &right ) {
      *this = *this ^ right;
      return *this;
    }
    mpint operator|( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_or( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator|=( const mpint &right ) {
      *this = *this | right;
      return *this;
    }
    mpint operator&( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_and( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator&=( const mpint &right ) {
      *this = *this + right;
      return *this;
    }
    mpint operator-() {
      mpint result;
      MPINT_SAFE_CALL( mp_neg( &result.value, &value ) );
      return result;
    }
    mpint operator+( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_add( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator+=( const mpint &right ) {
      *this = *this + right;
      return *this;
    }
    mpint operator++() {
      mpint temp = *this;
      *this += static_cast<mpint>( 1ul );
      return temp;
    }
    mpint &operator++( int ) {
      *this += static_cast<mpint>( 1ul );
      return *this;
    }
    mpint operator-( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_sub( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator-=( const mpint &right ) {
      *this = *this - right;
      return *this;
    }
    mpint operator--() {
      mpint temp = *this;
      *this -= static_cast<mpint>( 1ul );
      return temp;
    }
    mpint &operator--( int ) {
      *this -= static_cast<mpint>( 1ul );
      return *this;
    }
    mpint operator*( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_mul( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator*=( const mpint &right ) {
      *this = *this * right;
      return *this;
    }
    mpint operator/( const mpint &right ) const {
      mpint result;
      mpint remainder;
      MPINT_SAFE_CALL( mp_div( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value, &remainder.value ) );
      return result;
    }
    mpint &operator/=( const mpint &right ) {
      *this = *this / right;
      return *this;
    }
    mpint operator%( const mpint &right ) const {
      mpint result;
      MPINT_SAFE_CALL( mp_mod( const_cast< mp_int* >( &value ), const_cast< mp_int* >( &right.value ), &result.value ) );
      return result;
    }
    mpint &operator%=( const mpint &right ) {
      *this = *this % right;
      return *this;
    }
    std::string to_string( int radix ) const {
      int length;
      MPINT_SAFE_CALL( mp_radix_size( const_cast< mp_int* >( &value ), radix, &length ) );
      std::vector< char > temp( length );
      MPINT_SAFE_CALL( mp_toradix_n( const_cast< mp_int* >( &value ), &temp.front(), radix, length ) );
      std::string str( temp.begin(), temp.end() );
      return str;
    }
    void from_string( const std::string &str, int radix ) {
      MPINT_SAFE_CALL( mp_read_radix( const_cast< mp_int* >( &value ), str.c_str(), radix ) );
    }
    const mp_int &get_raw() const {
      return value;
    }
  private:
    mp_int value;
  };

  std::ostream &operator<<( std::ostream &stream, const mpint &mp_value ) {
    if( stream.flags() & std::ios_base::dec )
      stream << mp_value.to_string( 10 );
    else if( stream.flags() & std::ios_base::hex )
      stream << mp_value.to_string( 16 );
    else if( stream.flags() & std::ios_base::oct )
      stream << mp_value.to_string( 8 );
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



#endif

