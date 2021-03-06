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

#ifndef NOSTRING
#include <daw/daw_string_view.h>
#include <string>
#endif

#include <daw/daw_traits.h>

#include "ostreams.h"

namespace daw {
	namespace io {
		template<typename CharT = char>
		class memory_stream {
			static_assert( !::std::is_const_v<CharT>,
			               "Cannot write to a const buffer" );
			size_t m_capacity = 0;
			size_t m_position = 0;
			CharT *m_first = nullptr;

		public:
			// OutputStream Interface
			using character_t = CharT;

			constexpr memory_stream( ) noexcept = default;
			constexpr memory_stream( CharT *buffer, size_t capacity ) noexcept
			  : m_capacity( capacity )
			  , m_first( buffer ) {}

			constexpr CharT *data( ) const noexcept {
				return m_first;
			}

			constexpr size_t size( ) const noexcept {
				return m_position;
			}

			constexpr bool empty( ) const noexcept {
				return size( ) == 0;
			}

			constexpr size_t capacity( ) const noexcept {
				return m_capacity;
			}

			constexpr bool is_full( ) const noexcept {
				return size( ) == capacity( );
			}

		private:
			constexpr void append( CharT c ) noexcept {
				m_first[m_position++] = c;
			}

		public:
			// OutputStream Interface
			constexpr void operator( )( CharT c ) {
				::daw::exception::precondition_check<buffer_full_exception>(
				  !is_full( ) );
				append( c );
			}

			// OutputStream Interface
			template<typename String,
			         std::enable_if_t<( ::daw::impl::is_string_like_v<String> &&
			                            !::daw::traits::is_character_v<String>),
			                          std::nullptr_t> = nullptr>
			constexpr void operator( )( String &&str ) {
				static_assert(
				  std::is_same_v<remove_cvref_t<CharT>,
				                 remove_cvref_t<decltype( *str.data( ) )>>,
				  "String's data( ) character type must match that of output stream" );

				::daw::exception::precondition_check<buffer_full_exception>(
				  str.size( ) <= capacity( ) - size( ) );

				auto ptr = str.data( );
				auto const sz = str.size( );
				for( size_t n = 0; n < sz; ++n ) {
					append( *ptr++ );
				}
			}

			constexpr void reset( ) noexcept {
				m_position = 0;
			}

#ifndef NOSTRING
			constexpr ::daw::basic_string_view<CharT> to_os_string_view( ) const
			  noexcept {
				return ::daw::basic_string_view<CharT>( data( ), size( ) );
			}

			std::basic_string<CharT> to_os_string( ) const {
				return std::basic_string<CharT>( data( ), size( ) );
			}
		};
#endif
		template<typename CharT>
		struct supports_output_stream_interface<memory_stream<CharT>>
		  : std::true_type {};

		template<typename CharT>
		constexpr auto make_memory_buffer_stream( CharT *buffer,
		                                          size_t capacity ) noexcept {
			return memory_stream<CharT>( buffer, capacity );
		}
	} // namespace io
} // namespace daw
