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

#include "ostreams.h"

namespace daw {
	namespace io {
		namespace impl {
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

		template<typename CharT>
		constexpr auto make_con_out( ) noexcept {
			return daw::io::make_output_stream<CharT>(
			  daw::io::impl::stdout_callable( ) );
		}
	} // namespace io

	static auto con_out = io::make_con_out<char>( );
	static auto con_wout = io::make_con_out<wchar_t>( );
} // namespace daw
