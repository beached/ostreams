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

#include <limits>
#include <string>

#include <daw/daw_exception.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostream_helpers.h"
#include "static_string.h"

namespace ostream_converters {
	namespace impl {
		template<typename T>
		constexpr auto num_digits( T value ) noexcept {
			using int_t = std::decay_t<T>;
			static_assert( daw::is_integral_v<int_t>,
			               "value must be an integer type" );
			if( value == 0 ) {
				return static_cast<int_t>( 1 );
			}
			int_t result = 0;
			while( value != 0 ) {
				value /= 10;
				++result;
			}
			return result;
		}

		template<typename Number, std::enable_if_t<daw::is_arithmetic_v<Number>,
		                                           std::nullptr_t> = nullptr>
		constexpr Number pow10( uintmax_t n ) noexcept {
			auto result = static_cast<Number>( 1 );
			while( n > 1 ) {
				result *= static_cast<Number>( 10 );
				--n;
			}
			return result;
		}

		template<typename F>
		constexpr bool is_inf( F value ) noexcept {
			return value > std::numeric_limits<F>::max( );
		}

		template<typename F>
		constexpr uint16_t find_whole_exponent( F positive_value ) noexcept {
			uint16_t result = 0;
			while( positive_value >= static_cast<F>( 10 ) ) {
				positive_value /= static_cast<F>( 10 );
				++result;
			}
			return result;
		}

		template<typename Float>
		constexpr Float min_value( Float a, Float b ) noexcept {
			if( a <= b ) {
				return a;
			}
			return b;
		}

		template<typename Float>
		constexpr bool is_nan( Float value ) noexcept {
			return value != value;
		}

		struct unexpected_state {};

		template<typename Float>
		constexpr static int max_fractional_digits =
		  std::numeric_limits<Float>::min_exponent10 * -1;

		template<typename CharT>
		constexpr size_t strlen( CharT const *ptr ) noexcept {
			size_t result = 0;
			while( ptr[result] != '\0' ) {
				++result;
			}
			return result;
		}

	} // namespace impl

	// character pointer
	template<typename CharT, std::enable_if_t<daw::traits::is_character_v<CharT>,
	                                          std::nullptr_t> = nullptr>
	constexpr daw::basic_string_view<CharT>
	to_string( CharT const *str ) noexcept {
		return daw::basic_string_view<CharT>( str, impl::strlen( str ) );
	}

	// string_view
	template<typename CharT>
	constexpr daw::basic_string_view<CharT>
	to_string( daw::basic_string_view<CharT> str ) noexcept {
		return str;
	}

	// std::basic_string
	// TODO add overload for the operator<< to use accepts_asciiz
	template<typename CharT>
	daw::basic_string_view<CharT>
	to_string( std::basic_string<CharT> const &str ) noexcept {
		return daw::basic_string_view<CharT>( str.data( ), str.size( ) );
	}

	// Integer numbers
	template<
	  typename CharT, typename Integer,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_integral_v<daw::remove_cvref_t<Integer>>,
	                    !daw::is_same_v<bool, daw::remove_cvref_t<Integer>>,
	                    !daw::is_floating_point_v<daw::remove_cvref_t<Integer>>,
	                    !daw::traits::is_character_v<Integer>>,
	    std::nullptr_t> = nullptr>
	static constexpr auto to_string( Integer value ) {
		size_t const buff_size = std::numeric_limits<Integer>::digits10;
		daw::static_string_t<CharT, buff_size> result{};

		if( value < 0 ) {
			result += daw::char_traits<CharT>::minus;
			value *= -1;
		}

		auto max10 = impl::pow10<Integer>( impl::num_digits<Integer>( value ) );
		while( max10 > 0 ) {
			auto const tmp = ( value / max10 ) * max10;

			result += daw::char_traits<CharT>::get_char_digit( tmp / max10 );

			value -= tmp;
			max10 /= 10;
		}
		return result;
	}

	// Bool
	template<typename CharT, typename Bool,
	         std::enable_if_t<
	           daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                           daw::is_same_v<char, daw::remove_cvref_t<CharT>>>,
	           std::nullptr_t> = nullptr>
	constexpr auto to_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( "true" );
		}
		return daw::static_string_t<CharT, 5>( "false" );
	}

	template<
	  typename CharT, typename Bool,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                    daw::is_same_v<wchar_t, daw::remove_cvref_t<CharT>>>,
	    std::nullptr_t> = nullptr>
	constexpr auto to_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( L"true" );
		}
		return daw::static_string_t<CharT, 5>( L"false" );
	}

	// Single character (char, wchar_t).  Need to be treated separately from other
	// integers
	template<typename CharT,
	         std::enable_if_t<::daw::traits::is_character_v<CharT>,
	                          std::nullptr_t> = nullptr>
	constexpr auto to_string( CharT c ) noexcept {
		daw::static_string_t<CharT, 1> result{};
		result.push_back( c );
		return result;
	}

	// Floating point numbers
	template<
	  typename CharT, typename Float,
	  std::enable_if_t<daw::is_floating_point_v<Float>, std::nullptr_t> = nullptr>
	constexpr auto
	to_string( Float value,
	           int significant_digits = std::numeric_limits<Float>::max_digits10,
	           int precision = std::numeric_limits<Float>::max_digits10 ) {
		using value_t = daw::remove_cvref_t<Float>;
		size_t const buff_size = std::numeric_limits<value_t>::max_exponent10 + 2;
		daw::static_string_t<CharT, buff_size> result{};

		if( value == static_cast<value_t>( 0.0 ) ) {
			result += daw::char_traits<CharT>::get_char_digit( 0 );
			return result;
		}
		if( impl::is_nan( value ) ) {
			result += daw::char_traits<CharT>::nan( );
			return result;
		}
		if( value < static_cast<value_t>( 0 ) ) {
			result += daw::char_traits<CharT>::minus;
			value = -value;
		}
		if( impl::is_inf( value ) ) {
			result += daw::char_traits<CharT>::inf( );
			return result;
		}

		auto const e = impl::find_whole_exponent( value );
		auto p10 = impl::pow10<value_t>( e + 1 );
		auto tmp_value = value;

		for( uint16_t ex = 0; ex <= e; ++ex ) {
			auto digit = static_cast<char>( tmp_value / p10 );
			daw::exception::precondition_check<impl::unexpected_state>( digit >= 0 &&
			                                                            digit <= 10 );

			if( ex > std::numeric_limits<value_t>::max_digits10 ||
			    ex > significant_digits ) {
				digit = 0;
			}
			result += daw::char_traits<CharT>::get_char_digit( digit );

			tmp_value -= static_cast<value_t>( digit ) * p10;
			p10 /= 10.0;
		}

		if( e >= std::numeric_limits<value_t>::max_digits10 ||
		    e > significant_digits ||
		    tmp_value <= std::numeric_limits<value_t>::min( ) ) {
			return result;
		}
		result += daw::char_traits<CharT>::decimal_point;

		auto const num_dec_digits = impl::min_value(
		  impl::min_value( precision, impl::max_fractional_digits<Float> ),
		  std::numeric_limits<Float>::max_digits10 - e );
		for( int n = 0;
		     n < num_dec_digits && tmp_value > std::numeric_limits<Float>::min( );
		     ++n ) {
			auto digit = static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
			result += daw::char_traits<CharT>::get_char_digit( digit );
			tmp_value -= static_cast<Float>( digit ) / static_cast<Float>( 10 );
			tmp_value *= static_cast<Float>( 10 );
		}
		return result;
	}

	namespace impl {
		template<typename T>
		using has_tostring_detect2 =
		  decltype( to_string( std::declval<T const &>( ) ) );

		namespace to_string_detect {
			using ::ostream_converters::to_string;
			template<typename CharT, typename T>
			using has_to_string_detect =
			  decltype( to_string<CharT>( std::declval<T const &>( ) ) );
		} // namespace to_string_detect

	} // namespace impl

	template<typename CharT, typename T>
	constexpr bool has_to_string_v =
	  daw::is_detected_v<impl::to_string_detect::has_to_string_detect, CharT,
	                     ::daw::remove_cvref_t<T>>;
} // namespace ostream_converters
