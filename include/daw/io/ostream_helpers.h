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

#include <daw/daw_traits.h>

namespace daw {
	struct buffer_full_exception {};

	namespace impl {
		template<typename String>
		using has_data_member_detect = decltype( std::declval<String>( ).data( ) );

		template<typename String>
		using has_size_member_detect = decltype( std::declval<String>( ).size( ) );

		template<typename String>
		constexpr bool is_string_like_v = daw::all_true_v<
		  daw::is_detected_v<has_data_member_detect, remove_cvref_t<String>>,
		  daw::is_detected_v<has_size_member_detect, remove_cvref_t<String>>>;

	} // namespace impl
	template<typename CharT>
	struct char_traits;

	template<>
	struct char_traits<char> {
		constexpr static char const decimal_point = '.';
		constexpr static char const zero = '0';
		constexpr static char const minus = '-';

		template<typename T>
		static constexpr char get_char_digit( T value ) noexcept {
			return zero + static_cast<wchar_t>( value );
		}
	};

	template<>
	struct char_traits<wchar_t> {
		constexpr static wchar_t const decimal_point = L'.';
		constexpr static wchar_t const zero = L'0';
		constexpr static wchar_t const minus = L'-';

		template<typename T>
		static constexpr wchar_t get_char_digit( T value ) noexcept {
			return zero + static_cast<wchar_t>( value );
		}
	};
} // namespace daw
