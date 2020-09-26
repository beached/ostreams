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

template<typename>
struct get_format;

template<>
struct get_format<float> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%.*f"};
	}
};

template<>
struct get_format<double> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%.*f"};
	}
};

template<>
struct get_format<int8_t> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%hhi"};
	}
};

template<>
struct get_format<int16_t> {
	static constexpr daw::static_string_t<char, 3> get( ) {
		return daw::static_string_t<char, 3>{"%hi"};
	}
};

template<>
struct get_format<int32_t> {
	static constexpr daw::static_string_t<char, 2> get( ) {
		return daw::static_string_t<char, 2>{"%i"};
	}
};

template<>
struct get_format<long> {
	static constexpr daw::static_string_t<char, 3> get( ) {
		return daw::static_string_t<char, 3>{"%ld"};
	}
};

template<>
struct get_format<int64_t> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%lli"};
	}
};

template<>
struct get_format<uint8_t> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%hhu"};
	}
};

template<>
struct get_format<uint16_t> {
	static constexpr daw::static_string_t<char, 3> get( ) {
		return daw::static_string_t<char, 3>{"%hu"};
	}
};

template<>
struct get_format<uint32_t> {
	static constexpr daw::static_string_t<char, 2> get( ) {
		return daw::static_string_t<char, 2>{"%u"};
	}
};

template<>
struct get_format<unsigned long> {
	static constexpr daw::static_string_t<char, 3> get( ) {
		return daw::static_string_t<char, 3>{"%lu"};
	}
};

template<>
struct get_format<uint64_t> {
	static constexpr daw::static_string_t<char, 4> get( ) {
		return daw::static_string_t<char, 4>{"%llu"};
	}
};

struct test_t {
	template<typename Number>
	void stringstream_test( size_t count, Number number ) const {
		daw::DoNotOptimize( count );
		std::stringstream ss{};
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			ss << std::setprecision( std::numeric_limits<Number>::max_digits10 )
			   << number;
			daw::DoNotOptimize( ss );
		}
	}

	template<typename Number, std::enable_if_t<std::is_integral_v<Number>,
	                                           std::nullptr_t> = nullptr>
	void snprintf_test( size_t count, Number number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );

			snprintf( buffer.data( ), buffer.capacity( ),
			          get_format<Number>::get( ).buffer, number );
			daw::DoNotOptimize( buffer );
		}
	}

	template<typename Number, std::enable_if_t<std::is_floating_point_v<Number>,
	                                           std::nullptr_t> = nullptr>
	void snprintf_test( size_t count, Number number ) const {
		daw::static_string_t<char, 325> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			snprintf( buffer.data( ), buffer.capacity( ),
			          get_format<Number>::get( ).buffer,
			          std::numeric_limits<Number>::max_digits10,
			          static_cast<double>( number ) );

			buffer.shrink_to_fit( );
			daw::DoNotOptimize( buffer );
		}
	}

	template<typename Number>
	void memory_stream_test( size_t count, Number number ) const {
		daw::static_string_t<char, 425> buffer{};
		buffer.resize( buffer.capacity( ), false );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );

			auto ss =
			  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

			ss << number;

			daw::DoNotOptimize( buffer );
		}
	}

	template<typename Number>
	void cerr_test( size_t count, Number number ) const {
		daw::DoNotOptimize( count );
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			std::cerr << std::setprecision(
			               std::numeric_limits<Number>::max_digits10 )
			          << number;
		}
	}

	template<typename Number>
	void console_stream_test( size_t count, Number number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			daw::con_err << number;
		}
	}

	template<typename Number, std::enable_if_t<std::is_integral_v<Number>,
	                                           std::nullptr_t> = nullptr>
	void printf_test( size_t count, Number number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			fprintf( stderr, get_format<Number>::get( ).buffer, number );
		}
	}

	template<typename Number, std::enable_if_t<std::is_floating_point_v<Number>,
	                                           std::nullptr_t> = nullptr>
	void printf_test( size_t count, Number number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::DoNotOptimize( number );
			fprintf( stderr, get_format<Number>::get( ).buffer,
			         std::numeric_limits<Number>::max_digits10,
			         static_cast<double>( number ) );
		}
	}
};

int main( int argc, char ** ) {
	std::ios_base::sync_with_stdio( false );
	// constexpr size_t const count = 100'000;
	size_t count = 100'000;
	daw::do_bench_header( );

	daw::do_bench( "double", count, 0.1234, test_t{} );
	daw::do_bench( "double", count, 2.718281828459045, test_t{} );
	daw::do_bench( "double", count, 1.7976931348623157e308, test_t{} );
	daw::do_bench( "double", count, 123456.0435333, test_t{} );
	{
		double d = std::numeric_limits<double>::max( );
		daw::DoNotOptimize( d );
		for( size_t n = 0; n < 1'000'000; ++n ) {
			d -= 0.00001;
			daw::DoNotOptimize( d );
		}
		daw::do_bench( "double", count, d, test_t{} );
	}
	daw::do_bench( "double", count, 0.1, test_t{} );
	daw::do_bench( "double", count, 0.12, test_t{} );
	daw::do_bench( "double", count, 0.123, test_t{} );
	daw::do_bench( "double", count, 0.1234, test_t{} );
	daw::do_bench( "double", count, 1.2345, test_t{} );
	daw::do_bench( "double", count, 1.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 2.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 10.0 / 3.0, test_t{} );
	daw::do_bench( "double", count, 20.0 / 3.0, test_t{} );

	daw::do_bench( "double", 100 /* count*/, std::numeric_limits<double>::min( ),
	               test_t{} );
	daw::do_bench( "double", count, std::numeric_limits<double>::max( ),
	               test_t{} );
	daw::do_bench( "double", count, std::numeric_limits<double>::denorm_min( ),
	               test_t{} );
	daw::do_bench( "double", count,
	               std::numeric_limits<double>::min( ) *
	                 std::numeric_limits<double>::max( ),
	               test_t{} );
	daw::do_bench( "double", count,
	               std::numeric_limits<double>::max( ) -
	                 ( std::numeric_limits<double>::max( ) / 2.0 ),
	               test_t{} );

	daw::do_bench( "float", count, 0.1234f, test_t{} );
	daw::do_bench( "float", count, 2.718281828459045f, test_t{} );
	daw::do_bench( "float", count, 1.7976931348623157e38f, test_t{} );
	daw::do_bench( "float", count, 123456.0435333f, test_t{} );
	{
		float d = std::numeric_limits<float>::max( );
		daw::DoNotOptimize( d );
		for( size_t n = 0; n < 1'000'000; ++n ) {
			d -= 0.00001f;
			daw::DoNotOptimize( d );
		}
		daw::do_bench( "float", count, d, test_t{} );
	}
	daw::do_bench( "float", count, 0.1f, test_t{} );
	daw::do_bench( "float", count, 0.12f, test_t{} );
	daw::do_bench( "float", count, 0.123f, test_t{} );
	daw::do_bench( "float", count, 0.1234f, test_t{} );
	daw::do_bench( "float", count, 1.2345f, test_t{} );
	daw::do_bench( "float", count, 1.0f / 3.0f, test_t{} );
	daw::do_bench( "float", count, 2.0f / 3.0f, test_t{} );
	daw::do_bench( "float", count, 10.0f / 3.0f, test_t{} );
	daw::do_bench( "float", count, 20.0f / 3.0f, test_t{} );
	daw::do_bench( "float", count, std::numeric_limits<float>::min( ), test_t{} );
	daw::do_bench( "float", count, std::numeric_limits<float>::max( ), test_t{} );
	daw::do_bench( "float", count, std::numeric_limits<float>::denorm_min( ),
	               test_t{} );
	daw::do_bench( "float", count,
	               std::numeric_limits<float>::min( ) *
	                 std::numeric_limits<float>::max( ),
	               test_t{} );
	daw::do_bench( "float", count,
	               std::numeric_limits<float>::max( ) -
	                 ( std::numeric_limits<float>::max( ) / 2.0f ),
	               test_t{} );

	daw::do_bench( "uint64_t", count, std::numeric_limits<uint64_t>::min( ),
	               test_t{} );
	daw::do_bench( "uint64_t", count, std::numeric_limits<uint64_t>::max( ),
	               test_t{} );
	daw::do_bench( "uint64_t", count, 0ULL, test_t{} );

	daw::do_bench( "int64_t", count, std::numeric_limits<int64_t>::min( ),
	               test_t{} );
	daw::do_bench( "int64_t", count, std::numeric_limits<int64_t>::max( ),
	               test_t{} );
	daw::do_bench( "int64_t", count, 0LL, test_t{} );

	daw::do_bench( "uint32_t", count, std::numeric_limits<uint32_t>::min( ),
	               test_t{} );
	daw::do_bench( "uint32_t", count, std::numeric_limits<uint32_t>::max( ),
	               test_t{} );
	daw::do_bench( "uint32_t", count, 0UL, test_t{} );

	daw::do_bench( "int32_t", count, std::numeric_limits<int32_t>::min( ),
	               test_t{} );
	daw::do_bench( "int32_t", count, std::numeric_limits<int32_t>::max( ),
	               test_t{} );
	daw::do_bench( "int32_t", count, 0L, test_t{} );

	daw::do_bench( "uint16_t", count, std::numeric_limits<uint16_t>::min( ),
	               test_t{} );
	daw::do_bench( "uint16_t", count, std::numeric_limits<uint16_t>::max( ),
	               test_t{} );
	daw::do_bench( "uint16_t", count, static_cast<uint16_t>( 0 ), test_t{} );

	daw::do_bench( "int16_t", count, std::numeric_limits<int16_t>::min( ),
	               test_t{} );
	daw::do_bench( "int16_t", count, std::numeric_limits<int16_t>::max( ),
	               test_t{} );
	daw::do_bench( "int16_t", count, static_cast<int16_t>( 0 ), test_t{} );

	daw::do_bench( "uint8_t", count, std::numeric_limits<uint8_t>::min( ),
	               test_t{} );
	daw::do_bench( "uint8_t", count, std::numeric_limits<uint8_t>::max( ),
	               test_t{} );
	daw::do_bench( "uint8_t", count, static_cast<uint8_t>( 0 ), test_t{} );

	daw::do_bench( "int8_t", count, std::numeric_limits<int8_t>::min( ),
	               test_t{} );
	daw::do_bench( "int8_t", count, std::numeric_limits<int8_t>::max( ),
	               test_t{} );
	daw::do_bench( "int8_t", count, static_cast<int8_t>( 0 ), test_t{} );

	return 0;
}
