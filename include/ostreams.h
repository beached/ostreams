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
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

#include <daw/daw_exception.h>
#include <daw/daw_traits.h>

#include "ostream_converters.h"
#include "ostream_helpers.h"

namespace daw {
	namespace io {
		template<typename CharT, typename OutputCallback>
		class basic_output_stream {
			OutputCallback m_out;

		public:
			using reference = basic_output_stream &;
			using const_reference = basic_output_stream const &;

			constexpr explicit basic_output_stream( OutputCallback &oi ) noexcept(
			  daw::is_nothrow_copy_constructible_v<OutputCallback> )
			  : m_out( oi ) {}

			constexpr explicit basic_output_stream( OutputCallback &&oi ) noexcept(
			  daw::is_nothrow_move_constructible_v<OutputCallback> )
			  : m_out( std::move( oi ) ) {}

			static constexpr bool is_nothrow_on_char_v =
			  noexcept( m_out( std::declval<CharT>( ) ) );

			static constexpr bool is_nothrow_on_sv_v =
			  noexcept( m_out( std::declval<std::basic_string_view<CharT>>( ) ) );

			template<typename String,
			         std::enable_if_t<( ::daw::impl::is_string_like_v<String> &&
			                            !daw::is_same_v<CharT, String>),
			                          std::nullptr_t> = nullptr>
			constexpr reference
			operator( )( String &&str ) noexcept( is_nothrow_on_sv_v ) {
				static_assert( daw::is_same_v<std::decay_t<CharT>,
				                              std::decay_t<decltype( *str.data( ) )>>,
				               "str must contain CharT data" );

				m_out( std::basic_string_view<CharT>( str.data( ), str.size( ) ) );
				return *this;
			}

			constexpr reference
			operator( )( CharT c ) noexcept( is_nothrow_on_char_v ) {
				m_out( c );
				return *this;
			}

			constexpr const_reference operator( )( CharT c ) const
			  noexcept( is_nothrow_on_char_v ) {
				m_out( c );
				return *this;
			}

			constexpr OutputCallback &get_underlying_stream( ) noexcept {
				return m_out;
			}

			constexpr OutputCallback const &get_underlying_stream( ) const noexcept {
				return m_out;
			}
		};

		namespace impl {
			using ::daw::io::basic_output_stream;

			template<typename CharT, typename OutputCallback, typename T>
			using has_operator_lsh_lsh_detect = decltype( operator<<(
			  std::declval<
			    ::daw::io::basic_output_stream<CharT, OutputCallback> const &>( ),
			  std::declval<T const &>( ) ) );

			struct display_value {
				template<typename CharT, typename OutputCallback, typename T,
				         std::enable_if_t<
				           (!daw::is_same_v<std::decay_t<CharT>, std::decay_t<T>>),
				           std::nullptr_t> = nullptr>
				static constexpr void
				display( basic_output_stream<CharT, OutputCallback> &os, T &&value ) {
					using ::daw::io::ostream_converters::to_string;
					os( to_string<CharT>( std::forward<T>( value ) ) );
				}

				template<typename CharT, typename OutputCallback>
				static constexpr void
				display( basic_output_stream<CharT, OutputCallback> &os, CharT c ) {
					os( c );
				}

				template<typename CharT, typename OutputCallback>
				static constexpr void
				display( basic_output_stream<CharT, OutputCallback> &os,
				         CharT const *str, size_t N ) {
					os( std::basic_string_view<CharT>( str, N ) );
				}
			};

			template<typename CharT, typename OutputCallback, typename Arg>
			struct display_struct {
				basic_output_stream<CharT, OutputCallback> *os;
				Arg arg;

				constexpr void operator( )( ) {
					display_value::display( *os, arg );
				}
			};

			struct stdout_callable {
				constexpr stdout_callable( ) noexcept = default;

				inline void operator( )( char c ) const noexcept {
					putchar( c );
				}

				inline void operator( )( wchar_t c ) const noexcept {
					putwchar( c );
				}

				template<typename CharT>
				inline void operator( )( std::basic_string_view<CharT> str ) const
				  noexcept {
					for( auto c : str ) {
						putchar( c );
					}
				}
			};
		} // namespace impl

		template<typename CharT = char>
		class char_buffer_stream {
			static_assert( !daw::is_const_v<CharT>,
			               "Cannot write to a const buffer" );
			size_t m_capacity = 0;
			size_t m_position = 0;
			CharT *m_first = nullptr;

		public:
			constexpr char_buffer_stream( ) noexcept = default;
			constexpr char_buffer_stream( CharT *buffer, size_t capacity ) noexcept
			  : m_capacity( capacity )
			  , m_first( buffer ) {}

			constexpr CharT *data( ) const noexcept {
				return m_first;
			}

			constexpr size_t size( ) const noexcept {
				return m_position;
			}

			constexpr bool empty( ) const noexcept {
				return size( ) == 0;
			}

			constexpr size_t capacity( ) const noexcept {
				return m_capacity;
			}

			constexpr bool is_full( ) const noexcept {
				return size( ) == capacity( );
			}

		private:
			constexpr void append( CharT c ) noexcept {
				m_first[m_position++] = c;
			}

		public:
			constexpr void operator( )( CharT c ) {
				daw::exception::precondition_check<buffer_full_exception>(
				  !is_full( ) );
				append( c );
			}

			constexpr void operator( )( std::basic_string_view<CharT> sv ) {
				daw::exception::precondition_check<buffer_full_exception>(
				  sv.size( ) <= capacity( ) - size( ) );
				for( auto c : sv ) {
					append( c );
				}
			}

			constexpr void reset( ) const noexcept {
				m_position = 0;
			}

			constexpr std::basic_string_view<CharT> to_string_view( ) const noexcept {
				return std::basic_string_view<CharT>( data( ), size( ) );
			}

			std::basic_string<CharT> to_string( ) const {
				return std::basic_string<CharT>( data( ), size( ) );
			}

			constexpr basic_output_stream<CharT, char_buffer_stream> to_stream( ) {
				return basic_output_stream<CharT, char_buffer_stream>( *this );
			}
		};

		template<typename CharT, typename OutputCallback>
		constexpr auto make_output_stream( OutputCallback &&oi ) {
			return basic_output_stream<CharT, OutputCallback>(
			  std::forward<OutputCallback>( oi ) );
		}

		template<typename CharT>
		constexpr auto make_memory_buffer_stream( CharT *buffer,
		                                          size_t capacity ) noexcept {
			return ::daw::io::make_output_stream<CharT>(
			  ::daw::io::char_buffer_stream<CharT>( buffer, capacity ) );
		}

		template<typename CharT, typename OutputCallback, size_t N>
		constexpr basic_output_stream<CharT, OutputCallback> &
		operator<<( basic_output_stream<CharT, OutputCallback> &os,
		            CharT const ( &str )[N] ) {
			os( std::basic_string_view<CharT>( str, N - 1 ) );
			return os;
		}

		template<typename CharT, typename OutputCallback, typename T,
		         std::enable_if_t<(ostream_converters::impl::has_tostring_v<T>),
		                          std::nullptr_t> = nullptr>
		constexpr basic_output_stream<CharT, OutputCallback> &
		operator<<( basic_output_stream<CharT, OutputCallback> &os, T &&value ) {
			::daw::io::impl::display_struct<CharT, OutputCallback, T>{
			  &os, std::forward<T>( value )}( );
			return os;
		}

		template<typename CharT>
		constexpr auto make_con_out( ) noexcept {
			return daw::io::make_output_stream<CharT>(
			  daw::io::impl::stdout_callable( ) );
		}
	} // namespace io

	static auto con_out = io::make_con_out<char>( );
	static auto con_wout = io::make_con_out<wchar_t>( );
} // namespace daw
