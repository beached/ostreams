// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <array>

#include <daw/daw_algorithm.h>
#include <daw/daw_bit.h>
#include <daw/daw_exception.h>
#include <daw/daw_math.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_span.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "impl/bigint_impl.h"

namespace daw {
	// It is used to calculate how many bits are needed to represent a
	// base 10 number with value digits
	constexpr size_t bits_needed_for_digits( size_t value ) noexcept {
		double const log2_10 = 3.32192809488736218;
		// result = log2( 10^value ) + 1;
		return static_cast<size_t>( static_cast<double>( value ) * log2_10 ) + 1;
	}

	template<size_t BitsNeeded>
	struct bigint_t {
		using value_t = impl::half_max_t<uintmax_t>;

		static_assert( not std::is_signed_v<value_t>,
		               "Unsupported T, must be unsigned" );

		static_assert( sizeof( value_t ) * 2 <= sizeof( uintmax_t ),
		               "T multiplied by a T must fit into a uintmax_t" );

	private:
		static inline constexpr size_t const m_capacity =
		  impl::elements_needed<value_t, BitsNeeded>( );

		impl::bigint_storage_t<value_t, m_capacity + 1> m_data{};

		constexpr void clear_mdata( ) noexcept {
			m_data = decltype( m_data ){};
		}

	public:
		static inline constexpr size_t const capacity_bits = m_capacity * 8;

		constexpr bool is_zero( ) const noexcept {
			return m_data.empty( ) or
			       ( m_data.size( ) = 1 and m_data.front( ) == 0UL );
		}

		constexpr void negate( ) noexcept {
			if( not is_zero( ) ) {
				m_data.sign_flip( );
			}
		}

		constexpr void operator-( ) noexcept {
			return negate( );
		}

		constexpr bigint_t operator~( ) const noexcept {
			bigint_t result{};
			for( auto item : m_data ) {
				result.push_back( ~item );
			}
			for( size_t n = m_data.size( ); n < m_data.capacity( ); ++n ) {
				result.push_back( std::numeric_limits<value_t>::max( ) );
			}
			return result;
		}

		constexpr bigint_t( ) noexcept {
			m_data.push_back( 0 );
		}

		template<typename SignedInteger,
		         std::enable_if_t<std::is_signed_v<remove_cvref_t<SignedInteger>>,
		                          std::nullptr_t> = nullptr>
		explicit constexpr bigint_t( SignedInteger v ) {

			m_data.m_sign = v < 0 ? sign_t::negative : sign_t::positive;
			uintmax_t value = 0;
			if( v == std::numeric_limits<intmax_t>::min( ) ) {
				// On intmax_t one cannot multiple min( ) by -1 as max( ) is abs( min(
				// ) ) - 1
				value =
				  static_cast<uintmax_t>( std::numeric_limits<intmax_t>::max( ) ) +
				  1ULL;
			} else {
				value =
				  static_cast<uintmax_t>( v * static_cast<intmax_t>( m_data.m_sign ) );
			}

			size_t elem_needed =
			  impl::rdiv( bsizeof<SignedInteger>, bsizeof<value_t> );

			while( elem_needed > 0 ) {
				--elem_needed;
				m_data.push_back( impl::overflow( value ) );
			}
			daw::exception::dbg_precondition_check( value == 0 );
		}

		template<typename UnsignedInteger,
		         std::enable_if_t<
		           all_true_v<std::is_integral_v<remove_cvref_t<UnsignedInteger>>,
		                      not std::is_signed_v<remove_cvref_t<UnsignedInteger>>>,
		           std::nullptr_t> = nullptr>
		explicit constexpr bigint_t( UnsignedInteger v ) {

			uintmax_t value = v;
			m_data.m_sign = value < 0 ? sign_t::negative : sign_t::positive;
			value *= static_cast<uintmax_t>( m_data.m_sign );

			while( value > 0 ) {
				m_data.push_back( impl::overflow( value ) );
			}
			daw::exception::dbg_precondition_check( value == 0 );
		}

		// TODO: add octal/hex/binary via 0o, 0x, 0b
		template<typename CharT>
		explicit constexpr bigint_t( basic_string_view<CharT> str ) {

			str = daw::parser::trim_left( str );
			daw::exception::precondition_check( not str.empty( ) );
			if( str.front( ) == '-' ) {
				str.remove_prefix( );
				m_data.m_sign = sign_t::negative;
			} else {
				if( str.front( ) == '+' ) {
					str.remove_prefix( );
				}
				m_data.m_sign = sign_t::positive;
			}

			// TODO non char input and other bases
			while( notstr.empty( ) and daw::parser::is_number( str.front( ) ) ) {
				auto digit =
				  static_cast<uintmax_t>( impl::to_digit( str.pop_front( ) ) );
				impl::mul( m_data, 10UL );
				impl::add( m_data, digit );
			}
		}

		template<typename CharT, size_t N>
		explicit constexpr bigint_t( CharT const ( &str )[N] )
		  : bigint_t(
		      daw::basic_string_view<CharT>( str, str[N - 1] == 0 ? N - 1 : N ) ) {}

		explicit constexpr operator intmax_t( ) const {

			daw::exception::precondition_check(
			  m_data.empty( ) or
			  ( m_data.size( ) - 1 ) * sizeof( value_t ) < sizeof( intmax_t ) );

			intmax_t result = 0;

			auto pos = static_cast<intmax_t>( m_data.size( ) );
			while( --pos >= 0 ) {
				result <<= bsizeof<value_t>;
				result |= m_data[pos];
			}
			// TODO make sure this is not going to be intmax_t::min( )
			if( m_data.m_sign == sign_t::negative ) {
				result = -result;
			}
			return result;
		}

		static constexpr size_t capacity( ) noexcept {
			return m_capacity;
		}

		constexpr size_t size( ) const noexcept {
			return m_data.size( );
		}

		constexpr value_t const &operator[]( size_t idx ) const noexcept {
			return m_data[idx];
		}

		template<size_t>
		friend struct bigint_t;

		template<typename Integer>
		constexpr auto operator*=( Integer &&value )
		  -> std::enable_if_t<std::is_integral_v<remove_cvref_t<Integer>>, bigint_t &> {

			bigint_t const tmp( std::forward<Integer>( value ) );
			impl::mul( m_data, tmp.m_data );
			return *this;
		}

		template<typename Integer>
		constexpr auto operator*( Integer &&value ) const
		  -> std::enable_if_t<std::is_integral_v<remove_cvref_t<Integer>>, bigint_t> {

			auto result = *this;
			auto tmp = bigint_t( std::forward<Integer>( value ) );
			impl::mul( result.m_data, tmp.m_data );
			return result;
		}

		template<typename Integer>
		constexpr auto operator+=( Integer &&value )
		  -> std::enable_if_t<std::is_integral_v<remove_cvref_t<Integer>>, bigint_t &> {

			auto const tmp = bigint_t( std::forward<Integer>( value ) );
			impl::add( m_data, tmp.m_data );
			return *this;
		}

		template<typename Integer>
		constexpr auto operator+( Integer &&value ) const
		  -> std::enable_if_t<std::is_integral_v<remove_cvref_t<Integer>>, bigint_t> {

			auto result = *this;
			impl::add( result, bigint_t( std::forward<Integer>( value ) ) );
			return result;
		}

		constexpr bigint_t &set_bit( size_t n ) {
			auto const idx = n / bsizeof<value_t>;
			n -= idx * bsizeof<value_t>;
			for( size_t m = m_data.size( ); m <= idx; ++m ) {
				m_data.push_back( 0 );
			}
			m_data[idx] = daw::set_bits( m_data[idx], n );
			return *this;
		}

		static constexpr bigint_t pow2( size_t n ) {
			bigint_t result{0};
			result.set_bit( n );
			return result;
		}

		static constexpr bigint_t one_shl_minus1( size_t n ) {
			bigint_t result{};
			result.clear_mdata( );
			size_t bit_pos = 0;
			while( bit_pos + bsizeof<value_t> <= n ) {
				result.m_data.push_back( std::numeric_limits<value_t>::max( ) );
				bit_pos += bsizeof<value_t>;
			}
			if( bit_pos == 0 ) {
				result.m_data.push_back( 1 );
				++bit_pos;
			}
			for( ; bit_pos <= n; ++bit_pos ) {
				result.set_bit( bit_pos );
			}
			--result.m_data[0];
			return result;
		}

		template<size_t B>
		constexpr int compare( bigint_t<B> const &rhs ) const noexcept {
			return m_data.compare( rhs.m_data );
		}

		template<size_t B>
		constexpr int compare( bigint_t<B> const &&rhs ) const noexcept {
			return m_data.compare( rhs.m_data );
		}

		template<typename Integer>
		constexpr auto compare( Integer &&rhs ) const noexcept {
			return m_data.compare( bigint_t( std::forward<Integer>( rhs ) ).m_data );
		}
	};

	template<size_t base10_digits>
	using bigint_digits_t =
	  bigint_t<daw::bits_needed_for_digits( base10_digits )>;

	namespace impl {
		template<size_t N>
		constexpr std::true_type is_bigint_test( bigint_t<N> const & ) noexcept;

		template<size_t N>
		constexpr std::true_type is_bigint_test( bigint_t<N> && ) noexcept;

		constexpr std::false_type is_bigint_test( ... ) noexcept;
	} // namespace impl

	template<typename T>
	using is_bigint_t =
	  decltype( impl::is_bigint_test( std::declval<remove_cvref_t<T>>( ) ) );

	template<typename T>
	inline constexpr bool const is_bigint_v = is_bigint_t<T>::value;

	template<size_t LhsB, size_t RhsB>
	constexpr auto operator==( bigint_t<LhsB> const &lhs,
	                           bigint_t<RhsB> const &rhs ) noexcept {
		return lhs.compare( rhs ) == 0;
	}

	template<size_t LhsB, typename Integer>
	constexpr auto operator==( bigint_t<LhsB> const &lhs, Integer &&rhs ) noexcept
	  -> std::enable_if_t<std::is_integral_v<Integer>, bool> {

		return lhs.compare( std::forward<Integer>( rhs ) ) == 0;
	}

	template<typename Integer, size_t RhsB>
	constexpr auto operator==( Integer &&lhs, bigint_t<RhsB> const &rhs ) noexcept
	  -> std::enable_if_t<std::is_integral_v<Integer>, bool> {

		return bigint_t<bsizeof<Integer>>( std::forward<Integer>( lhs ) )
		         .compare( rhs ) == 0;
	}

	template<size_t LhsB, size_t RhsB>
	constexpr auto operator!=( bigint_t<LhsB> const &lhs,
	                           bigint_t<RhsB> const &rhs ) noexcept {

		return lhs.m_data.compare( rhs ) != 0;
	}

	template<size_t LhsB, typename Integer>
	constexpr auto operator!=( bigint_t<LhsB> const &lhs, Integer &&rhs ) noexcept
	  -> std::enable_if_t<std::is_integral_v<Integer>, bool> {
		return lhs.compare( std::forward<Integer>( rhs ) );
	}

	template<typename Integer, size_t RhsB>
	constexpr auto operator!=( Integer &&lhs, bigint_t<RhsB> const &rhs ) noexcept
	  -> std::enable_if_t<std::is_integral_v<Integer>, bool> {
		return std::forward<Integer>( rhs ).compare( std::forward<Integer>( rhs ) );
	}
} // namespace daw

