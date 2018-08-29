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

#include "ostream_helpers.h"

namespace daw {
	namespace io {
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
					Number result = static_cast<Number>( 1 );
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

				template<typename CharT, size_t N>
				struct sv_buff {
					CharT buffer[N + 1] = {0};
					size_t len = 0;

					constexpr sv_buff( ) noexcept = default;

					constexpr CharT const *data( ) const {
						return buffer;
					}

					constexpr size_t size( ) const {
						return len;
					}

					constexpr void push_back( CharT c ) {
						daw::exception::precondition_check<buffer_full_exception>( len + 1 <
						                                                           N );
						buffer[len++] = c;
					}

					constexpr void push_back( std::basic_string_view<CharT> sv ) {
						daw::exception::precondition_check<buffer_full_exception>(
						  len + sv.size( ) < N );
						for( auto c : sv ) {
							buffer[len++] = c;
						}
					}

					constexpr operator std::basic_string_view<CharT>( ) const noexcept {
						return std::string_view( buffer, len );
					}
				};

				template<typename CharT>
				struct get_zero;

				template<>
				struct get_zero<char> {
					constexpr char operator( )( ) const noexcept {
						return '0';
					}
				};

				template<>
				struct get_zero<wchar_t> {
					constexpr char operator( )( ) const noexcept {
						return L'0';
					}
				};

				template<typename CharT>
				struct get_decimal;

				template<>
				struct get_decimal<char> {
					constexpr char operator( )( ) const noexcept {
						return '.';
					}
				};

				template<>
				struct get_decimal<wchar_t> {
					constexpr char operator( )( ) const noexcept {
						return L'.';
					}
				};

				template<typename CharT>
				struct get_minus;

				template<>
				struct get_minus<char> {
					constexpr char operator( )( ) const noexcept {
						return '-';
					}
				};

				template<>
				struct get_minus<wchar_t> {
					constexpr char operator( )( ) const noexcept {
						return L'-';
					}
				};
			} // namespace impl

			template<typename CharT,
			         std::enable_if_t<(daw::is_same_v<char, std::decay_t<CharT>> ||
			                           daw::is_same_v<wchar_t, std::decay_t<CharT>>),
			                          std::nullptr_t> = nullptr>
			constexpr std::basic_string_view<CharT>
			to_string( CharT const *str ) noexcept {
				return std::basic_string_view<CharT>( str, impl::strlen( str ) );
			}

			template<size_t N, typename CharT>
			constexpr std::basic_string_view<CharT>
			to_string( CharT const ( &str )[N] ) noexcept {
				return std::basic_string_view<CharT>( str, N - 1 );
			}

			template<typename CharT>
			constexpr std::basic_string_view<CharT>
			to_string( std::basic_string_view<CharT> str ) noexcept {
				return str;
			}

			template<typename CharT>
			std::basic_string_view<CharT>
			to_string( std::basic_string<CharT> const &str ) noexcept {
				return std::basic_string_view<CharT>( str.data( ), str.size( ) );
			}

			template<typename CharT, typename Integer,
			         std::enable_if_t<(daw::is_integral_v<std::decay_t<Integer>> &&
			                           !daw::is_same_v<bool, std::decay_t<Integer>>),
			                          std::nullptr_t> = nullptr>
			static constexpr auto to_string( Integer value ) {
				impl::sv_buff<CharT, std::numeric_limits<Integer>::digits10> result{};

				if( value < 0 ) {
					result.push_back( impl::get_minus<CharT>{}( ) );
					value *= -1;
				}

				auto max10 = impl::pow10<Integer>( impl::num_digits<Integer>( value ) );
				while( max10 > 0 ) {
					auto const tmp = ( value / max10 ) * max10;
					CharT const cur_digit =
					  impl::get_zero<CharT>{}( ) + static_cast<CharT>( tmp / max10 );

					result.push_back( cur_digit );
					value -= tmp;
					max10 /= 10;
				}
				return result;
			}

			template<typename CharT, typename Bool,
			         std::enable_if_t<daw::is_same_v<bool, std::decay_t<Bool>>,
			                          std::nullptr_t> = nullptr>
			constexpr auto to_string( Bool b ) noexcept {
				impl::sv_buff<CharT, 5> result{};
				if( b ) {
					result.push_back( "true" );
				} else {
					result.push_back( "false" );
				}
				return result;
			}

			template<typename CharT>
			constexpr auto to_string( CharT c ) noexcept {
				impl::sv_buff<CharT, 1> result{};
				result.push_back( c );
				return result;
			}

			template<typename CharT, typename Float,
			         std::enable_if_t<daw::is_floating_point_v<Float>,
			                          std::nullptr_t> = nullptr>
			constexpr auto to_string( Float value, int round_to = -1 ) {
				using value_t = std::decay_t<Float>;
				impl::sv_buff<CharT,
				              ( std::numeric_limits<value_t>::max_exponent10 + 2 )>
				  result{};

				if( round_to < 0 ) {
					round_to = std::numeric_limits<value_t>::max_digits10;
				}
				if( value == static_cast<value_t>( 0.0 ) ) {
					result.push_back( '0' );
					return result;
				}
				if( impl::is_nan( value ) ) {
					result.push_back( "nan" );
					return result;
				}
				if( value < static_cast<value_t>( 0 ) ) {
					result.push_back( '-' );
					value = -value;
				}
				if( impl::is_inf( value ) ) {
					result.push_back( "inf" );
					return result;
				}
				auto const e = impl::find_whole_exponent( value );
				auto p10 = impl::pow10<value_t>( e + 1 );
				auto tmp_value = value;

				for( uint16_t ex = 0; ex <= e; ++ex ) {
					auto digit = static_cast<char>( tmp_value / p10 );
					daw::exception::precondition_check<impl::unexpected_state>(
					  digit >= 0 && digit <= 10 );

					if( ex > std::numeric_limits<value_t>::max_digits10 ) {
						digit = 0;
					}
					result.push_back( impl::get_zero<CharT>{}( ) + digit );

					tmp_value -= static_cast<value_t>( digit ) * p10;
					p10 /= 10.0;
				}

				if( e >= std::numeric_limits<value_t>::max_digits10 ||
				    tmp_value <= std::numeric_limits<value_t>::min( ) ) {
					return result;
				}
				result.push_back( impl::get_decimal<CharT>{}( ) );

				auto const num_dec_digits = impl::min_value(
				  impl::min_value( round_to, impl::max_fractional_digits<Float> ),
				  std::numeric_limits<Float>::max_digits10 - e );
				for( int n = 0; n < num_dec_digits &&
				                tmp_value > std::numeric_limits<Float>::min( );
				     ++n ) {
					auto digit =
					  static_cast<char>( tmp_value * static_cast<Float>( 10 ) );
					result.push_back( impl::get_zero<CharT>{}( ) + digit );
					tmp_value -= static_cast<Float>( digit ) / static_cast<Float>( 10 );
					tmp_value *= static_cast<Float>( 10 );
				}
				return result;
			}

			namespace impl {
				template<typename T>
				using has_tostring_detect2 =
				  decltype( to_string( std::declval<T const &>( ) ) );

				template<typename T>
				constexpr bool has_tostring2_v =
				  daw::is_detected_v<impl::has_tostring_detect2, T>;

				using daw::io::ostream_converters::to_string;
				template<typename T>
				using has_tostring_detect =
				  decltype( to_string( std::declval<T const &>( ) ) );

				template<typename T>
				constexpr bool has_tostring_v =
				  daw::is_detected_v<has_tostring_detect, T>;

			} // namespace impl
		}   // namespace ostream_converters
	}     // namespace io
} // namespace daw
