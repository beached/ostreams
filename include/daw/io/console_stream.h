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
		enum class output_stream_type_destinations { output, error };

		template<typename CharT, output_stream_type_destinations Dest>
		struct console_stream {
			using character_t = CharT;

		private:
			inline static FILE *get_handle( ) noexcept {
				switch( Dest ) {
				case output_stream_type_destinations::error:
					return stderr;
				case output_stream_type_destinations::output:
				default:
					return stdout;
				}
			}

		public:
			inline auto operator( )( CharT c ) const noexcept {
				return impl::write_char{}( c, get_handle( ) );
			}

			inline auto operator( )( ::daw::io::impl::accept_asciiz,
			                         CharT const *ptr ) const noexcept {
				return impl::write_char{}( ptr, get_handle( ) );
			}

			template<size_t N>
			inline auto operator( )( CharT const ( &str )[N] ) const noexcept {
				return impl::write_char{}( str, N - 1, get_handle( ) );
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

				impl::write_char{}( str.data( ), str.size( ), get_handle( ) );
			}
		};

		template<typename CharT, output_stream_type_destinations Dest>
		struct supports_output_stream_interface<console_stream<CharT, Dest>>
		  : std::true_type {};
	} // namespace io

#ifdef stdout
	constexpr auto con_out =
	  io::console_stream<char, io::output_stream_type_destinations::output>{};
	constexpr auto con_wout =
	  io::console_stream<wchar_t, io::output_stream_type_destinations::output>{};
#endif
#ifdef stderr
	constexpr auto con_err =
	  io::console_stream<char, io::output_stream_type_destinations::error>{};
	constexpr auto con_werr =
	  io::console_stream<wchar_t, io::output_stream_type_destinations::error>{};
#endif

} // namespace daw
