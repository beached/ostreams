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

#include "daw/io/file_stream.h"

int main( int argc, char **argv ) {
	if( argc < 2 ) {
		puts( "Must supply file to write to" );
		exit( EXIT_FAILURE );
	}
	{
		auto fs_out = daw::make_file_stream( argv[1] );
		if( !fs_out ) {
			std::perror( "File opening failed" );
			exit( EXIT_FAILURE );
		}

		float const f = static_cast<float>( argc ) * 1.2334f;
		fs_out << "The number is: " << f << ". " << argc << " times number is "
		       << ( static_cast<float>( argc ) * f ) << '\n';

		double const d = static_cast<double>( argc ) * 1.2334;
		fs_out << "The number is: " << d << ". " << argc << " times number is "
		       << ( static_cast<double>( argc ) * d ) << '\n';
		fs_out << '\n';
	}

	auto fs_out = daw::make_file_stream( stdout );
	fs_out << "Done\n";

	return EXIT_SUCCESS;
}
