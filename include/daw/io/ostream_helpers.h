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

#include <daw/cpp_17.h>
#include <daw/daw_string_view.h>

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

	template<typename T>
	struct as_int_t {
		T value;

		constexpr operator T const &( ) const noexcept {
			return value;
		}

		constexpr operator T &( ) noexcept {
			return value;
		}
	};

	template<typename Integer>
	constexpr as_int_t<Integer> as_int( Integer &&i ) noexcept {
		return {std::forward<Integer>( i )};
	}

	template<typename>
	struct is_asint_t : std::false_type {};

	template<typename Integer>
	struct is_asint_t<as_int_t<Integer>> : std::true_type {};

	template<typename T>
	constexpr bool is_asint_v = is_asint_t<T>::value;

	template<typename>
	struct char_traits;

	template<>
	struct char_traits<char> {
		static constexpr char const decimal_point = '.';
		static constexpr char const minus = '-';

		static constexpr daw::basic_string_view<char> nan( ) {
			return "nan";
		}

		static constexpr daw::basic_string_view<char> inf( ) {
			return "inf";
		}

		template<typename T>
		static constexpr char get_char_digit( T value ) noexcept {
			daw::exception::dbg_precondition_check(
			  0 <= value && value <= 9, "Only valid single digit numbers" );
			return '0' + static_cast<char>( value );
		}

		static constexpr char get_char( char c ) noexcept {
			return c;
		}
	};

	template<>
	struct char_traits<wchar_t> {
		static constexpr wchar_t const decimal_point = L'.';
		static constexpr wchar_t const minus = L'-';

		static constexpr daw::basic_string_view<wchar_t> nan( ) {
			return L"nan";
		}

		static constexpr daw::basic_string_view<wchar_t> inf( ) {
			return L"inf";
		}

		template<typename T>
		static constexpr wchar_t get_char_digit( T value ) {
			return L'0' + static_cast<wchar_t>( value );
		}

		static constexpr wchar_t get_char( char c ) noexcept {
			return L'\0' + c;
		}
	};
} // namespace daw
