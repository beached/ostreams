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
#include <iomanip>
#include <iostream>
#include <limits>

template<size_t N>
void verify_value( float f, double d, char const ( &str )[N] ) {
	daw::con_out << "number: " << str << '\n' << "con_out f: " << f << '\n';
	std::cout << "cout    f: "
	          << std::setprecision( std::numeric_limits<float>::max_digits10 )
	          << f << '\n';

	daw::con_out << "con_out d: " << d << '\n';

	std::cout << "cout    d: "
	          << std::setprecision( std::numeric_limits<double>::max_digits10 )
	          << d << '\n'
	          << '\n';
}

template<size_t N, size_t N2, typename Number>
void verify_value( char const ( &type_name )[N], char const ( &value )[N2],
                   Number n ) {
	daw::con_out << "number: " << value << " " << type_name << '\n';
	std::cout
	  << "cout   -> "
	  << static_cast<std::conditional_t<sizeof( Number ) == 1, int, Number>>( n )
	  << '\n';
	daw::con_out << "con_out-> " << n << '\n';
}

int main( int argc, char ** ) {
	daw::string_view str = "Console test\n";
	daw::con_out << str;
	float const f = static_cast<float>( argc ) * 1.2334f;
	daw::con_out << "The number is: " << f << ". " << argc << " times number is "
	             << ( static_cast<float>( argc ) * f ) << '\n';

	double const d = static_cast<double>( argc ) * 1.2334;
	daw::con_out << "The number is: " << d << ". " << argc << " times number is "
	             << ( static_cast<double>( argc ) * d ) << '\n';
	verify_value( 123456.0435333f, 123456.0435333, "123456.0435333" );
	verify_value( 0.1f, 0.1, "0.1" );
	verify_value( 0.12f, 0.12, "0.12" );
	verify_value( 0.123f, 0.123, "0.123" );
	verify_value( 0.1234f, 0.1234, "0.1234" );
	verify_value( 1.2345f, 1.2345, "1.2345" );
	verify_value( 1.0f / 3.0f, 1.0 / 3.0, "1.0/3.0" );
	verify_value( 2.0f / 3.0f, 2.0 / 3.0, "2.0/3.0" );
	verify_value( 10.0f / 3.0f, 10.0 / 3.0, "10.0/3.0" );
	verify_value( 20.0f / 3.0f, 20.0 / 3.0, "20.0/3.0" );
	verify_value( std::numeric_limits<float>::min( ),
	              std::numeric_limits<double>::min( ), "min( )" );
	verify_value( std::numeric_limits<float>::min( ) * 2.0f,
	              std::numeric_limits<double>::min( ) * 2.0, "min( )*2" );
	verify_value( std::numeric_limits<float>::min( ) +
	                ( std::numeric_limits<float>::min( ) * 10.0f ),
	              std::numeric_limits<double>::min( ) +
	                ( std::numeric_limits<double>::min( ) * 10.0 ),
	              "min( )+(min( )*10)" );
	verify_value( std::numeric_limits<float>::max( ),
	              std::numeric_limits<double>::max( ), "max( )" );
	verify_value( std::numeric_limits<float>::denorm_min( ),
	              std::numeric_limits<double>::denorm_min( ), "denorm_min( )" );

	verify_value(
	  std::numeric_limits<float>::min( ) * std::numeric_limits<float>::max( ),
	  std::numeric_limits<double>::min( ) * std::numeric_limits<double>::max( ),
	  "min( )*max( )" );
	verify_value( std::numeric_limits<float>::max( ) -
	                ( std::numeric_limits<float>::max( ) / 2.0f ),
	              std::numeric_limits<double>::max( ) -
	                ( std::numeric_limits<double>::max( ) / 2.0 ),
	              "max( )-(max( )/2.0)" );
	daw::con_out << '\n';
	verify_value( "uint64_t", "min", std::numeric_limits<uint64_t>::min( ) );
	verify_value( "uint64_t", "max", std::numeric_limits<uint64_t>::max( ) );
	verify_value( "uint64_t", "0", 0ULL );

	verify_value( "int64_t", "min", std::numeric_limits<int64_t>::min( ) );
	verify_value( "int64_t", "max", std::numeric_limits<int64_t>::max( ) );
	verify_value( "int64_t", "0", 0LL );

	verify_value( "uint32_t", "min", std::numeric_limits<uint32_t>::min( ) );
	verify_value( "uint32_t", "max", std::numeric_limits<uint32_t>::max( ) );
	verify_value( "uint32_t", "0", 0UL );

	verify_value( "int32_t", "min", std::numeric_limits<int32_t>::min( ) );
	verify_value( "int32_t", "max", std::numeric_limits<int32_t>::max( ) );
	verify_value( "int32_t", "0", 0L );

	verify_value( "uint16_t", "min", std::numeric_limits<uint16_t>::min( ) );
	verify_value( "uint16_t", "max", std::numeric_limits<uint16_t>::max( ) );
	verify_value( "uint16_t", "0", static_cast<uint16_t>( 0 ) );

	verify_value( "int16_t", "min", std::numeric_limits<int16_t>::min( ) );
	verify_value( "int16_t", "max", std::numeric_limits<int16_t>::max( ) );
	verify_value( "int16_t", "0", static_cast<int16_t>( 0 ) );

	verify_value( "uint8_t", "min",
	              daw::as_int( std::numeric_limits<uint8_t>::min( ) ) );
	verify_value( "uint8_t", "max",
	              daw::as_int( std::numeric_limits<uint8_t>::max( ) ) );
	verify_value( "uint8_t", "0", daw::as_int( static_cast<uint8_t>( 0 ) ) );

	verify_value( "int8_t", "min",
	              daw::as_int( std::numeric_limits<int8_t>::min( ) ) );
	verify_value( "int8_t", "max",
	              daw::as_int( std::numeric_limits<int8_t>::max( ) ) );
	verify_value( "int8_t", "0", daw::as_int( static_cast<int8_t>( 0 ) ) );

	verify_value( "uint8_t", "min", std::numeric_limits<uint8_t>::min( ) );
	verify_value( "uint8_t", "max", std::numeric_limits<uint8_t>::max( ) );
	verify_value( "uint8_t", "0", static_cast<uint8_t>( 0 ) );
	verify_value( "uint8_t", "97", static_cast<uint8_t>( 97 ) );

	verify_value( "int8_t", "min", std::numeric_limits<int8_t>::min( ) );
	verify_value( "int8_t", "max", std::numeric_limits<int8_t>::max( ) );
	verify_value( "int8_t", "0", static_cast<int8_t>( 0 ) );
	verify_value( "int8_t", "97", static_cast<int8_t>( 97 ) );

	return 0;
}
