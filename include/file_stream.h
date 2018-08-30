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

#include "ostreams.h"

namespace daw {
	namespace io {
		enum class file_open_flags : bool { Write, Append };

		namespace impl {
			class fileout_callable {
				FILE *m_file_handle = nullptr;
				public:
				constexpr fileout_callable( ) noexcept = default;

				template<typename CharT>
				explicit fileout_callable( std::basic_string<CharT> const &file_name,
				                           file_open_flags flags ) noexcept
				  : m_file_handle(
				      fopen( file_name.c_str( ),
				             ( flags == file_open_flags::Write ? "w" : "a" ) ) ) {}

				inline void operator( )( char c ) const noexcept {
					fputc( c, m_file_handle );
				}

				inline void operator( )( wchar_t c ) const noexcept {
					fputwc( c, m_file_handle );
				}

				inline explicit operator bool( ) const noexcept {
					return static_cast<bool>( m_file_handle );
				}

				inline FILE *native_handle( ) const {
					return m_file_handle;
				}

				void close( ) noexcept {
					auto tmp = std::exchange( m_file_handle, nullptr );
					if( tmp ) {
						fclose( tmp );
					}
				}

				~fileout_callable( ) {
					close( );
				}

				fileout_callable( fileout_callable && ) noexcept = default;
				fileout_callable &operator=( fileout_callable && ) noexcept = default;

				fileout_callable( fileout_callable const & ) = delete;
				fileout_callable &operator=( fileout_callable const & ) = delete;
				template<typename CharT>
				inline void operator( )( daw::basic_string_view<CharT> str ) const
				  noexcept {
					for( auto c : str ) {
						putchar( c );
					}
				}
			};
		} // namespace impl
	}   // namespace io

	template<typename CharT, size_t n>
	auto make_file_stream(
	  CharT const ( &file_name )[n],
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return daw::io::make_output_stream<CharT>(
		  daw::io::impl::fileout_callable( file_name.to_string( ), flags ) );
	}

	template<typename CharT>
	auto make_file_stream(
	  std::basic_string<CharT> const &file_name,
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return daw::io::make_output_stream<CharT>(
		  daw::io::impl::fileout_callable( file_name, flags ) );
	}

	template<typename CharT>
	auto make_file_stream(
	  CharT const *file_name,
	  io::file_open_flags flags = io::file_open_flags::Write ) noexcept {

		return make_file_stream( std::basic_string<CharT>( file_name ),
		                         flags );
	}
} // namespace daw
