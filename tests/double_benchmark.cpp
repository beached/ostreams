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

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifndef _MSC_VER
#include <daw/daw_utility.h>
#endif

#include "./benchmark.h"
#include "daw/io/console_stream.h"
#include "daw/io/memory_stream.h"

template<typename T>
inline void do_not_optimize( T &&t ) noexcept {
#ifdef _MSC_VER
	daw::force_evaluation_ms( std::forward<T>( t ) );
#else
	daw::force_evaluation( std::forward<T>( t ) );
#endif
}

struct stringstream_test {
	void operator( )( size_t count, double number ) const {
		do_not_optimize( count );
		std::stringstream ss{};
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			ss << "The answer to the meaning of life is " << count << ' '
			   << std::setprecision( std::numeric_limits<double>::max_digits10 )
			   << number << '\n';
			do_not_optimize( ss );
		}
	}
};

struct snprintf_test {
	void operator( )( size_t count, double number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );

			snprintf( buffer.data( ), buffer.capacity( ),
			          "The asnwer to the meaning of life is %lu %.*f\n", count,
			          std::numeric_limits<double>::max_digits10, number );
			do_not_optimize( buffer );
		}
	}
};

struct memory_stream_test {
	void operator( )( size_t count, double number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );

			auto ss =
			  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

			ss << "The answer to the meaning of life is " << count << ' ' << number
			   << '\n';

			do_not_optimize( buffer );
		}
	}
};

struct cerr_test {
	void operator( )( size_t count, double number ) const {
		do_not_optimize( count );
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			std::cerr << "The answer to the meaning of life is " << count << ' '
			          << std::setprecision(
			               std::numeric_limits<double>::max_digits10 )
			          << number << '\n';
		}
	}
};

struct printf_test {
	void operator( )( size_t count, double number ) const {
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			fprintf( stderr, "The asnwer to the meaning of life is %lu %.*f\n", count,
			         std::numeric_limits<double>::max_digits10, number );
		}
	}
};

struct console_stream_test {
	template<typename Number>
	void operator( )( size_t count, Number number ) const {
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			daw::con_err << "The answer to the meaning of life is " << count << ' '
			             << number << '\n';
		}
	}
};

template<size_t N>
void do_bench( char const ( &str )[N], size_t count, double number ) {
	std::cout << "\nnumber: " << number << ' ' << str << std::endl;
	do_not_optimize( number );
	std::cout << "char buffer" << std::endl;
	daw::benchmark( "std::string_stream", stringstream_test{}, count, count,
	                number );
	daw::benchmark( "snprintf", snprintf_test{}, count, count, number );
	daw::benchmark( "daw::memory_stream", memory_stream_test{}, count, count,
	                number );

	std::cout << "\nconsole" << std::endl;
	daw::benchmark( "srd::cerr", cerr_test{}, count, count, number );
	std::cerr << std::flush;
	daw::benchmark( "printf", printf_test{}, count, count, number );
	fflush( stderr );
	daw::benchmark( "daw::console_stream", console_stream_test{}, count, count,
	                number );
	fflush( stderr );
}

int main( int argc, char ** ) {
	std::ios_base::sync_with_stdio( false );
	size_t count = 100'000;
	do_bench( "double", count, 42.0 );
	/*
	do_bench( "double", count, 0.1234 );
	do_bench( "double", count, 2.718281828459045 );
	do_bench( "double", count, 1.7976931348623157e308 );
	do_bench( "double", count, 123456.0435333 );
	do_bench( "double", count, 0.1 );
	do_bench( "double", count, 0.12 );
	do_bench( "double", count, 0.123 );
	do_bench( "double", count, 0.1234 );
	do_bench( "double", count, 1.2345 );
	do_bench( "double", count, 1.0 / 3.0 );
	do_bench( "double", count, 2.0 / 3.0 );
	do_bench( "double", count, 10.0 / 3.0 );
	do_bench( "double", count, 20.0 / 3.0 );
	do_bench( "double", count, std::numeric_limits<double>::min( ) );
	do_bench( "double", count, std::numeric_limits<double>::max( ) );
	do_bench( "double", count, std::numeric_limits<double>::denorm_min( ) );
	do_bench( "double", count,
	          std::numeric_limits<double>::min( ) *
	            std::numeric_limits<double>::max( ) );
	do_bench( "double", count,
	          std::numeric_limits<double>::max( ) -
	            ( std::numeric_limits<double>::max( ) / 2.0 ) );
*/
	return 0;
}
