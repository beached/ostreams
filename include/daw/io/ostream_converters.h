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
		template<typename T, typename Result = daw::remove_cvref_t<T>>
		constexpr Result num_digits( T value ) noexcept {
			static_assert( daw::is_integral_v<Result>,
			               "Result must be an integer type" );
			if( value == 0 ) {
				return static_cast<Result>( 1 );
			}
			Result result = 0;
			while( value != 0 ) {
				value /= 10;
				++result;
			}
			return result;
		}

#ifndef SMALL_MEMORY
		template<
		  typename Result, typename Int,
		  std::enable_if_t<daw::is_same_v<Result, float>, std::nullptr_t> = nullptr>
		constexpr float pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( 0 <= n && n <= 38, "overflow" );
			float const vals[20] = {1e0f,  1e2f,  1e4f,  1e6f,  1e8f,  1e10f, 1e12f,
			                        1e14f, 1e16f, 1e18f, 1e20f, 1e22f, 1e24f, 1e26f,
			                        1e28f, 1e30f, 1e32f, 1e34f, 1e36f, 1e38f};
			if( n % 2 == 0 ) {
				return vals[n / 2];
			}
			return vals[n / 2] * 1e1f;
		}
		template<typename Result, typename Int,
		         std::enable_if_t<daw::is_same_v<Result, double>, std::nullptr_t> =
		           nullptr>
		constexpr double pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( 0 <= n && n <= 308, "overflow" );
			double const vals[155] = {
			  1e0,   1e2,   1e4,   1e6,   1e8,   1e10,  1e12,  1e14,  1e16,  1e18,
			  1e20,  1e22,  1e24,  1e26,  1e28,  1e30,  1e32,  1e34,  1e36,  1e38,
			  1e40,  1e42,  1e44,  1e46,  1e48,  1e50,  1e52,  1e54,  1e56,  1e58,
			  1e60,  1e62,  1e64,  1e66,  1e68,  1e70,  1e72,  1e74,  1e76,  1e78,
			  1e80,  1e82,  1e84,  1e86,  1e88,  1e90,  1e92,  1e94,  1e96,  1e98,
			  1e100, 1e102, 1e104, 1e106, 1e108, 1e110, 1e112, 1e114, 1e116, 1e118,
			  1e120, 1e122, 1e124, 1e126, 1e128, 1e130, 1e132, 1e134, 1e136, 1e138,
			  1e140, 1e142, 1e144, 1e146, 1e148, 1e150, 1e152, 1e154, 1e156, 1e158,
			  1e160, 1e162, 1e164, 1e166, 1e168, 1e170, 1e172, 1e174, 1e176, 1e178,
			  1e180, 1e182, 1e184, 1e186, 1e188, 1e190, 1e192, 1e194, 1e196, 1e198,
			  1e200, 1e202, 1e204, 1e206, 1e208, 1e210, 1e212, 1e214, 1e216, 1e218,
			  1e220, 1e222, 1e224, 1e226, 1e228, 1e230, 1e232, 1e234, 1e236, 1e238,
			  1e240, 1e242, 1e244, 1e246, 1e248, 1e250, 1e252, 1e254, 1e256, 1e258,
			  1e260, 1e262, 1e264, 1e266, 1e268, 1e270, 1e272, 1e274, 1e276, 1e278,
			  1e280, 1e282, 1e284, 1e286, 1e288, 1e290, 1e292, 1e294, 1e296, 1e298,
			  1e300, 1e302, 1e304, 1e306, 1e308};
			if( n % 2 == 0 ) {
				return vals[n / 2];
			}
			return vals[n / 2] * 1e1;
		}
#else
		template<
		  typename Result, typename Int,
		  std::enable_if_t<daw::is_same_v<Result, float>, std::nullptr_t> = nullptr>
		constexpr float pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( n <= 38, "overflow" );

			float result = 1;
			while( n-- > 0 ) {
				result *= 1e1f;
			}
			return result;
		}
		template<typename Result, typename Int,
		         std::enable_if_t<daw::is_same_v<Result, double>, std::nullptr_t> =
		           nullptr>
		constexpr double pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( n <= 308, "overflow" );
			double result = 1;
			while( n-- > 0 ) {
				result *= 1e1;
			}
			return result;
		}
#endif

		template<
		  typename Result, typename Int,
		  std::enable_if_t<daw::is_integral_v<Result>, std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			uint64_t const vals[10] = {1ULL,
			                           100ULL,
			                           10000ULL,
			                           1000000ULL,
			                           100000000ULL,
			                           10000000000ULL,
			                           1000000000000ULL,
			                           100000000000000ULL,
			                           10000000000000000ULL,
			                           1000000000000000000ULL};
			if( n % 2 == 0 ) {
				return static_cast<Result>( vals[n / 2] );
			}
			return static_cast<Result>( vals[n / 2] * 10ULL );
		}

		template<
		  typename Result, typename Number,
		  std::enable_if_t<daw::is_arithmetic_v<Number>, std::nullptr_t> = nullptr>
		constexpr uint16_t whole_log10( Number positive_value ) noexcept {
			Result result = 0;
			while( positive_value >= 10 ) {
				++result;
				positive_value /= static_cast<Number>( 10 );
			}
			return result;
		}

		template<typename F>
		constexpr bool is_inf( F value ) noexcept {
			return value > std::numeric_limits<F>::max( );
		}
		template<typename Float>
		constexpr bool is_nan( Float value ) noexcept {
			return value != value;
		}

		struct unexpected_state {};

		template<typename Float>
		constexpr static int max_fractional_digits =
		  std::numeric_limits<Float>::min_exponent10 * -1;
	} // namespace impl

	// character pointer
	template<typename CharT, std::enable_if_t<daw::traits::is_character_v<CharT>,
	                                          std::nullptr_t> = nullptr>
	constexpr daw::basic_string_view<CharT>
	to_string( CharT const *str ) noexcept {
		return daw::basic_string_view<CharT>( str );
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

	namespace int_string_sizes {
		template<size_t N>
		constexpr size_t get( ) noexcept {
			return 0;
		}

		template<>
		constexpr size_t get<1>( ) noexcept {
			return 5;
		}

		template<>
		constexpr size_t get<2>( ) noexcept {
			return 7;
		}

		template<>
		constexpr size_t get<4>( ) noexcept {
			return 11;
		}

		template<>
		constexpr size_t get<8>( ) noexcept {
			return 21;
		}
	} // namespace int_string_sizes
	namespace min_strings {
		constexpr auto get( char, std::integral_constant<size_t, 1> ) noexcept {
			return daw::static_string_t<char, int_string_sizes::get<1>( )>( "-128" );
		}

		constexpr auto get( wchar_t, std::integral_constant<size_t, 1> ) noexcept {
			return daw::static_string_t<wchar_t, int_string_sizes::get<1>( )>(
			  L"-128" );
		}

		constexpr auto get( char, std::integral_constant<size_t, 2> ) noexcept {
			return daw::static_string_t<char, int_string_sizes::get<2>( )>(
			  "-32768" );
		}

		constexpr auto get( wchar_t, std::integral_constant<size_t, 2> ) noexcept {
			return daw::static_string_t<wchar_t, int_string_sizes::get<2>( )>(
			  L"-32768" );
		}

		constexpr auto get( char, std::integral_constant<size_t, 4> ) noexcept {
			return daw::static_string_t<char, int_string_sizes::get<4>( )>(
			  "-2147483648" );
		}

		constexpr auto get( wchar_t, std::integral_constant<size_t, 4> ) noexcept {
			return daw::static_string_t<wchar_t, int_string_sizes::get<4>( )>(
			  L"-2147483648" );
		}

		constexpr auto get( char, std::integral_constant<size_t, 8> ) noexcept {
			return daw::static_string_t<char, int_string_sizes::get<8>( )>(
			  "-9223372036854775808" );
		}

		constexpr auto get( wchar_t, std::integral_constant<size_t, 8> ) noexcept {
			return daw::static_string_t<wchar_t, int_string_sizes::get<8>( )>(
			  L"-9223372036854775808" );
		}
	} // namespace min_strings

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
		daw::static_string_t<CharT, int_string_sizes::get<sizeof( Integer )>( )>
		  result{};

		if( value < 0 ) {
			if( value == std::numeric_limits<Integer>::min( ) ) {
				return min_strings::get(
				  CharT{}, std::integral_constant<size_t, sizeof( Integer )>{} );
			}
			result += daw::char_traits<CharT>::minus;
			value = -value;
		}
		for( auto pow10 =
		       impl::pow10<Integer>( impl::whole_log10<uint16_t>( value ) );
		     pow10 >= 1; pow10 /= static_cast<Integer>( 10 ) ) {

			auto const tmp = value / pow10;

			daw::exception::dbg_precondition_check(
			  tmp >= 0 && tmp < 10,
			  "There should only ever be a single digit positive number" );

			result += daw::char_traits<CharT>::get_char_digit( tmp );

			value -= tmp * pow10;
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

	// Single character (char, wchar_t).  Need to be treated separately from
	// other integers
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

		size_t const buff_size = std::numeric_limits<Float>::max_exponent10 + 2 - std::numeric_limits<Float>::min_exponent10;
		daw::static_string_t<CharT, buff_size> result{};

		if( ( value == static_cast<Float>( 0 ) &&
		      value != std::numeric_limits<Float>::min( ) ) ||
		    ( value == static_cast<Float>( -0.0 ) &&
		      value != -std::numeric_limits<Float>::min( ) ) ) {
			result += daw::char_traits<CharT>::get_char_digit( 0 );
			return result;
		}
		if( impl::is_nan( value ) ) {
			result += daw::char_traits<CharT>::nan( );
			return result;
		}
		if( value < 0 ) {
			result += daw::char_traits<CharT>::minus;
			value = -value;
		}
		if( impl::is_inf( value ) ) {
			result += daw::char_traits<CharT>::inf( );
			return result;
		}

		// Whole numbers
		auto const e = impl::whole_log10<uint16_t>( value );
		auto tmp_value = value;

		auto const max_dig = static_cast<int16_t>( daw::max(
		  0, static_cast<int>( e ) -
		       static_cast<int>( daw::min( std::numeric_limits<Float>::max_digits10,
		                                   significant_digits ) ) ) );

		for( int ex = e; ex >= max_dig; --ex ) {
			auto const p10 = impl::pow10<Float>( ex );
			auto const digit = static_cast<char>( tmp_value / p10 );

			daw::exception::dbg_precondition_check<impl::unexpected_state>(
			  digit >= 0 && digit <= 10 );

			result += daw::char_traits<CharT>::get_char_digit( digit );

			tmp_value -= static_cast<Float>( digit ) * p10;
		}
		for( int16_t ex = max_dig; ex > 0; --ex ) {
			result += daw::char_traits<CharT>::get_char_digit( 0 );
		}
		if( e >= std::numeric_limits<Float>::max_digits10 ||
		    e >= significant_digits ||
		    tmp_value < std::numeric_limits<Float>::min( ) ) {
			return result;
		}
		// Fractional Part
		result += daw::char_traits<CharT>::decimal_point;

		if( e == 0 && value < static_cast<Float>( 1 ) &&
		    value >= std::numeric_limits<Float>::min( ) ) {
			// We have no significant digits previously.
			// Output zeros
			auto digit = static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
			while( digit == 0 ) {
				result += daw::char_traits<CharT>::get_char_digit( 0 );
				tmp_value *= static_cast<Float>( 10 );
				digit = static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
			}
		}

		auto const num_dec_digits =
		  daw::min( daw::min( precision, impl::max_fractional_digits<Float> ),
		            std::numeric_limits<Float>::max_digits10 - e );

		for( int n = 0;
		     n < num_dec_digits && tmp_value >= std::numeric_limits<Float>::min( );
		     ++n ) {
			auto const digit =
			  static_cast<char>( tmp_value * static_cast<Float>( 10 ) );

			result += daw::char_traits<CharT>::get_char_digit( digit );
			tmp_value -= static_cast<Float>( digit ) / static_cast<Float>( 10 );
			tmp_value *= static_cast<Float>( 10 );
		}
		while( result[result.size( ) - 2] !=
		         daw::char_traits<CharT>::decimal_point &&
		       result.back( ) == daw::char_traits<CharT>::zero ) {
			result.resize( result.size( ) - 1, false );
		}
		return result;
	} // namespace ostream_converters

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

	/*
	  template<typename CharT, typename... Args>
	  auto fmt( Args&&... args ) {
	    auto strings = std::make_tuple( to_string<CharT>( std::forward<Args>(
	  args
	  )... ) );

	  }*/
} // namespace ostream_converters
