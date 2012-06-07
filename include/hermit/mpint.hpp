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

#include <boost/cstdint.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/comparison.hpp>
#include <tommath.h>


#define MPINT_SAFE_CALL( expr ) \
{ \
  int mp_op_result = expr; \
  if( mp_op_result != MP_OKAY ) \
  throw std::runtime_error( mp_error_to_string( mp_op_result ) ); \
}

namespace hermit {
  boost::optional< std::string > is_hex( const std::string &temp );
  boost::optional< std::string > is_dec( const std::string temp );
  boost::optional< std::string > is_oct( const std::string temp );

  class mpint {
    public:
      mpint() {
        MPINT_SAFE_CALL( mp_init( &value ) );
        mp_zero( &value );
      }
      mpint( const mpint &src_ ) {
        MPINT_SAFE_CALL( mp_init_copy( &value, const_cast< mp_int* >( &src_.value ) ) );
      }
      mpint &operator=( const mpint &src_ ) {
        MPINT_SAFE_CALL( mp_init_copy( &value, const_cast< mp_int* >( &src_.value ) ) );
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
          mp_zero( &value );
          from_string( boost::lexical_cast< std::string >( value_ ), 10 );
        }
      mpint( const std::string &value_ );
      ~mpint() {
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
        static const mpint mask( "0xFFFFFFFFFFFFFFFF" );
        return boost::lexical_cast< uint64_t >( *this & mask );
      }
      operator int64_t() const {
        static const mpint mask( "0x7FFFFFFFFFFFFFFF" );
        return boost::lexical_cast< int64_t >( *this & mask );
      }
      operator uint32_t() const {
        static const mpint mask( "0xFFFFFFFF" );
        return boost::lexical_cast< uint32_t >( *this & mask );
      }
      operator int32_t() const {
        static const mpint mask( "0x7FFFFFFF" );
        return boost::lexical_cast< int32_t >( *this & mask );
      }
      operator uint16_t() const {
        static const mpint mask( "0xFFFF" );
        return boost::lexical_cast< uint16_t >( *this & mask );
      }
      operator int16_t() const {
        static const mpint mask( "0x7FFF" );
        return boost::lexical_cast< int16_t >( *this & mask );
      }
      operator uint8_t() const {
        static const mpint mask( "0xFF" );
        return boost::lexical_cast< uint8_t >( *this & mask );
      }
      operator int8_t() const {
        static const mpint mask( "0x7F" );
        return boost::lexical_cast< int8_t >( *this & mask );
      }
      mpint &operator>>=( int right ) {
        *this = *this >> right;
        return *this;
      }
      mpint &operator<<=( int right ) {
        *this = *this << right;
        return *this;
      }
      mpint operator>>( int right ) const {
        mpint result;
        mpint remainder;
        MPINT_SAFE_CALL( mp_div_2d( const_cast< mp_int* >( &value ), right, &result.value, &remainder.value ) ) 
          return result;
      }
      mpint operator<<( int right ) const {
        mpint result;
        MPINT_SAFE_CALL( mp_mul_2d( const_cast< mp_int* >( &value ), right, &result.value ) ) 
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
      std::string to_string( int radix ) const;
      void from_string( const std::string &str, int radix ) {
        MPINT_SAFE_CALL( mp_read_radix( const_cast< mp_int* >( &value ), str.c_str(), radix ) );
      }

      const mp_int &get_raw() const {
        return value;
      }
    private:
      mp_int value;
  };

  std::ostream &operator<<( std::ostream &stream, const mpint &mp_value );
  std::istream &operator>>( std::istream &stream, mpint &mp_value );
  mpint sqrt( const mpint &value );
  void swap( mpint &left, mpint &right );

}



#endif

