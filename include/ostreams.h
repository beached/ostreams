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

#include <type_traits>

#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostream_converters.h"
#include "ostream_helpers.h"

namespace daw {
	namespace io {
		template<typename>
		struct is_output_stream : std::false_type {};

		template<typename T>
		constexpr bool is_output_stream_v =
		  is_output_stream<remove_cvref_t<T>>::value;

		template<typename CharT, typename OutputCallback>
		class basic_output_stream {
			OutputCallback m_out;

		public:
			using character_t = CharT;
			using callback_t = OutputCallback;
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
			  noexcept( m_out( std::declval<daw::basic_string_view<CharT>>( ) ) );

			template<typename String,
			         std::enable_if_t<( ::daw::impl::is_string_like_v<String> &&
			                            !daw::is_same_v<CharT, String>),
			                          std::nullptr_t> = nullptr>
			constexpr reference
			operator( )( String &&str ) noexcept( is_nothrow_on_sv_v ) {
				static_assert( daw::is_same_v<std::decay_t<CharT>,
				                              std::decay_t<decltype( *str.data( ) )>>,
				               "str must contain CharT data" );

				m_out( daw::basic_string_view<CharT>( str.data( ), str.size( ) ) );
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

			constexpr OutputCallback const & raw_handle( ) const noexcept {
				return m_out;
			}

			constexpr OutputCallback & raw_handle( ) noexcept {
				return m_out;
			}
		};

		template<typename CharT, typename OutputCallback>
		struct is_output_stream<basic_output_stream<CharT, OutputCallback>>
		  : std::true_type {};

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
			};

			template<typename OutputStream, typename T,
			         std::enable_if_t<is_output_stream_v<OutputStream>,
			                          std::nullptr_t> = nullptr>
			struct display_struct {
				daw::remove_cvref_t<OutputStream> *os;
				T arg;

				constexpr void operator( )( ) {
					display_value::display( *os, arg );
				}
			};
		} // namespace impl

		template<typename CharT, typename OutputCallback>
		constexpr auto make_output_stream( OutputCallback &&oi ) {
			return basic_output_stream<CharT, OutputCallback>(
			  std::forward<OutputCallback>( oi ) );
		}

		template<typename OutputStream, typename T,
		         std::enable_if_t<is_output_stream_v<OutputStream>,
		                          std::nullptr_t> = nullptr>
		constexpr OutputStream &operator<<( OutputStream &os, T &&value ) {

			::daw::io::impl::display_struct<OutputStream, T>{
			  &os, std::forward<T>( value )}( );

			return os;
		}
	} // namespace io
} // namespace daw
