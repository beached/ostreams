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
#include <daw/daw_benchmark.h>

#include "./benchmark.h"
#include "daw/io/console_stream.h"
#include "daw/io/memory_stream.h"

struct test_t {
	void stringstream_test( size_t count, double number ) const {
		daw::DoNotOptimize( count );
		std::stringstream ss{};
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			ss << "The answer to the meaning of life is " << count << ' '
			   << std::setprecision( std::numeric_limits<double>::max_digits10 )
			   << number << '\n';
			daw::DoNotOptimize( ss );
		}
	}

	void snprintf_test( size_t count, double number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );

			snprintf( buffer.data( ), buffer.capacity( ),
			          "The asnwer to the meaning of life is %lu %.*f\n", count,
			          std::numeric_limits<double>::max_digits10, number );
			daw::DoNotOptimize( buffer );
		}
	}

	void memory_stream_test( size_t count, double number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );

			auto ss =
			  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

			ss << "The answer to the meaning of life is " << count << ' ' << number
			   << '\n';

			daw::DoNotOptimize( buffer );
		}
	}

	void cerr_test( size_t count, double number ) const {
		daw::DoNotOptimize( count );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			std::cerr << "The answer to the meaning of life is " << count << ' '
			          << std::setprecision(
			               std::numeric_limits<double>::max_digits10 )
			          << number << '\n';
		}
	}

	void printf_test( size_t count, double number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			fprintf( stderr, "The asnwer to the meaning of life is %lu %.*f\n", count,
			         std::numeric_limits<double>::max_digits10, number );
		}
	}

	template<typename Number>
	void console_stream_test( size_t count, Number number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			daw::con_err << "The answer to the meaning of life is " << count << ' '
			             << number << '\n';
		}
	}
};

int main( int argc, char ** ) {
	std::ios_base::sync_with_stdio( false );
	size_t count = 100'000;
	daw::do_bench_header( );
	daw::do_bench( "double", count, 42.0, test_t{} );
	/*
	daw::do_bench( "double", count, 0.1234, test_t{} );
	daw::do_bench( "double", count, 2.718281828459045, test_t{} );
	daw::do_bench( "double", count, 1.7976931348623157e308, test_t{} );
	daw::do_bench( "double", count, 123456.0435333, test_t{} );
	daw::do_bench( "double", count, 0.1, test_t{} );
	daw::do_bench( "double", count, 0.12, test_t{} );
	daw::do_bench( "double", count, 0.123, test_t{} );
	daw::do_bench( "double", count, 0.1234, test_t{} );
	daw::do_bench( "double", count, 1.2345, test_t{} );
	daw::do_bench( "double", count, 1.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 2.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 10.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 20.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, std::numeric_limits<double>::min( ), test_t{}
	); daw::do_bench( "double", count, std::numeric_limits<double>::max( ),
	test_t{} ); daw::do_bench( "double", count,
	std::numeric_limits<double>::denorm_min( ), test_t{} ); daw::do_bench(
	"double", count, std::numeric_limits<double>::min( ) *
	            std::numeric_limits<double>::max( ), test_t{} );
	daw::do_bench( "double", count,
	          std::numeric_limits<double>::max( ) -
	            ( std::numeric_limits<double>::max( ) / 2.0 ), test_t{} );
*/
	return 0;
}
