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

#include <string>

#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostreams.h"

namespace daw {
	namespace io {
		template<typename CharT = char>
		class char_buffer_stream {
			static_assert( !daw::is_const_v<CharT>,
			               "Cannot write to a const buffer" );
			size_t m_capacity = 0;
			size_t m_position = 0;
			CharT *m_first = nullptr;

		public:
			constexpr char_buffer_stream( ) noexcept = default;
			constexpr char_buffer_stream( CharT *buffer, size_t capacity ) noexcept
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
			constexpr void operator( )( CharT c ) {
				daw::exception::precondition_check<buffer_full_exception>(
				  !is_full( ) );
				append( c );
			}

			constexpr void operator( )( daw::basic_string_view<CharT> sv ) {
				daw::exception::precondition_check<buffer_full_exception>(
				  sv.size( ) <= capacity( ) - size( ) );
				for( auto c : sv ) {
					append( c );
				}
			}

			constexpr void reset( ) noexcept {
				m_position = 0;
			}

			constexpr daw::basic_string_view<CharT> to_string_view( ) const noexcept {
				return daw::basic_string_view<CharT>( data( ), size( ) );
			}

			std::basic_string<CharT> to_string( ) const {
				return std::basic_string<CharT>( data( ), size( ) );
			}
		};

		template<typename CharT>
		constexpr auto make_memory_buffer_stream( CharT *buffer,
		                                          size_t capacity ) noexcept {
			return ::daw::io::make_output_stream<CharT>(
			  ::daw::io::char_buffer_stream<CharT>( buffer, capacity ) );
		}
	} // namespace io
} // namespace daw
