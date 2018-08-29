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

#include <stdexcept>
#include <experimental/type_traits>

using std::experimental::is_detected_v;

namespace daw {
	struct buffer_full_exception {};

	template<typename ExceptionType = std::runtime_error, typename... Args>
	[[noreturn]] void daw_throw( Args &&... args ) {
#if defined( __cpp_exceptions ) || defined( __EXCEPTIONS ) ||                  \
  defined( _CPPUNWIND )
		throw ExceptionType( std::forward<Args>( args )... );
#else
		std::terminate( );
#endif
	}

	namespace impl {
		template<typename String>
		using has_data_member_detect = decltype( std::declval<String>( ).data( ) );

		template<typename String>
		using has_size_member_detect = decltype( std::declval<String>( ).size( ) );

		template<typename String>
		constexpr bool is_string_like_v =
		  is_detected_v<has_data_member_detect, String>
		    &&std::experimental::is_detected_v<has_size_member_detect, String>;

	} // namespace impl
} // namespace daw

