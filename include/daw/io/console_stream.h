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

#include <cstdio>

#include "file_stream.h"

namespace daw {
	namespace io {
		template<typename CharT>
		struct console_stream {
			FILE *m_file_handle;
			using character_t = CharT;

			explicit constexpr console_stream( FILE *f ) noexcept
			  : m_file_handle( f ) {}

			inline auto operator( )( CharT c ) const noexcept {
				return impl::write_char{}( c, m_file_handle );
			}

			inline auto operator( )( ::daw::io::impl::accept_asciiz,
			                         CharT const *ptr ) const noexcept {
				return impl::write_char{}( ptr, m_file_handle );
			}

			template<size_t N>
			inline auto operator( )( CharT const ( &str )[N] ) const noexcept {
				return impl::write_char{}( str, N - 1, m_file_handle );
			}
			// OutputStream Interface
			template<typename String,
			         std::enable_if_t<( ::daw::impl::is_string_like_v<String> &&
			                            !::daw::traits::is_character_v<String>),
			                          std::nullptr_t> = nullptr>
			void operator( )( String &&str ) const noexcept {
				static_assert(
				  daw::is_same_v<remove_cvref_t<CharT>,
				                 remove_cvref_t<decltype( *str.data( ) )>>,
				  "String's data( ) character type must match that of output stream" );

				impl::write_char{}( str.data( ), str.size( ), m_file_handle );
			}

			constexpr FILE *native_handle( ) const {
				return m_file_handle;
			}
		};

		template<typename CharT>
		struct supports_output_stream_interface<console_stream<CharT>>
		  : std::true_type {};

		namespace impl {
			template<typename CharT>
			console_stream<CharT> *get_console_stream( FILE *f ) noexcept {
				static auto const stream = std::make_unique<console_stream<CharT>>( f );
				return stream.get( );
			}
		} // namespace impl

		struct console_stdout_char_t {};
		struct console_stderr_char_t {};
		struct console_stdout_wchar_t {};
		struct console_stderr_wchar_t {};

		// Account for pointer to output streams
		template<size_t N>
		constexpr console_stdout_char_t operator<<( console_stdout_char_t os,
		                                            char const ( &str )[N] ) {
			auto const cs = console_stream<char>( stdout );
			cs( str );
			return os;
		}

		template<typename T>
		constexpr console_stdout_char_t operator<<( console_stdout_char_t os,
		                                            T &&value ) {
			auto cs = console_stream<char>( stdout );
			operator<<( cs, std::forward<T>( value ) );
			return os;
		}

		template<size_t N>
		constexpr console_stderr_char_t operator<<( console_stderr_char_t os,
		                                            char const ( &str )[N] ) {
			auto const cs = console_stream<char>( stderr );
			cs( *&str, N - 1 );
			return os;
		}

		template<typename T>
		constexpr console_stderr_char_t operator<<( console_stderr_char_t os,
		                                            T &&value ) {
			auto cs = console_stream<char>( stderr );
			operator<<( cs, std::forward<T>( value ) );
			return os;
		}

		// wchar_t
		template<size_t N>
		constexpr console_stdout_wchar_t operator<<( console_stdout_wchar_t os,
		                                             wchar_t const ( &str )[N] ) {
			auto const cs = console_stream<wchar_t>( stdout );
			cs( str );
			return os;
		}

		template<typename T>
		constexpr console_stdout_wchar_t operator<<( console_stdout_wchar_t os,
		                                             T &&value ) {
			auto cs = console_stream<wchar_t>( stdout );
			operator<<( cs, std::forward<T>( value ) );
			return os;
		}

		template<size_t N>
		constexpr console_stderr_wchar_t operator<<( console_stderr_wchar_t os,
		                                             wchar_t const ( &str )[N] ) {
			auto const cs = console_stream<wchar_t>( stderr );
			cs( *&str, N - 1 );
			return os;
		}

		template<typename T>
		constexpr console_stderr_wchar_t operator<<( console_stderr_wchar_t os,
		                                             T &&value ) {
			auto cs = console_stream<wchar_t>( stderr );
			operator<<( cs, std::forward<T>( value ) );
			return os;
		}
	} // namespace io

#ifdef stdout
	static auto const con_out = io::console_stdout_char_t{};
	static auto const con_wout = io::console_stdout_wchar_t{};
#endif
#ifdef stderr
	static auto const con_err = io::console_stderr_char_t{};
	static auto const con_werr = io::console_stderr_wchar_t{};
#endif

} // namespace daw
