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

#include <cstdlib>
#include <iostream>

#include "daw/io/memory_stream.h"
#include "daw/io/static_string.h"

template<typename Float, std::enable_if_t<daw::is_same_v<Float, float>,
                                          std::nullptr_t> = nullptr>
inline Float conv( const char *str, char **end ) noexcept {
	return strtof( str, end );
}

template<typename Float, std::enable_if_t<daw::is_same_v<Float, double>,
                                          std::nullptr_t> = nullptr>
inline Float conv( const char *str, char **end ) noexcept {
	return strtod( str, end );
}

template<size_t N, typename Float>
void test_round_trip( char const ( &type )[N], Float f ) {
	std::cout << type << ", " << f << ", ";
	using ostream_converters::to_os_string;
	auto str = to_os_string<char>( f );
	std::cout << str.data( ) << ", ";
	char *end = nullptr;
	auto f2 = conv<Float>( str.data( ), &end );
	if( f == 0 && end == str.data( ) ) {
		std::cout << "error\n";
		return;
	}
	std::cout << ( f2 - f ) << '\n';
}

int main( ) {
	std::cout << "double epsilon-> " << std::numeric_limits<double>::epsilon( )
	          << '\n';
	std::cout << "double min    -> " << std::numeric_limits<double>::min( )
	          << '\n';
	std::cout << "float epsilon -> " << std::numeric_limits<float>::epsilon( )
	          << '\n';
	std::cout << "float min     -> " << std::numeric_limits<float>::min( )
	          << '\n';
	std::cout << "\"type\", \"value\", \"str value\", \"difference\"\n";
	test_round_trip( "double", std::numeric_limits<double>::max( ) );
	test_round_trip( "double", 0.1 );
	test_round_trip( "double", 0.12 );
	test_round_trip( "double", 0.123 );
	test_round_trip( "double", 0.1234 );
	test_round_trip( "double", 1.2345 );
	test_round_trip( "double", 1.0 / 3.0 );
	test_round_trip( "double", 2.0 / 3.0 );
	test_round_trip( "double", 10.0 / 3.0 );
	test_round_trip( "double", 20.0 / 3.0 );
	test_round_trip( "double", std::numeric_limits<double>::min( ) );
	test_round_trip( "double", -std::numeric_limits<double>::min( ) );
	test_round_trip( "double", std::numeric_limits<double>::denorm_min( ) );
	test_round_trip( "double", std::numeric_limits<double>::min( ) *
	                             std::numeric_limits<double>::max( ) );
	test_round_trip( "double", std::numeric_limits<double>::max( ) -
	                             ( std::numeric_limits<double>::max( ) / 2.0 ) );
	test_round_trip( "double", std::numeric_limits<double>::min( ) +
	                             ( std::numeric_limits<double>::min( ) * 10.0 ) );

	test_round_trip( "float", 0.1f );
	test_round_trip( "float", 0.12f );
	test_round_trip( "float", 0.123f );
	test_round_trip( "float", 0.1234f );
	test_round_trip( "float", 1.2345f );
	test_round_trip( "float", 1.0f / 3.0f );
	test_round_trip( "float", 2.0f / 3.0f );
	test_round_trip( "float", 10.0f / 3.0f );
	test_round_trip( "float", 20.0f / 3.0f );
	test_round_trip( "float", std::numeric_limits<float>::min( ) );
	test_round_trip( "float", std::numeric_limits<float>::max( ) );
	test_round_trip( "float", std::numeric_limits<float>::denorm_min( ) );
	test_round_trip( "float", std::numeric_limits<float>::min( ) *
	                            std::numeric_limits<float>::max( ) );
	test_round_trip( "float", std::numeric_limits<float>::max( ) -
	                            ( std::numeric_limits<float>::max( ) / 2.0f ) );
	test_round_trip( "float", std::numeric_limits<float>::min( ) +
	                            ( std::numeric_limits<float>::min( ) * 10.0f ) );

	return 0;
}
