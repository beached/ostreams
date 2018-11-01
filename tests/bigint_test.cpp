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
constexpr daw::bigint_t<113> mul_test( ) {
	auto result = daw::bigint_t<113>( "91844674407370955161634534534543" );
	result = result * static_cast<Integer>( 100 );
	return result;
}

constexpr daw::bigint_t<104> mul_test2( ) {
	daw::bigint_t<104> result( 1 );
	result = result * 8589934592ULL;
	return result;
}

constexpr bool test_004( ) {
	auto v0 = daw::bigint_t<32>( std::numeric_limits<int32_t>::min( ) );
	auto v1 = daw::bigint_t<32>( "-2147483648" );
	return v0 == v1;
}

constexpr void test( bool b ) {
    if( !b ) std::terminate( );
}

constexpr bool test_001( ) {
	daw::bigint_t<100> m( "18446744073709551616" );
	test( m[0] == 0 );
	test( m[1] == 0 );
	test( m[2] == 1 );
	return true;
}

//#define static_assert test

constexpr bool test_002( ) {
	daw::bigint_t<104> m1( "1844674407370955161634534534543" );
	test( m1[0] == 0x0a6b'2d8f );
	test( m1[1] == 0x0000'0008 );
	test( m1[2] == 0x4876'e800 );
	test( m1[3] == 0x0000'0017 );
	return true;
}

//#define static_assert test

constexpr bool test_003( ) {
	auto const m2 = daw::bigint_t<113>( "9184467440737095516163453453454300" );
	auto const m_test = mul_test<int>( );
	auto const is_eq = m_test == m2;
	test( is_eq );
	test( mul_test<unsigned int>( ) == m2 );
	auto const m3 = mul_test2( );
	auto const m_test2 = mul_test2( );
	test( mul_test2( ) == 8589934592ULL );
	return true;
}

//#undef static_assert

static_assert( daw::bigint_t<32>( std::numeric_limits<int32_t>::min( ) ) ==
               daw::bigint_t<32>( "-2147483648" ) );

static_assert( static_cast<intmax_t>( daw::bigint_t<32>(
                 std::numeric_limits<int32_t>::min( ) ) ) == -2'147'483'648LL );

static_assert( daw::bigint_t<64>( std::numeric_limits<int64_t>::min( ) ) ==
               daw::bigint_t<64>( "-9223372036854775808" ) );

static_assert( daw::bigint_t<64>( std::numeric_limits<int64_t>::min( ) ) ==
               std::numeric_limits<int64_t>::min( ) );

static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( ) == 0 );
static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( 5 ) == 5 );
static_assert( daw::bigint_t<daw::bsizeof<uint8_t>>( -5 ) == -5 );

static_assert( -5'000'000'000LL == static_cast<intmax_t>(
                                     daw::bigint_t<100>( -5'000'000'000LL ) ) );

static_assert( 5'000'000'000LL == static_cast<intmax_t>(
                                    daw::bigint_t<100>( 5'000'000'000ULL ) ) );

static_assert( 5'000'000'000LL ==
               static_cast<intmax_t>( daw::bigint_t<100>( "5000000000" ) ) );

static_assert( -5'000'000'000LL ==
               static_cast<intmax_t>( daw::bigint_t<100>( "-5000000000" ) ) );

static_assert( 5'000'000'000ULL == daw::bigint_t<100>( "5000000000" ) );

static_assert( daw::bigint_t<100>( "5000000000" ) == 5'000'000'000ULL );

static_assert( -5'000'000'000LL == daw::bigint_t<100>( "-5000000000" ) );

static_assert( daw::bigint_t<100>( "-5000000000" ) == -5'000'000'000LL );

int main( ) {
	static_assert( test_001( ) );
	static_assert( test_002( ) );
	test_003( );
	//static_assert( test_003( ) );
	static_assert( test_004( ) );
	return 0;
}
