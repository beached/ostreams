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

#include "ostream_converters_impl.h"
#include "ostream_converters_int.h"
#include "ostream_helpers.h"
#include "static_string.h"

namespace ostream_converters {
	namespace impl {
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

		template<typename F>
		constexpr bool is_inf( F value ) noexcept {
			return value > std::numeric_limits<F>::max( );
		}
		template<typename Float>
		constexpr bool is_nan( Float value ) noexcept {
			return value != value;
		}

		template<typename Float>
		constexpr static int max_fractional_digits =
		  std::numeric_limits<Float>::min_exponent10 * -1;
	} // namespace impl

	// Floating point numbers
	template<
	  typename CharT, typename Float, typename Traits = daw::char_traits<CharT>,
	  std::enable_if_t<daw::is_floating_point_v<Float>, std::nullptr_t> = nullptr>
	constexpr auto to_os_string(
	  Float value,
	  int significant_digits = std::numeric_limits<Float>::max_digits10,
	  int precision = std::numeric_limits<Float>::max_digits10 ) {
		auto const min_value = std::numeric_limits<Float>::is_iec559 ? std::numeric_limits<Float>::denorm_min( ) : std::numeric_limits<Float>::min( );

		size_t const buff_size = std::numeric_limits<Float>::max_digits10 + 8;

		daw::static_string_t<CharT, buff_size> result{};

		if( ( value == static_cast<Float>( 0 ) &&
		      value != min_value ) ||
		    ( value == static_cast<Float>( -0.0 ) &&
		      value != -min_value ) ) {

				result += Traits::get_char_digit( 0 );
				return result;
		}
		if( impl::is_nan( value ) ) {
			result += Traits::nan( );
			return result;
		}
		if( value < 0 ) {
			result += static_cast<CharT>( '-' );
			value = -value;
		}
		if( impl::is_inf( value ) ) {
			result += Traits::inf( );
			return result;
		}

		// Whole numbers
		auto const e = impl::whole_log10<uint16_t>( value );
		auto tmp_value = value;

		auto const max_dig = static_cast<int16_t>(
		  daw::max( 0, static_cast<int>( e ) -
		                 daw::min( std::numeric_limits<Float>::max_digits10,
		                           significant_digits ) ) );

		for( int ex = e; ex >= max_dig; --ex ) {
			auto const p10 = impl::pow10<Float>( ex );
			auto const digit = static_cast<char>( tmp_value / p10 );

			daw::exception::dbg_precondition_check<impl::unexpected_state>(
			  digit >= 0 && digit <= 10 );

			result += Traits::get_char_digit( digit );

			tmp_value -= static_cast<Float>( digit ) * p10;
		}
		for( int16_t ex = max_dig; ex > 0; --ex ) {
			result += Traits::get_char_digit( 0 );
			result += result.back( );
			for( size_t n = result.size( ) - 2; n > 0; --n ) {
				result[n] = result[n - 1];
			}
			result[1] = Traits::decimal_point;
			result += 'e';
			result += to_os_string<CharT>( e );
			return result;
		}
		if( e >= std::numeric_limits<Float>::max_digits10 ||
		    e >= significant_digits ||
		    tmp_value < min_value ) {
			return result;
		}
		// Fractional Part
		intmax_t neg_exp = -1;
		result += Traits::decimal_point;
		if( e == 0 && value < static_cast<Float>( 1 ) &&
		    value >= min_value ) {
			neg_exp = 0;
			// We have no significant digits previously.
			// Output zeros
			auto digit = static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
			while( digit == 0 ) {
				++neg_exp;
				tmp_value *= static_cast<Float>( 10 );
				digit = static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
			}
			if( neg_exp > 3 && result.size( ) > 0 ) {
				if( result[0] == static_cast<CharT>( '-' ) ) {
					result.resize( 1, false );
				} else {
					result.resize( 0, false );
				}
				result += Traits::get_char_digit( digit );
				tmp_value *= static_cast<Float>( 10 );
				tmp_value -= static_cast<Float>( digit );
				result += Traits::decimal_point;
			} else {
				neg_exp = -1;
			}
		}

		auto const num_dec_digits =
		  daw::min( daw::min( precision, impl::max_fractional_digits<Float> ),
		            std::numeric_limits<Float>::max_digits10 - e );

		for( int n = 0;
		     n < num_dec_digits && tmp_value >= min_value;
		     ++n ) {
			auto const digit =
			  static_cast<char>( tmp_value * static_cast<Float>( 10 ) );

			result += Traits::get_char_digit( digit );
			tmp_value -= static_cast<Float>( digit ) / static_cast<Float>( 10 );
			tmp_value *= static_cast<Float>( 10 );
		}
		while( result[result.size( ) - 2] != Traits::decimal_point &&
		       result.back( ) == Traits::get_char_digit( 0 ) ) {
			result.resize( result.size( ) - 1, false );
		}
		if( neg_exp >= 0 ) {
			result += static_cast<CharT>( 'e' );
			result += static_cast<CharT>( '-' );
			result += to_os_string<CharT>( neg_exp + 1 );
		}
		return result;
	} // namespace ostream_converters
} // namespace ostream_converters
