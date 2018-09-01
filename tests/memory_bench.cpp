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
	void operator( )( size_t count, int number ) const {
		std::stringstream ss{};
		for( size_t n = 0; n < count; ++n ) {
			ss.clear( );
			do_not_optimize( number );
			ss << "The answer to the meaining of life is " << number << ' '
			   << ( static_cast<float>( number ) * 1.5f ) << '\n';
			do_not_optimize( ss );
		}
	}
};

struct snprintf_test {
	void operator( )( size_t count, int number ) const {
		daw::static_string_t<char, 50> buffer{};
		for( size_t n = 0; n < count; ++n ) {
			buffer.resize( buffer.capacity( ), false );
			do_not_optimize( number );
			snprintf( buffer.data( ), buffer.capacity( ),
			          "The asnwer to the meaning of life is %d %f\n", number,
			          static_cast<float>( number ) * 1.5f );
			buffer.shrink_to_fit( );
			do_not_optimize( buffer );
		}
	}
};

struct memory_stream_test {
	void operator( )( size_t count, int number ) const {
		daw::static_string_t<char, 50> buffer{};
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );

			buffer.resize( buffer.capacity( ), false );

			auto ss =
			  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

			ss << "The answer to the meaining of life is " << number << ' '
			   << ( static_cast<float>( number ) * 1.5f ) << '\n';
			buffer.shrink_to_fit( );
			do_not_optimize( buffer );
		}
	}
};

struct cerr_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			std::cerr << "The answer to the meaining of life is " << number << ' '
			          << ( static_cast<float>( number ) * 1.5f ) << '\n';
		}
	}
};

struct console_stream_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			daw::con_err << "The answer to the meaining of life is " << ' '
			             << ( static_cast<float>( number ) * 1.5f ) << number << '\n';
		}
	}
};

struct printf_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			do_not_optimize( number );
			fprintf( stderr, "The asnwer to the meaning of life is %d %f\n", number,
			         static_cast<float>( number ) * 1.5f );
		}
	}
};

int main( int argc, char ** ) {
	std::ios_base::sync_with_stdio( false );
	constexpr size_t const count = 100'000;
	std::cout << "char buffer" << std::endl;
	daw::benchmark( "std::string_stream", stringstream_test{}, count, count,
	                argc );
	daw::benchmark( "snprintf", snprintf_test{}, count, count, argc );
	daw::benchmark( "daw::memory_stream", memory_stream_test{}, count, count,
	                argc );

	std::cout << "\nconsole" << std::endl;
	daw::benchmark( "srd::cerr", cerr_test{}, count, count, argc );
	std::cerr << std::flush;
	daw::benchmark( "printf", printf_test{}, count, count, argc );
	fflush( stderr );
	daw::benchmark( "daw::console_stream", console_stream_test{}, count, count,
	                argc );
	fflush( stderr );
	return 0;
}
