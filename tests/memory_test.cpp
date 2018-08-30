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

#include <daw/daw_string_view.h>

#include "console_stream.h"
#include "memory_stream.h"

namespace {
	template<size_t buff_sz, typename CharT = char>
	class buffer_t {
		CharT value[buff_sz + 1] = {0};
		static constexpr size_t sz = buff_sz;

	public:
		constexpr size_t size( ) const noexcept {
			return sz;
		}
		constexpr CharT const *data( ) const noexcept {
			return value;
		}
		constexpr CharT *data( ) noexcept {
			return value;
		}

		constexpr buffer_t( ) noexcept = default;

		constexpr operator CharT const *( ) const noexcept {
			return data( );
		}
	};

	struct A {};

	// Only need to define a structure that has data( ) and size( ) members
	// This includes std::basic_string, std::basic_string_view,
	// daw::basic_string_view
	template<typename CharT>
	constexpr auto to_string( A ) noexcept {
		buffer_t<1> result;
		result.data( )[0] = 'A';
		return result;
	}

	template<size_t buff_sz = 74, typename Float>
	constexpr auto test( Float f ) {
		// If buffer isn't large enough a buffer_full_exception will be thrown
		buffer_t<76> buffer{};
		auto buff_os =
		  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );
		buff_os << A{} << " The number is: " << f << ". " << 5
		        << " times number is " << ( static_cast<Float>( 5 ) * f ) << '\n';

		return buffer;
	}
} // namespace

constexpr static auto const test_result = test( 1234560.435333 );

auto const &test2( ) {
	return test_result;
}
int main( int argc, char ** ) {
	daw::con_out << test2( ) << '\n';
	return 0;
}
