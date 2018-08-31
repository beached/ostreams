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
	namespace utility {
		template<typename T>
		std::string format_seconds( T t, size_t prec = 0 ) {
			std::stringstream ss;
			ss << std::setprecision( static_cast<int>( prec ) ) << std::fixed;
			auto val = static_cast<double>( t ) * 1000000000000000.0;
			if( val < 1000 ) {
				ss << val << "fs";
				return ss.str( );
			}
			val /= 1000.0;
			if( val < 1000 ) {
				ss << val << "ps";
				return ss.str( );
			}
			val /= 1000.0;
			if( val < 1000 ) {
				ss << val << "ns";
				return ss.str( );
			}
			val /= 1000.0;
			if( val < 1000 ) {
				ss << val << "us";
				return ss.str( );
			}
			val /= 1000.0;
			if( val < 1000 ) {
				ss << val << "ms";
				return ss.str( );
			}
			val /= 1000.0;
			ss << val << "s";
			return ss.str( );
		}
	} // namespace utility

	template<typename Test, typename... Args>
	void benchmark( std::string title, Test test_callable, size_t item_count,
	                  Args &&... args ) noexcept {
		auto const start = std::chrono::high_resolution_clock::now( );
		
		test_callable( std::forward<Args>( args )... );

		auto const finish = std::chrono::high_resolution_clock::now( );
		std::chrono::duration<double> const duration = finish - start;
		std::cout << title << " took "
		          << utility::format_seconds( duration.count( ), 2 );
		if( item_count > 1 ) {
			std::cout << " to process " << item_count << " items at "
			          << utility::format_seconds( ( duration / item_count ).count( ),
			                                      2 )
			          << " per item\n";
		} else {
			std::cout << '\n';
		}
	}

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


} // namespace daw

