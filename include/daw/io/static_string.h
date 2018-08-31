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

#include <daw/daw_string_view.h>

#include "ostream_helpers.h"

namespace daw {
	template<typename CharT, size_t Capacity>
	class static_string_t {
		CharT buffer[Capacity + 1] = {0};
		size_t len = 0;

	public:
		using value_type = CharT;
		using size_type = size_t;
		using reference = CharT &;
		using const_reference = CharT const &;
		using pointer = CharT *;
		using const_pointer = CharT const *;
		using iterator = pointer;
		using const_iterator = const_pointer;

		constexpr static_string_t( ) noexcept = default;

		template<size_type N>
		constexpr static_string_t( value_type const ( &str )[N] ) noexcept
		  : len( N - 1 ) {
			static_assert( ( N - 1 ) <= Capacity,
			               "Not enough space in static_string_t" );
			daw::algorithm::copy_n( str, buffer, N - 1 );
		}

		constexpr size_type capacity( ) const noexcept {
			return Capacity;
		}

		constexpr pointer data( ) noexcept {
			return buffer;
		}

		constexpr const_pointer data( ) const noexcept {
			return buffer;
		}

		// This is implicit on purpose
		constexpr operator const_pointer( ) const noexcept {
			return data( );
		}

		constexpr size_type size( ) const noexcept {
			return len;
		}

		constexpr void push_back( value_type c ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + 1 <= capacity( ) );
			buffer[len++] = c;
		}

		constexpr static_string_t &operator+=( value_type c ) {
			push_back( c );
			return *this;
		}

		constexpr void push_back( daw::basic_string_view<value_type> sv ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + sv.size( ) <= capacity( ) );

			for( auto c : sv ) {
				buffer[len++] = c;
			}
		}

		constexpr static_string_t &
		operator+=( daw::basic_string_view<value_type> sv ) {
			push_back( sv );
			return *this;
		}

		constexpr operator daw::basic_string_view<value_type>( ) const noexcept {
			return daw::basic_string_view<value_type>( buffer, len );
		}

		constexpr void clear( ) noexcept {
			for( size_type n = 0; n < size( ); ++n ) {
				buffer[n] = 0;
			}
			len = 0;
		}

		constexpr void resize( size_type new_size, bool set_zero = true ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  new_size <= capacity( ) );

			if( set_zero ) {
				for( size_type n = size( ); n < new_size; ++n ) {
					buffer[n] = 0;
				}
				for( size_type n = new_size; n < size( ); ++n ) {
					buffer[n] = 0;
				}
			}
			len = new_size;
		}

		constexpr void shrink_to_fit( ) noexcept {
			for( ; len > 0 && buffer[len] == 0; --len ) {}
		}

		constexpr iterator begin( ) noexcept {
			return data( );
		}

		constexpr const_iterator begin( ) const noexcept {
			return data( );
		}

		constexpr const_iterator cbegin( ) const noexcept {
			return data( );
		}

		constexpr iterator end( ) noexcept {
			return data( ) + size( );
		}

		constexpr const_iterator end( ) const noexcept {
			return data( ) + size( );
		}

		constexpr const_iterator cend( ) const noexcept {
			return data( ) + size( );
		}

		constexpr reference operator[]( size_type pos ) noexcept {
			return buffer[pos];
		}

		constexpr const_reference operator[]( size_type pos ) const noexcept {
			return buffer[pos];
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
