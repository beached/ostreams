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

#include "daw/io/bigint.h"

static_assert( daw::bits_needed_for_digits( 1 ) == 4 );
static_assert( daw::bits_needed_for_digits( 19 ) == 64 );
static_assert( daw::bits_needed_for_digits( 100 ) == 333 );
static_assert( daw::bits_needed_for_digits( 1000 ) == 3322 );

static_assert(
  daw::is_same_v<daw::bigint_digits_t<1000>, daw::bigint_t<3322>> );

template<typename Integer>
constexpr daw::bigint_t<104> mul_test( ) {
	daw::bigint_t<104> result( "91844674407370955161634534534543" );
	result = result * static_cast<Integer>( 100 );
	return result;
}

daw::bigint_t<104> mul_test2( ) {
	daw::bigint_t<104> result( 1 );
	result = result * 8589934592ULL;
	return result;
}

bool test_004( ) {
	auto v0 = daw::bigint_t<32>( std::numeric_limits<int32_t>::min( ) );
	auto v1 = daw::bigint_t<32>( "-2147483648" );
	return v0 == v1;
}

constexpr void test( bool ) {}

//#define static_assert test

void test_001( ) {
	constexpr daw::bigint_t<100> m( "18446744073709551616" );
	static_assert( m[0] == 0 );
	static_assert( m[1] == 0 );
	static_assert( m[2] == 1 );
}

void test_002( ) {
	constexpr daw::bigint_t<104> m1( "1844674407370955161634534534543" );
	static_assert( m1[0] == 0x0a6b'2d8f );
	static_assert( m1[1] == 0x0000'0008 );
	static_assert( m1[2] == 0x4876'e800 );
	static_assert( m1[3] == 0x0000'0017 );
}

void test_003( ) {
	constexpr daw::bigint_t<104> m2( "9184467440737095516163453453454300" );
	static_assert( mul_test<int>( ) == m2 );
	static_assert( mul_test<unsigned int>( ) == m2 );

	auto m3 = mul_test2( );
	// static_assert( mul_test2( ) == 8589934592ULL );
}

int main( ) {
	static_assert( daw::bigint_t<32>( std::numeric_limits<int32_t>::min( ) ) ==
	               daw::bigint_t<32>( "-2147483648" ) );

	static_assert( static_cast<intmax_t>( daw::bigint_t<32>(
	                 std::numeric_limits<int32_t>::min( ) ) ) ==
	               -2'147'483'648LL );

	static_assert( daw::bigint_t<64>( std::numeric_limits<int64_t>::min( ) ) ==
	               daw::bigint_t<64>( "-9223372036854775808" ) );

	static_assert( daw::bigint_t<64>( std::numeric_limits<int64_t>::min( ) ) ==
	               std::numeric_limits<int64_t>::min( ) );

	static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( ) == 0 );
	static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( 5 ) == 5 );
	static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( -5 ) == -5 );


	static_assert( -5'000'000'000LL == static_cast<intmax_t>( daw::bigint_t<100>(
	                                     -5'000'000'000LL ) ) );

	static_assert( 5'000'000'000LL == static_cast<intmax_t>( daw::bigint_t<100>(
	                                    5'000'000'000ULL ) ) );

	static_assert( 5'000'000'000LL ==
	               static_cast<intmax_t>( daw::bigint_t<100>( "5000000000" ) ) );

	static_assert( -5'000'000'000LL ==
	               static_cast<intmax_t>( daw::bigint_t<100>( "-5000000000" ) ) );

	static_assert( 5'000'000'000ULL == daw::bigint_t<100>( "5000000000" ) );

	static_assert( daw::bigint_t<100>( "5000000000" ) == 5'000'000'000ULL );

	static_assert( -5'000'000'000LL == daw::bigint_t<100>( "-5000000000" ) );

	static_assert( daw::bigint_t<100>( "-5000000000" ) == -5'000'000'000LL );

	test_001( );
	test_002( );
	test_003( );
	test_004( );
	return 0;
}
