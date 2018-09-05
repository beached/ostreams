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
	namespace impl {
		template<typename CharT>
		constexpr int strncmp( CharT const * lhs, CharT const * rhs, size_t sz ) noexcept {
			while( sz > 0 && *lhs == *rhs ) {
				--sz;
			}
			if( sz > 0 ) {
				if( *lhs < *rhs ) {
					return -1;
				}
				return 1;
			}
			return 0;
		}

		template<typename CharT>
		constexpr CharT * copy_to( CharT const * src, CharT * dst, size_t count ) noexcept {
			while( count > 0 ) {
				*dst = *src;
			}

		}
	}
	template<typename CharT, size_t Capacity>
	struct static_string_t {
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

		template<size_t N>
		constexpr void push_back( value_type const ( &str )[N] ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + ( N - 1 ) <= capacity( ) );

			for( size_t n = 0; n < ( N - 1 ); ++n ) {
				buffer[len++] = str[n];
			}
		}

		template<size_t N>
		constexpr static_string_t &operator+=( value_type const ( &str )[N] ) {
			daw::exception::precondition_check<daw::buffer_full_exception>(
			  len + ( N - 1 ) <= capacity( ) );

			for( size_t n = 0; n < ( N - 1 ); ++n ) {
				buffer[len++] = str[n];
			}
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

		constexpr const_reference front( ) const noexcept {
			return buffer[0];
		}

		constexpr reference front( ) noexcept {
			return buffer[0];
		}

		constexpr const_reference back( ) const noexcept {
			return buffer[size( ) - 1];
		}

		constexpr reference back( ) noexcept {
			return buffer[size( ) - 1];
		}

		template<size_t rhs_size>
		constexpr int compare( static_string_t<CharT, rhs_size> const & rhs ) const noexcept {
			auto result = impl::strncmp( buffer, rhs.buffer, daw::min( size( ), rhs.size( ) ) );
			if( result != 0 ) { return result; }
			if( size( ) < rhs.size( ) ) {
				return -1;
			}
			if( size( ) > rhs.size( ) ) {
				return 1;
			}
			return 0;
		}

		template<size_t N>
		constexpr int compare( CharT const (&rhs)[N] ) const noexcept {
			auto const rhs_size = N-1;
			auto result = impl::strncmp( buffer, rhs, daw::min( size( ), rhs_size ) );
			if( result != 0 ) { return result; }
			if( size( ) < rhs_size ) {
				return -1;
			}
			if( size( ) > rhs_size ) {
				return 1;
			}
			return 0;
		}
	};

	template<typename>
  struct is_static_string_t: std::false_type {};

	template<typename CharT, size_t N>
	struct is_static_string_t<static_string_t<CharT, N>>: std::true_type {};

	template<typename StaticString>
	constexpr bool is_static_string_v = is_static_string_t<StaticString>::value;

	namespace impl {
		template<typename CharT, size_t N, typename StaticString, std::enable_if_t<is_static_string_v<StaticString>, std::nullptr_t> = nullptr>
		constexpr int compare( CharT const (& lhs)[N], StaticString && rhs ) noexcept {
			auto const lhs_size = N - 1;
			auto result = impl::strncmp( lhs, rhs.data( ), daw::min( lhs_size, rhs.size( ) ) );
			if( result != 0 ) { return result; }
			if( lhs_size < rhs.size( ) ) {
				return -1;
			}
			if( lhs_size > rhs.size( ) ) {
				return 1;
			}
			return 0;
		}
	}

	// Using SFINAE as some compilers(clang 5/Apple Clang) have issues in constexpr
	// if the argument is const
	// https://gcc.godbolt.org/z/40xZFh
	// Operator==
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator==( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) == 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator==( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) == 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator==( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) == 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator==( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) == 0;
	}
	// Operator!=
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator!=( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) != 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator!=( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) != 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator!=( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) != 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator!=( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) != 0;
	}
	// Operator <
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator<( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) < 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator<( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) < 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator<( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) < 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator<( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) < 0;
	}
	// Operator>
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator>( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) > 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator>( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) > 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator>( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) > 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator>( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) > 0;
	}
	// Operator>=
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator>=( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) >= 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator>=( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) >= 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator>=( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) >= 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator>=( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) >= 0;
	}
	// Operator<=
	template<typename LhsString, typename RhsString, std::enable_if_t<daw::all_true_v<is_static_string_v<LhsString>, is_static_string_v<RhsString>>, std::nullptr_t> = nullptr>
	constexpr bool operator<=( LhsString && lhs, RhsString && rhs ) noexcept {
		return lhs.compare( rhs ) <= 0;
	}

	template<typename CharT, size_t N, typename LhsString, std::enable_if_t<is_static_string_v<LhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator<=( LhsString && lhs, CharT const (&rhs)[N] ) noexcept {
		return lhs.compare( rhs ) <= 0;
	}

	template<typename CharT, size_t N, typename RhsString, std::enable_if_t<is_static_string_v<RhsString>, std::nullptr_t> = nullptr>
	constexpr bool operator<=( CharT const (&lhs)[N], RhsString && rhs ) noexcept {
		return impl::compare( lhs, std::forward<RhsString>( rhs ) ) <= 0;
	}

	template<typename CharT, size_t lhs_size, size_t rhs_size>
	constexpr bool operator<=( daw::static_string_t<CharT, lhs_size> const & lhs, CharT const (&rhs)[rhs_size] ) noexcept {
		return lhs.compare( rhs ) <= 0;
	}


	template<typename CharT, size_t N>
	constexpr static_string_t<CharT, N> const &
	to_os_string( static_string_t<CharT, N> const &str ) noexcept {
		return str;
	}

	template<typename CharT, size_t N>
	constexpr static_string_t<CharT, N> &&
	to_os_string( static_string_t<CharT, N> &&str ) noexcept {
		return std::move( str );
	}
} // namespace daw
