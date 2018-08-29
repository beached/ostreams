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

#include "ostreams.h"

template<typename CharT>
constexpr size_t fill_buffer( CharT *buffer, size_t capacity, int n ) {
	auto buff_os = ::daw::make_memory_buffer_stream( buffer, capacity );

	buff_os << "Hello the number is: " << ' ' << -234432 << ' ' << true
	        << " string literal " << n;

	return buff_os.get_underlying_stream( ).size( );
}

/*
template<typename Float>
struct str_t {
	static constexpr size_t buff_sz = std::numeric_limits<Float>::max_exponent10+2;

	daw::io::ostream_converters::impl::sv_buff<char, buff_sz> buffer;

	explicit constexpr str_t( Float f )
	  : buffer( daw::io::ostream_converters::to_string<char>( f ) ) {}
};
*/

template<typename Float, size_t buff_sz=500>
struct str_t {
	char buffer[buff_sz + 1] = {0};

	explicit constexpr str_t( Float f ) {
		auto buff_os = ::daw::make_memory_buffer_stream( buffer, buff_sz );
		buff_os << "The number is: " << f << ' ' << 5 << " times\n";
	}
};

template<typename Float>
str_t( Float value )->str_t<Float>;

auto test( ) {
	constexpr auto result = str_t( 1234560.435333 );
	// constexpr auto result = str_t( 0x1p87f );
	return result;
}

int main( int argc, char ** ) {
	std::cout << static_cast<std::string_view>( test( ).buffer ) << '\n';
	float const f = static_cast<float>( argc ) * 1.2334f;
	std::cout << "processing: float " << f << '\n';
	std::cout << static_cast<std::string_view>( str_t( f ).buffer ) << '\n';
	double const d = static_cast<double>( argc ) * 1.2334;
	std::cout << "processing: double " << d << '\n';
	std::cout << static_cast<std::string_view>( str_t( d ).buffer ) << '\n';
	std::cout << '\n';

	return 0;
}

