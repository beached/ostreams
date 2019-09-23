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

#include <daw/daw_exception.h>
#include <daw/daw_traits.h>

#include "ostream_converters_impl.h"
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
		  typename Result, typename Integer,
		  std::enable_if_t<daw::is_integral_v<Result>, std::nullptr_t> = nullptr>
		constexpr Result pow10( Integer n ) noexcept {
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
	} // namespace impl

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

	namespace impl {
		// Integer numbers
		template<
		  typename CharT, typename Integer,
		  typename Traits = daw::char_traits<CharT>,
		  std::enable_if_t<
		    daw::all_true_v<daw::is_integral_v<daw::remove_cvref_t<Integer>>,
		                    !daw::is_same_v<bool, daw::remove_cvref_t<Integer>>,
		                    !daw::is_floating_point_v<daw::remove_cvref_t<Integer>>,
		                    !daw::traits::is_character_v<Integer>>,
		    std::nullptr_t> = nullptr>
		constexpr auto to_os_string( Integer value, daw::tag_t<int> ) {
			daw::static_string_t<CharT, int_string_sizes::get<sizeof( Integer )>( )>
			  result{};

			if( value < 0 ) {
				if( value == std::numeric_limits<Integer>::min( ) ) {
					return min_strings::get(
					  CharT{}, std::integral_constant<size_t, sizeof( Integer )>{} );
				}
				result += static_cast<CharT>( '-' );
				value = -value;
			}
			for( auto pow10 =
			       impl::pow10<Integer>( impl::whole_log10<uint16_t>( value ) );
			     pow10 >= 1; pow10 /= static_cast<Integer>( 10 ) ) {

				auto const tmp = value / pow10;

				daw::exception::dbg_precondition_check(
				  tmp >= 0 && tmp < 10,
				  "There should only ever be a single digit positive number" );

				result += Traits::get_char_digit( tmp );

				value -= tmp * pow10;
			}
			return result;
		}
	} // namespace impl
	template<
	  typename CharT, typename Integer, typename Traits = daw::char_traits<CharT>,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_integral_v<daw::remove_cvref_t<Integer>>,
	                    !daw::is_same_v<bool, daw::remove_cvref_t<Integer>>,
	                    !daw::is_floating_point_v<daw::remove_cvref_t<Integer>>,
	                    !daw::traits::is_character_v<Integer>>,
	    std::nullptr_t> = nullptr>
	constexpr auto to_os_string( Integer value ) {
		return impl::to_os_string<CharT>( value, daw::tag<int> );
	}

	template<typename CharT, typename Integer,
	         typename Traits = daw::char_traits<CharT>>
	constexpr auto to_os_string( daw::as_int_t<Integer> value ) {
		return impl::to_os_string<CharT>( value.value, daw::tag<int> );
	}
} // namespace ostream_converters
