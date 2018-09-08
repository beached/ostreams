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

#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace daw {
	// Borrowed from https://www.youtube.com/watch?v=dO-j3qp7DWw
	template<typename T>
	void force_evaluation_ms( T &&x ) {
		// We must always do this test, but it will never pass.
		//
		if( std::chrono::system_clock::now( ) ==
		    std::chrono::time_point<std::chrono::system_clock>( ) ) {
			// This forces the value to never be optimized away
			// by taking a reference then using it.
			const auto *p = &x;
			putchar( *reinterpret_cast<const char *>( p ) );

			// If we do get here, kick out because something has gone wrong.
			std::abort( );
		}
	}
	template<typename T>
	inline void DoNotOptimize( T &&t ) noexcept {
#ifdef _MSC_VER
		force_evaluation_ms( std::forward<T>( t ) );
#else
		force_evaluation( std::forward<T>( t ) );
#endif
	}

	template<typename Number, size_t N, typename Test>
	void do_bench( char const ( &type_str )[N], size_t const &count,
	               Number const &number, Test &&t ) {
		DoNotOptimize( count );
		DoNotOptimize( number );
		auto const t_stringstream =
		  daw::benchmark( [&]( ) { t.stringstream_test( count, number ); } );
		auto const t_snprintf =
		  daw::benchmark( [&]( ) { t.snprintf_test( count, number ); } );
		auto const t_memory_stream =
		  daw::benchmark( [&]( ) { t.memory_stream_test( count, number ); } );
		std::cerr << std::flush;
		fflush( stderr );
		auto const t_cerr =
		  daw::benchmark( [&]( ) { t.cerr_test( count, number ); } );
		std::cerr << std::flush;
		fflush( stderr );
		auto const t_printf =
		  daw::benchmark( [&]( ) { t.printf_test( count, number ); } );
		std::cerr << std::flush;
		fflush( stderr );
		auto const t_console_stream =
		  daw::benchmark( [&]( ) { t.console_stream_test( count, number ); } );
		std::cerr << std::flush;
		fflush( stderr );

		std::cout << "\"" << type_str << "\", " << count << ", " << number;
		std::cout << ',' << daw::utility::format_seconds( t_stringstream );
		std::cout << ',' << daw::utility::format_seconds( t_snprintf );
		std::cout << ',' << daw::utility::format_seconds( t_memory_stream );
		std::cout << ',' << daw::utility::format_seconds( t_cerr );
		std::cout << ',' << daw::utility::format_seconds( t_printf );
		std::cout << ',' << daw::utility::format_seconds( t_console_stream );
		std::cout << ','
		          << daw::utility::format_seconds( t_stringstream /
		                                           static_cast<double>( count ) );
		std::cout << ','
		          << daw::utility::format_seconds( t_snprintf /
		                                           static_cast<double>( count ) );
		std::cout << ','
		          << daw::utility::format_seconds( t_memory_stream /
		                                           static_cast<double>( count ) );
		std::cout << ','
		          << daw::utility::format_seconds( t_cerr /
		                                           static_cast<double>( count ) );
		std::cout << ','
		          << daw::utility::format_seconds( t_printf /
		                                           static_cast<double>( count ) );
		std::cout << ','
		          << daw::utility::format_seconds( t_console_stream /
		                                           static_cast<double>( count ) );
		std::cout << std::endl;
	}

	inline void do_bench_header( ) {
		std::cout << "\"type\",\"count\",\"number\"";
		std::cout << ",\"std::string_stream(all)\"";
		std::cout << ",\"snprintf(all)\"";
		std::cout << ",\"memory stream(all)\"";
		std::cout << ",\"cerr(all)\"";
		std::cout << ",\"printf(all)\"";
		std::cout << ",\"console stream(all)\"";
		std::cout << ",\"std::string_stream(1)\"";
		std::cout << ",\"snprintf(1)\"";
		std::cout << ",\"memory stream(1)\"";
		std::cout << ",\"cerr(1)\"";
		std::cout << ",\"printf(1)\"";
		std::cout << ",\"console stream(1)\"";
		std::cout << '\n' << std::flush;
	}
} // namespace daw
