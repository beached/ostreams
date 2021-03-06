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
#include "daw/io/static_string.h"

namespace {
	struct A {};

	// Only need to define a structure that has data( ) and size( ) members
	// This includes std::basic_string, std::basic_string_view,
	// daw::basic_string_view
	template<typename CharT>
	constexpr daw::static_string_t<CharT, 1> to_os_string( A ) noexcept {
		daw::static_string_t<CharT, 1> result{};
		result += 'A';
		return result;
	}
} // namespace

int main( int, char ** ) {
	daw::con_out << A{} << " The number is: " << 1234560.435333 << ". " << 5
	             << " times number is " << ( 5.0f * 1234560.435333f ) << '\n';

	daw::con_wout << A{} << L" The number is: " << 1234560.435333 << L". " << 5
	              << L" times number is " << ( 5.0f * 1234560.435333f ) << '\n';
	return 0;
}
