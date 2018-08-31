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

#include <cstddef>
#include <cstdint>
#include <string>

#include <daw/daw_string_view.h>

#include "ostream_helpers.h"

namespace daw {
	template<typename CharT, size_t N>
	struct static_string_t {
		CharT buffer[N + 1] = {static_cast<CharT>( 0 )};
		size_t len = 0;

		constexpr size_t capacity( ) const noexcept {
			return N;
		}

		constexpr static_string_t( ) noexcept = default;

		constexpr CharT *data( ) noexcept {
			return buffer;
		}

		constexpr CharT const *data( ) const noexcept {
			return buffer;
		}

		// This is on purpose
		constexpr operator CharT const *( ) const noexcept {
			return data( );
		}

		constexpr size_t size( ) const noexcept {
			return len;
		}

		constexpr void push_back( CharT c ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + 1 <= capacity( ) );
			buffer[len++] = c;
		}

		constexpr static_string_t &operator+=( CharT c ) {
			push_back( c );
			return *this;
		}

		constexpr void push_back( daw::basic_string_view<CharT> sv ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + sv.size( ) <= capacity( ) );
			for( auto c : sv ) {
				buffer[len++] = c;
			}
		}

		constexpr static_string_t &operator+=( daw::basic_string_view<CharT> sv ) {
			push_back( sv );
			return *this;
		}

		constexpr operator daw::basic_string_view<CharT>( ) const noexcept {
			return daw::basic_string_view<CharT>( buffer, len );
		}

		constexpr void clear( ) noexcept {
			for( size_t n = 0; n < size( ); ++n ) {
				buffer[n] = 0;
			}
			len = 0;
		}

		constexpr void resize( size_t new_size, bool set_zero = true ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  new_size <= capacity( ) );
			if( set_zero ) {
				for( size_t n = size( ); n < new_size; ++n ) {
					buffer[n] = 0;
				}
				for( size_t n = new_size; n < size( ); ++n ) {
					buffer[n] = 0;
				}
			}
			len = new_size;
		}

		constexpr void shrink_to_fit( ) noexcept {
			for( ; len > 0 && buffer[len] == 0; --len ) {}
		}
	};

	template<typename CharT, size_t N>
	constexpr static_string_t<CharT, N> const &
	to_string( static_string_t<CharT, N> const &str ) noexcept {
		return str;
	}

	template<typename CharT, size_t N>
	constexpr static_string_t<CharT, N> &&
	to_string( static_string_t<CharT, N> &&str ) noexcept {
		return std::move( str );
	}
} // namespace daw
