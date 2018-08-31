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
#include <cwchar>
#include <string>

#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostreams.h"

namespace daw {
	namespace io {
		enum class file_open_flags : bool { Write, Append };

		namespace impl {
			struct write_char {
				constexpr write_char( ) noexcept = default;

				inline auto operator( )( char c, FILE *f ) const noexcept {
					return putc( c, f );
				}

				inline auto operator( )( wchar_t c, FILE *f ) const noexcept {
					return putwc( c, f );
				}
			};
		} // namespace impl
		template<typename CharT>
		class file_stream {
			FILE *m_file_handle;
			bool m_is_owner = true;

		public:
			// OutputStream Interface
			using character_t = CharT;

			inline explicit file_stream( std::basic_string<CharT> const &file_name,
			                             file_open_flags flags ) noexcept
			  : m_file_handle(
			      fopen( file_name.c_str( ),
			             ( flags == file_open_flags::Write ? "w" : "a" ) ) ) {}

			inline explicit file_stream( FILE *fp, bool take_ownership )
			  : m_file_handle( fp )
			  , m_is_owner( take_ownership ) {}

			// OutputStream Interface
			inline auto operator( )( CharT c ) const noexcept {
				return impl::write_char{}( c, m_file_handle );
			}

			// OutputStream Interface
			template<typename String,
			         std::enable_if_t<(::daw::impl::is_string_like_v<String> &&
			                           !::daw::traits::is_character_v<String>),
			                          std::nullptr_t> = nullptr>
			constexpr void operator( )( String &&str ) const noexcept {
				static_assert(
				  daw::is_same_v<remove_cvref_t<CharT>,
				                 remove_cvref_t<decltype( *str.data( ) )>>,
				  "String's data( ) character type must match that of output stream" );

				auto ptr = str.data( );
				auto const sz = str.size( );
				for( size_t n = 0; n < sz; ++n ) {
					impl::write_char{}( *ptr++, m_file_handle );
				}
			}

			inline explicit operator bool( ) const noexcept {
				return static_cast<bool>( m_file_handle );
			}

			inline FILE *native_handle( ) const {
				return m_file_handle;
			}

			inline void flush( ) noexcept {
				fflush( m_file_handle );
			}

			inline void close( ) noexcept {
				auto tmp = std::exchange( m_file_handle, nullptr );
				if( tmp ) {
					fflush( tmp );
					fclose( tmp );
				}
			}

			inline ~file_stream( ) {
				if( m_is_owner ) {
					close( );
				}
			}

			inline file_stream( file_stream &&other ) noexcept
			  : m_file_handle( other.m_file_handle )
			  , m_is_owner( std::exchange( other.m_is_owner, false ) ) {}

			inline file_stream &operator=( file_stream &&rhs ) noexcept {
				if( this != &rhs ) {
					if( m_is_owner ) {
						close( );
					}
					m_is_owner = std::exchange( rhs.m_is_owner, false );
					m_file_handle = rhs.m_file_handle;
				}
				return *this;
			}

			file_stream( file_stream const & ) = delete;
			file_stream &operator=( file_stream const & ) = delete;
		};

		template<typename CharT>
		struct supports_output_stream_interface<file_stream<CharT>>
		  : std::true_type {};
	} // namespace io

	template<typename CharT, typename f_char_t, size_t N>
	auto make_file_stream(
	  f_char_t const ( &file_name )[N],
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return io::file_stream<CharT>(
		  std::basic_string<f_char_t>( file_name, N - 1 ), flags );
	}

	template<typename CharT, typename f_char_t>
	auto make_file_stream(
	  std::basic_string<f_char_t> const &file_name,
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return io::file_stream<CharT>( file_name, flags );
	}

	template<typename CharT,
	         std::enable_if_t<(daw::is_same_v<char, remove_cvref_t<CharT>> ||
	                           daw::is_same_v<wchar_t, remove_cvref_t<CharT>>),
	                          std::nullptr_t> = nullptr>
	auto make_file_stream(
	  CharT const *file_name,
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return io::file_stream<CharT>( std::basic_string<CharT>( file_name ),
		                               flags );
	}

	template<typename CharT = char>
	inline auto make_file_stream( FILE *fp, bool take_ownership = false ) {

		return io::file_stream<CharT>( fp, take_ownership );
	}
} // namespace daw
