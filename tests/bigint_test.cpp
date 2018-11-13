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
static_assert( test_004( ) );

constexpr bool test_1element_plus1( ) {
	auto v0 = daw::bigint_t<48>(
	  static_cast<uint64_t>( std::numeric_limits<uint32_t>::max( ) ) + 1ULL );
	v0 += static_cast<uint64_t>( std::numeric_limits<uint32_t>::max( ) );
	v0 += 1ULL;
	return v0.size( ) > 1ULL;
}
static_assert( test_1element_plus1( ) );

constexpr bool test_uintmax_plus1( ) {
	auto v0 = daw::bigint_t<48>( std::numeric_limits<uint64_t>::max( ) );
	v0 += 1ULL;
	return v0.size( ) == 3 and v0[0] == 0 and v0[1] == 0 and v0[2] == 1;
}
static_assert( test_uintmax_plus1( ) );

constexpr bool test_1element_times2( ) {
	auto v0 = daw::bigint_t<48>(
	  static_cast<uint64_t>( std::numeric_limits<uint32_t>::max( ) ) );
	v0 *= 2ULL;
	return v0.size( ) > 1ULL;
}
static_assert( test_1element_times2( ) );

constexpr bool test_1element_times_1element( ) {
	auto v0 = daw::bigint_t<48>(
	  static_cast<uint64_t>( std::numeric_limits<uint32_t>::max( ) ) );
	v0 *= std::numeric_limits<uint32_t>::max( );
	return v0.size( ) == 2 and v0[0] == 1 and v0[1] == 0xFFFF'FFFE;
}
static_assert( test_1element_times_1element( ) );

constexpr bool test_uintmax_times_uintmax( ) {
	auto v0 = daw::bigint_t<130>( std::numeric_limits<uint64_t>::max( ) );
	v0 *= std::numeric_limits<uint64_t>::max( );
	return v0.size( ) == 4 and v0[0] == 0 and v0[1] == 1 and
	       v0[2] == 0xFFFF'FFFF and v0[3] == 0xFFFF'FFFE;
}
static_assert( test_uintmax_times_uintmax( ) );

constexpr bool test_pow2_32_max( ) {
	auto v0 = daw::bigint_t<128>::pow2( 32 );
	return v0.size( ) == 2 and v0[0] == 0 and v0[1] == 1;
}
static_assert( test_pow2_32_max( ) );

constexpr bool test_pow2_64_max( ) {
	auto v0 = daw::bigint_t<128>::pow2( 64 );
	return v0.size( ) == 3 and v0[0] == 0 and v0[1] == 0 and v0[2] == 1;
}
static_assert( test_pow2_64_max( ) );

constexpr bool test_one_shl_minus_1_31( ) {
	auto v0 = daw::bigint_t<64>::one_shl_minus1( 31 );
	return v0.size( ) == 1 and v0[0] == 0xFFFF'FFFE;
}
static_assert( test_one_shl_minus_1_31( ) );

constexpr bool test_one_shl_minus_1_32( ) {
	auto v0 = daw::bigint_t<64>::one_shl_minus1( 32 );
	return v0.size( ) == 2 and v0[0] == 0xFFFF'FFFE and v0[1] == 1;
}
static_assert( test_one_shl_minus_1_32( ) );

constexpr bool test_one_shl_minus_1_64( ) {
	auto v0 = daw::bigint_t<64>::one_shl_minus1( 64 );
	return v0.size( ) == 3 and v0[0] == 0xFFFF'FFFE and v0[1] == 0xFFFF'FFFF and
	       v0[2] == 1;
}
static_assert( test_one_shl_minus_1_64( ) );

constexpr void test( bool b ) {
	if( !b )
		std::terminate( );
}

constexpr bool test_001( ) {
	daw::bigint_t<100> m( "18446744073709551616" );
	return m[0] == 0 and m[1] == 0 and m[2] == 1;
}
static_assert( test_001( ) );

//#define static_assert test

constexpr bool test_002( ) {
	daw::bigint_t<104> m1( "1844674407370955161634534534543" );
	return m1[0] == 0x0A6B'2D8F and m1[1] == 0x0000'0008 and
	       m1[2] == 0x4876'E800 and m1[3] == 0x0000'0017;
}
static_assert( test_002( ) );

//#define static_assert test

constexpr bool test_003( ) {
	auto const m2a = daw::bigint_t<113>( "9184467440737095516163453453454300" );
	auto m2b = daw::bigint_t<113>( "91844674407370955161634534534543" );
	m2b *= 100ULL;
	auto result = m2a == m2b;
	auto m2c = daw::bigint_t<113>( "91844674407370955161634534534543" ) * 100ULL;
	result = result and m2a == m2c;
	auto const m_test2 = mul_test2( );
	result = result and mul_test2( ) == 8589934592ULL;
	return result;
}
//static_assert( test_003( ) );

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
	test_one_shl_minus_1_31( );
	return 0;
}
