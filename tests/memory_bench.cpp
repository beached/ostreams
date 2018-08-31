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

#include <daw/daw_utility.h>

#include "./benchmark.h"
#include "daw/io/console_stream.h"
#include "daw/io/memory_stream.h"

struct stringstream_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
			std::stringstream ss{};
			ss << "The answer to the meaining of life is " << number << ' '
			   << ( static_cast<float>( number ) * 1.5f ) << '\n';
			daw::force_evaluation( ss );
		}
	}
};

struct snprintf_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
			daw::static_string_t<char, 50> buffer{};
			buffer.resize( buffer.capacity( ), false );

			snprintf( buffer.data( ), buffer.capacity( ),
			          "The asnwer to the meaning of life is %d %f\n", number,
			          static_cast<float>( number ) * 1.5f );
			daw::force_evaluation( buffer );
		}
	}
};

struct memory_stream_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
			daw::static_string_t<char, 50> buffer{};

			// Indicate we want to have the full size of static_string as in use, and
			// do not default init the char's to 0
			buffer.resize( buffer.capacity( ), false );

			auto ss =
			  ::daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );

			ss << "The answer to the meaining of life is " << number << ' '
			   << ( static_cast<float>( number ) * 1.5f ) << '\n';
			daw::force_evaluation( buffer );
		}
	}
};

struct cerr_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
			std::cerr << "The answer to the meaining of life is " << number << ' '
			          << ( static_cast<float>( number ) * 1.5f ) << '\n';
		}
	}
};

struct console_stream_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
			daw::con_err << "The answer to the meaining of life is " << ' '
			             << ( static_cast<float>( number ) * 1.5f ) << number << '\n';
		}
	}
};

struct printf_test {
	void operator( )( size_t count, int number ) const {
		for( size_t n = 0; n < count; ++n ) {
			daw::force_evaluation( number );
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
