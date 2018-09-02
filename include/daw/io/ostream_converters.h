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
		template<
		  typename Result, typename Int,
		  std::enable_if_t<daw::is_same_v<Result, float>, std::nullptr_t> = nullptr>
		constexpr float pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( n <= 38 );
			float const vals[39] = {
			  1e0f,  1e1f,  1e2f,  1e3f,  1e4f,  1e5f,  1e6f,  1e7f,  1e8f,  1e9f,
			  1e10f, 1e11f, 1e12f, 1e13f, 1e14f, 1e15f, 1e16f, 1e17f, 1e18f, 1e19f,
			  1e20f, 1e21f, 1e22f, 1e23f, 1e24f, 1e25f, 1e26f, 1e27f, 1e28f, 1e29f,
			  1e30f, 1e31f, 1e32f, 1e33f, 1e34f, 1e35f, 1e36f, 1e37f, 1e38f};
			return vals[n];
		}

		template<typename Result, typename Int,
		         std::enable_if_t<daw::is_same_v<Result, double>, std::nullptr_t> =
		           nullptr>
		constexpr double pow10( Int n ) noexcept {
			daw::exception::dbg_precondition_check( n <= 308 );
			double const vals[309] = {
			  1e0,   1e1,   1e2,   1e3,   1e4,   1e5,   1e6,   1e7,   1e8,   1e9,
			  1e10,  1e11,  1e12,  1e13,  1e14,  1e15,  1e16,  1e17,  1e18,  1e19,
			  1e20,  1e21,  1e22,  1e23,  1e24,  1e25,  1e26,  1e27,  1e28,  1e29,
			  1e30,  1e31,  1e32,  1e33,  1e34,  1e35,  1e36,  1e37,  1e38,  1e39,
			  1e40,  1e41,  1e42,  1e43,  1e44,  1e45,  1e46,  1e47,  1e48,  1e49,
			  1e50,  1e51,  1e52,  1e53,  1e54,  1e55,  1e56,  1e57,  1e58,  1e59,
			  1e60,  1e61,  1e62,  1e63,  1e64,  1e65,  1e66,  1e67,  1e68,  1e69,
			  1e70,  1e71,  1e72,  1e73,  1e74,  1e75,  1e76,  1e77,  1e78,  1e79,
			  1e80,  1e81,  1e82,  1e83,  1e84,  1e85,  1e86,  1e87,  1e88,  1e89,
			  1e90,  1e91,  1e92,  1e93,  1e94,  1e95,  1e96,  1e97,  1e98,  1e99,
			  1e100, 1e101, 1e102, 1e103, 1e104, 1e105, 1e106, 1e107, 1e108, 1e109,
			  1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116, 1e117, 1e118, 1e119,
			  1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128, 1e129,
			  1e130, 1e131, 1e132, 1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139,
			  1e140, 1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149,
			  1e150, 1e151, 1e152, 1e153, 1e154, 1e155, 1e156, 1e157, 1e158, 1e159,
			  1e160, 1e161, 1e162, 1e163, 1e164, 1e165, 1e166, 1e167, 1e168, 1e169,
			  1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176, 1e177, 1e178, 1e179,
			  1e180, 1e181, 1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 1e189,
			  1e190, 1e191, 1e192, 1e193, 1e194, 1e195, 1e196, 1e197, 1e198, 1e199,
			  1e200, 1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209,
			  1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219,
			  1e220, 1e221, 1e222, 1e223, 1e224, 1e225, 1e226, 1e227, 1e228, 1e229,
			  1e230, 1e231, 1e232, 1e233, 1e234, 1e235, 1e236, 1e237, 1e238, 1e239,
			  1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246, 1e247, 1e248, 1e249,
			  1e250, 1e251, 1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 1e259,
			  1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269,
			  1e270, 1e271, 1e272, 1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279,
			  1e280, 1e281, 1e282, 1e283, 1e284, 1e285, 1e286, 1e287, 1e288, 1e289,
			  1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296, 1e297, 1e298, 1e299,
			  1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308};
			return vals[n];
		}

		template<typename Result, typename Int,
		         std::enable_if_t<daw::all_true_v<daw::is_integral_v<Result>,
		                                          sizeof( Result ) == 1>,
		                          std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			switch( n ) {
			case 0:
				return static_cast<Result>( 1 );
			case 1:
				return static_cast<Result>( 10 );
			case 2:
				return static_cast<Result>( 100 );
			default:
				abort( );
			}
		}

		template<typename Result, typename Int,
		         std::enable_if_t<daw::all_true_v<daw::is_integral_v<Result>,
		                                          sizeof( Result ) == 2>,
		                          std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			switch( n ) {
			case 3:
				return static_cast<Result>( 1'000 );
			case 4:
				return static_cast<Result>( 10'000 );
			default:
				return static_cast<Result>( pow10<uint8_t>( n ) );
			}
		}

		template<typename Result, typename Int,
		         std::enable_if_t<daw::all_true_v<daw::is_integral_v<Result>,
		                                          sizeof( Result ) == 4>,
		                          std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			switch( n ) {
			case 5:
				return static_cast<Result>( 100'000UL );
			case 6:
				return static_cast<Result>( 1'000'000UL );
			case 7:
				return static_cast<Result>( 10'000'000UL );
			case 8:
				return static_cast<Result>( 100'000'000UL );
			case 9:
				return static_cast<Result>( 1'000'000'000UL );
			default:
				return static_cast<Result>( pow10<uint16_t>( n ) );
			}
		}

		template<typename Result, typename Int,
		         std::enable_if_t<
		           daw::all_true_v<daw::is_integral_v<Result>,
		                           sizeof( Result ) == 8, daw::is_signed_v<Result>>,
		           std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			switch( n ) {
			case 10:
				return static_cast<Result>( 10'000'000'000LL );
			case 11:
				return static_cast<Result>( 100'000'000'000LL );
			case 12:
				return static_cast<Result>( 1'000'000'000'000LL );
			case 13:
				return static_cast<Result>( 10'000'000'000'000LL );
			case 14:
				return static_cast<Result>( 100'000'000'000'000LL );
			case 15:
				return static_cast<Result>( 1'000'000'000'000'000LL );
			case 16:
				return static_cast<Result>( 10'000'000'000'000'000LL );
			case 17:
				return static_cast<Result>( 100'000'000'000'000'000LL );
			case 18:
				return static_cast<Result>( 1'000'000'000'000'000'000LL );
			default:
				return static_cast<Result>( pow10<uint32_t>( n ) );
			}
		}

		template<typename Result, typename Int,
		         std::enable_if_t<daw::all_true_v<daw::is_integral_v<Result>,
		                                          sizeof( Result ) == 8,
		                                          !daw::is_signed_v<Result>>,
		                          std::nullptr_t> = nullptr>
		constexpr Result pow10( Int n ) noexcept {
			switch( n ) {
			case 19:
				return static_cast<Result>( 10'000'000'000'000'000'000ULL );
			default:
				return static_cast<Result>( pow10<int64_t>( n ) );
			}
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
		for( auto pow10 =
		       impl::pow10<Integer>( impl::whole_log10<uint16_t>( value ) );
		     pow10 >= 1; pow10 /= 10 ) {

			auto const tmp = value / pow10;

			daw::exception::dbg_precondition_check(
			  tmp >= 0 && tmp < 10,
			  "There should only ever be a single digit positive number" );

			result += daw::char_traits<CharT>::get_char_digit( tmp );

			value -= tmp * 10;
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

		size_t const buff_size = std::numeric_limits<Float>::max_exponent10 + 2;
		daw::static_string_t<CharT, buff_size> result{};

		if( value == static_cast<Float>( 0 ) ||
		    value == static_cast<Float>( -0.0 ) ) {
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
		    tmp_value <= std::numeric_limits<Float>::min( ) ) {
			return result;
		}

		// Fractional Part
		result += daw::char_traits<CharT>::decimal_point;

		auto const num_dec_digits =
		  daw::min( daw::min( precision, impl::max_fractional_digits<Float> ),
		            std::numeric_limits<Float>::max_digits10 - e );

		for( int n = 0;
		     n < num_dec_digits && tmp_value > std::numeric_limits<Float>::min( );
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
