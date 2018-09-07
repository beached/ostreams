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

#include <limits>
#include <string>

#include <daw/daw_exception.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostream_converters_float.h"
#include "ostream_converters_impl.h"
#include "ostream_converters_int.h"
#include "ostream_helpers.h"
#include "static_string.h"

namespace ostream_converters {
	// character pointer
	template<typename CharT, std::enable_if_t<daw::traits::is_character_v<CharT>,
	                                          std::nullptr_t> = nullptr>
	constexpr daw::basic_string_view<CharT>
	to_os_string( CharT const *str ) noexcept {
		return daw::basic_string_view<CharT>( str );
	}

	// string_view
	template<typename CharT>
	constexpr daw::basic_string_view<CharT>
	to_os_string( daw::basic_string_view<CharT> str ) noexcept {
		return str;
	}

	// std::basic_string
	// TODO add overload for the operator<< to use accepts_asciiz
	template<typename CharT>
	daw::basic_string_view<CharT>
	to_os_string( std::basic_string<CharT> const &str ) noexcept {
		return daw::basic_string_view<CharT>( str.data( ), str.size( ) );
	}

	// Bool
	template<typename CharT, typename Bool,
	         std::enable_if_t<
	           daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                           daw::is_same_v<char, daw::remove_cvref_t<CharT>>>,
	           std::nullptr_t> = nullptr>
	constexpr auto to_os_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( "true" );
		}
		return daw::static_string_t<CharT, 5>( "false" );
	}

	template<
	  typename CharT, typename Bool,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                    daw::is_same_v<wchar_t, daw::remove_cvref_t<CharT>>>,
	    std::nullptr_t> = nullptr>
	constexpr auto to_os_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( L"true" );
		}
		return daw::static_string_t<CharT, 5>( L"false" );
	}

	// Single character (char, wchar_t).  Need to be treated separately from
	// other integers
	template<typename CharT,
	         std::enable_if_t<::daw::traits::is_character_v<CharT>,
	                          std::nullptr_t> = nullptr>
	constexpr auto to_os_string( CharT c ) noexcept {
		daw::static_string_t<CharT, 1> result{};
		result.push_back( c );
		return result;
	}
	namespace impl {
		template<typename T>
		using has_to_os_string_detect2 =
		  decltype( to_os_string( std::declval<T const &>( ) ) );

		namespace to_os_string_detect {
			using ::ostream_converters::to_os_string;
			template<typename CharT, typename T>
			using has_to_os_string_detect =
			  decltype( to_os_string<CharT>( std::declval<T const &>( ) ) );
		} // namespace to_os_string_detect
	}   // namespace impl

	template<typename CharT, typename T>
	constexpr bool has_to_os_string_v =
	  daw::is_detected_v<impl::to_os_string_detect::has_to_os_string_detect,
	                     CharT, ::daw::remove_cvref_t<T>>;
} // namespace ostream_converters
