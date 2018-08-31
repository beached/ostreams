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

#include "daw/io/console_stream.h"

#include "daw/io/memory_stream.h"
#include "daw/io/static_string.h"

namespace {
	struct A {};

	// Only need to define a structure that has data( ) and size( ) members
	// This includes std::basic_string, std::basic_string_view,
	// daw::basic_string_view
	template<typename CharT>
	constexpr auto to_string( A ) noexcept {
		daw::static_string_t<CharT, 1> result = "A";
		return result;
	}

	template<size_t BUFF_SIZE = 76, typename Float>
	constexpr auto test( Float f ) {
		// If buffer isn't large enough a buffer_full_exception will be thrown
		daw::static_string_t<char, BUFF_SIZE> buffer{};

		// Indicate we want to have the full size of static_string as in use, and
		// do not default init the char's to 0
		buffer.resize( buffer.capacity( ), false );

		auto buff_os =
		  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

		buff_os << A{} << " The number is: " << f << ". " << 5
		        << " times number is " << ( static_cast<Float>( 5 ) * f ) << '\n';

		// Reset static_string size to last non-zero item in array
		buffer.shrink_to_fit( );
		return buffer;
	}
} // namespace

constexpr static auto const test_result = test( 1234560.435333 );

auto const &test2( ) {
	return test_result;
}

int main( int, char ** ) {
	daw::con_out << test2( ) << '\n';
	return 0;
}
