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

#include <cstdio>

#include "file_stream.h"

namespace daw {
#ifdef stdout
	extern io::file_stream<char> con_out;
	io::file_stream<char> con_out = make_file_stream<char>( stdout );

	extern io::file_stream<wchar_t> con_wout;
	io::file_stream<wchar_t> con_wout = make_file_stream<wchar_t>( stdout );
#endif
#ifdef stderr
	extern io::file_stream<char> con_err;
	io::file_stream<char> con_err = make_file_stream<char>( stderr );

	extern io::file_stream<wchar_t> con_werr;
	io::file_stream<wchar_t> con_werr = make_file_stream<wchar_t>( stderr );
#endif
} // namespace daw
