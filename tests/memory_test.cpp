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


#include <iostream>
#include <daw/daw_string_view.h>

#include "memory_stream.h"

template<typename Float, size_t buff_sz = 500>
struct str_t {
	char buffer[buff_sz + 1] = {0};

	explicit constexpr str_t( Float f ) {
		auto buff_os = ::daw::io::make_memory_buffer_stream( buffer, buff_sz );
		buff_os << "The number is: " << f << ". " << 5 << " times number is "
		        << ( static_cast<Float>( 5 ) * f ) << '\n';
	}
};

auto const & test( ) {
	static constexpr auto result = str_t<double>( 1234560.435333 );
	return result;
}


int main( int argc, char ** ) {
	std::cout << static_cast<daw::string_view>( test( ).buffer ) << '\n';
	return 0;
}
