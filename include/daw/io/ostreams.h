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

#include <type_traits>

#include <daw/daw_traits.h>

#include "ostream_converters.h"

namespace daw {
	namespace io {
		// Specialize for your OutputStream type to overload from std::true_type to
		// indicate you support the interface as outlined below
		template<typename>
		struct supports_output_stream_interface : std::false_type {};

		/* **** Sample OutputStream ****
		 *
		 *   template<typename CharT>
		 *   struct basic_output_stream {
		 *     using character_t = CharT;
		 *
		 *     void operator( )( CharT c );
		 *		// optional
		 *		void operator( )( accept_asciiz, CharT const * ptr );
		 *
		 *		      // String requires a size( ) and data( ) member
		 *		      template<typename String,
		 *		               std::enable_if_t<(
		 *::daw::impl::is_string_like_v<String> &&
		 *		                                  !::daw::traits::is_character_v<String>),
		 *		                                std::nullptr_t> = nullptr>
		 *		      void operator( )( String &&str );
		 *		    };
		 *
		 */
		namespace impl {
			template<typename CharT, typename OutputStream>
			using has_operator_parens_char =
			  decltype( std::declval<remove_cvref_t<OutputStream>>( )(
			    std::declval<CharT>( ) ) );

			struct accept_asciiz {};

			template<typename CharT, typename OutputStream>
			using has_operator_parens_asciiz =
			  decltype( std::declval<remove_cvref_t<OutputStream>>( )(
			    std::declval<accept_asciiz>( ), std::declval<CharT const *>( ) ) );

			template<typename CharT>
			struct operator_parens_string_t {
				CharT const *data( ) const;
				size_t size( ) const;
			};

			template<typename CharT, typename OutputStream>
			using has_operator_parens_string =
			  decltype( std::declval<remove_cvref_t<OutputStream>>( )(
			    std::declval<operator_parens_string_t<CharT>>( ) ) );

			template<typename CharT, typename OutputStream>
			constexpr bool has_operator_parans_char_v =
			  daw::is_detected_v<has_operator_parens_char, CharT, OutputStream>;

			template<typename CharT, typename OutputStream>
			constexpr bool has_operator_parans_asciiz_v =
			  daw::is_detected_v<has_operator_parens_asciiz, CharT, OutputStream>;

			template<typename CharT, typename OutputStream>
			constexpr bool has_operator_parans_string_v =
			  daw::is_detected_v<has_operator_parens_string, CharT, OutputStream>;
		} // namespace impl

		template<typename OutputStream>
		constexpr bool is_output_stream_v =
		  supports_output_stream_interface<remove_cvref_t<OutputStream>>::value;

		// Stream Operators
		template<
		  typename OutputStream, typename CharT, size_t N,
		  std::enable_if_t<
		    daw::all_true_v<
		      is_output_stream_v<OutputStream>, daw::traits::is_character_v<CharT>,
		      !impl::has_operator_parans_asciiz_v<CharT, OutputStream>>,
		    std::nullptr_t> = nullptr>
		constexpr OutputStream &
		operator<<( OutputStream &os,
		            CharT const ( &str )[N] ) noexcept( noexcept( os( str[0] ) ) ) {
			// Is OutputStream callable with a single CharT
			static_assert( impl::has_operator_parans_char_v<CharT, OutputStream>,
			               "Missing operator( )( CharT ) member on OutputStream" );

			for( size_t n = 0; n < ( N - 1 ); ++n ) {
				os( str[n] );
			}
			return os;
		}

		template<
		  typename OutputStream, typename CharT, size_t N,
		  std::enable_if_t<
		    daw::all_true_v<
		      is_output_stream_v<OutputStream>, daw::traits::is_character_v<CharT>,
		      impl::has_operator_parans_asciiz_v<CharT, OutputStream>>,
		    std::nullptr_t> = nullptr>
		constexpr OutputStream &
		operator<<( OutputStream &os,
		            CharT const ( &str )[N] ) noexcept( noexcept( os( str[0] ) ) ) {
			// Is OutputStream callable with a single CharT
			static_assert( impl::has_operator_parans_char_v<CharT, OutputStream>,
			               "Missing operator( )( CharT ) member on OutputStream" );

			os( impl::accept_asciiz{}, str );
			return os;
		}

		template<typename OutputStream, typename T, typename CharT, size_t N,
		         std::enable_if_t<(is_output_stream_v<OutputStream>),
		                          std::nullptr_t> = nullptr>
		constexpr OutputStream &operator<<( OutputStream &os,
		                                    static_string_t<CharT, N> const &str ) {
			static_assert(
			  daw::is_same_v<remove_cvref_t<typename OutputStream::character_t>,
			                 remove_cvref_t<CharT>>,
			  "Character type in OutputStream does not match that of stirng" );
			os( impl::accept_asciiz{}, str.data( ) );
		}

		template<typename OutputStream, typename T, typename CharT, size_t N,
		         std::enable_if_t<(is_output_stream_v<OutputStream>),
		                          std::nullptr_t> = nullptr>
		constexpr OutputStream &operator<<( OutputStream &os,
		                                    static_string_t<CharT, N> &&str ) {
			static_assert(
			  daw::is_same_v<remove_cvref_t<typename OutputStream::character_t>,
			                 remove_cvref_t<CharT>>,
			  "Character type in OutputStream does not match that of stirng" );
			os( impl::accept_asciiz{}, str.data( ) );
		}

		template<typename OutputStream, typename T,
		         std::enable_if_t<(is_output_stream_v<OutputStream>),
		                          std::nullptr_t> = nullptr>
		constexpr OutputStream &operator<<( OutputStream &os, T &&value ) {
			using CharT = typename OutputStream::character_t;
			// Can type T be called with to_string
			static_assert( ::ostream_converters::has_to_string_v<CharT, T>,
			               "Could not find a valid to_string<CharT> overload" );
			// Is OutputStream callable with a single CharT
			static_assert( impl::has_operator_parans_char_v<CharT, OutputStream>,
			               "Missing operator( )( CharT ) member on OutputStream" );

			// Is OutputStream callable with a String (has size( ) and data( ) member)
			static_assert( impl::has_operator_parans_string_v<CharT, OutputStream>,
			               "Missing operator( )( String ) member on OutputStream" );

			using ::ostream_converters::to_string;
			os( to_string<CharT>( std::forward<T>( value ) ) );
			return os;
		}
	} // namespace io
} // namespace daw
