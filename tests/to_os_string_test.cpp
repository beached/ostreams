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

#include "daw/io/ostream_converters.h"
#include "daw/io/static_string.h"

using ostream_converters::to_os_string;

// static_string comparison
// ==
static_assert( "10" == daw::static_string_t<char, 2>( "10" ), "" );
static_assert( daw::static_string_t<char, 2>( "20" ) == "20", "" );
static_assert( daw::static_string_t<char, 2>( "30" ) ==
                 daw::static_string_t<char, 2>( "30" ),
               "" );

static_assert( L"10" == daw::static_string_t<wchar_t, 2>( L"10" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"20" ) == L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"30" ) ==
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );

// !=
static_assert( "10" != daw::static_string_t<char, 2>( "20" ), "" );
static_assert( daw::static_string_t<char, 2>( "20" ) != "10", "" );
static_assert( daw::static_string_t<char, 2>( "20" ) !=
                 daw::static_string_t<char, 2>( "30" ),
               "" );
static_assert( daw::static_string_t<char, 2>( "20" ) != "afkdfjskdfjsdlkfj10",
               "" );

static_assert( L"10" != daw::static_string_t<wchar_t, 2>( L"20" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) != L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"20" ) !=
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) !=
                 L"afkdfjskdfjsdlkfj10",
               "" );
// <
static_assert( "10" < daw::static_string_t<char, 2>( "20" ), "" );
static_assert( daw::static_string_t<char, 2>( "20" ) < "30", "" );
static_assert( daw::static_string_t<char, 2>( "20" ) <
                 daw::static_string_t<char, 2>( "30" ),
               "" );
static_assert( daw::static_string_t<char, 2>( "20" ) < "afkdfjskdfjsdlkfj10",
               "" );

static_assert( L"10" < daw::static_string_t<wchar_t, 2>( L"20" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) < L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"20" ) <
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) <
                 L"afkdfjskdfjsdlkfj10",
               "" );
// >
static_assert( "50" > daw::static_string_t<char, 2>( "20" ), "" );
static_assert( daw::static_string_t<char, 2>( "20" ) > "10", "" );
static_assert( daw::static_string_t<char, 2>( "AA" ) >
                 daw::static_string_t<char, 2>( "30" ),
               "" );
static_assert( daw::static_string_t<char, 2>( "z0" ) > "afkdfjskdfjsdlkfj10",
               "" );

static_assert( L"R3" > daw::static_string_t<wchar_t, 2>( L"20" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"a0" ) > L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"a0" ) >
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"z0" ) >
                 L"afkdfjskdfjsdlkfj10",
               "" );
// >=
static_assert( "Ra" >= daw::static_string_t<char, 2>( "20" ), "" );
static_assert( daw::static_string_t<char, 2>( "20" ) >= "10", "" );
static_assert( daw::static_string_t<char, 2>( "30" ) >=
                 daw::static_string_t<char, 2>( "30" ),
               "" );
static_assert( daw::static_string_t<char, 2>( "99" ) >= "01kdfjskdfjsdlkfj10",
               "" );

static_assert( L"a0" >= daw::static_string_t<wchar_t, 2>( L"20" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"a0" ) >= L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"a0" ) >=
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"z0" ) >=
                 L"afkdfjskdfjsdlkfj10",
               "" );
// <=
static_assert( "10" <= daw::static_string_t<char, 2>( "20" ), "" );
static_assert( daw::static_string_t<char, 2>( "10" ) <= "10", "" );
static_assert( daw::static_string_t<char, 2>( "20" ) <=
                 daw::static_string_t<char, 2>( "30" ),
               "" );
static_assert( daw::static_string_t<char, 2>( "20" ) <= "afkdfjskdfjsdlkfj10",
               "" );

static_assert( L"10" <= daw::static_string_t<wchar_t, 2>( L"20" ), "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) <= L"20", "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"20" ) <=
                 daw::static_string_t<wchar_t, 2>( L"30" ),
               "" );
static_assert( daw::static_string_t<wchar_t, 2>( L"10" ) <=
                 L"afkdfjskdfjsdlkfj10",
               "" );

// Integral Conversions
static_assert( to_os_string<char>( 5 ) == "5", "" );

static_assert( to_os_string<char>( 5 ) == daw::static_string_t<char, 1>( "5" ),
               "" );

static_assert( to_os_string<char>( std::numeric_limits<int64_t>::min( ) ) ==
                 "-9223372036854775808",
               "" );

// Float conversions
static_assert( to_os_string<char>( 0.1f ) == "0.1", "" );
static_assert( to_os_string<char>( 0.12 ) == "0.11999999999999998", "" );

int main( ) {

	return 0;
}
