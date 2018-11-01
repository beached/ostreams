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

#include <array>

#include <daw/daw_bit.h>
#include <daw/daw_exception.h>
#include <daw/daw_math.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	enum class sign_t : int_fast8_t { positive = 1, negative = -1 };
	namespace impl {
		static_assert( sizeof( uintmax_t ) == sizeof( uint64_t ),
		               "Assumed that uint64_t is largest unsigned integer type" );
		static_assert( sizeof( intmax_t ) == sizeof( int64_t ),
		               "Assumed that uint64_t is largest integer type" );

		template<typename T = uint8_t>
		constexpr T to_digit( char c ) noexcept {
			if( '0' <= c && c <= '9' ) {
				return static_cast<T>( c - '0' );
			}
			if( 'a' <= c && c <= 'f' ) {
				return static_cast<T>( 10 ) + static_cast<T>( c - 'a' );
			}
			if( 'A' <= c && c <= 'F' ) {
				return static_cast<T>( 10 ) + static_cast<T>( c - 'A' );
            } else {    // Work around constexpr bug in gcc 8.2.  Need to put else clause
                std::terminate();
            }
		}

		template<typename T = uint8_t>
		constexpr T to_digit( wchar_t c ) noexcept {
			if( L'0' <= c && c <= L'9' ) {
				return static_cast<T>( c - L'0' );
			}
			if( L'a' <= c && c <= L'f' ) {
				return static_cast<T>( 10 ) + static_cast<T>( c - L'a' );
			}
			if( 'A' <= c && c <= 'F' ) {
				return static_cast<T>( 10 ) + static_cast<T>( c - L'A' );
			} else {    // Work around constexpr bug in gcc 8.2.  Need to put else clause
                std::terminate();
            }
		}

		template<typename T>
		using half_max_t = std::conditional_t<
		  sizeof( T ) == 8, uint32_t,
		  std::conditional_t<sizeof( T ) == 4, uint16_t,
		                     std::conditional_t<sizeof( T ) == 2, uint8_t, void>>>;

		constexpr auto low_part( uintmax_t value ) noexcept {
			using value_t = half_max_t<uintmax_t>;
			auto const lower_mask =
			  daw::get_left_mask<uintmax_t>( bsizeof<uintmax_t> - bsizeof<value_t> );

			return static_cast<value_t>( value bitand lower_mask );
		}

		constexpr auto high_part( uintmax_t value ) noexcept {
			using value_t = half_max_t<uintmax_t>;
			auto const high_mask =
			  daw::get_right_mask<uintmax_t>( bsizeof<uintmax_t> - bsizeof<value_t> );

			return static_cast<value_t>( value bitand high_mask );
		}

		template<typename T, size_t N>
		struct bigint_storage_t {
			using value_type = T;
			using size_type = size_t;
			using reference = value_type &;
			using const_reference = value_type const &;
			using iterator = typename std::array<value_type, N>::iterator;
			using const_iterator = typename std::array<value_type, N>::const_iterator;

			static_assert( N > 0, "Cannot store 0 bytes of anything" );

			std::array<value_type, N> m_data = {0};
			size_type m_idx = 0;
			sign_t m_sign = sign_t::positive;

			constexpr bigint_storage_t( ) noexcept = default;

			constexpr void clear( ) noexcept {
				while( m_idx > 0 ) {
					--m_idx;
					m_data[m_idx] = 0;
				}
			}

			template<typename Size>
			constexpr reference operator[]( Size idx ) noexcept {
				daw::exception::dbg_precondition_check<std::out_of_range>( in_range( static_cast<size_t>( idx ), 0ULL, N ) );
				return m_data[static_cast<size_type>( idx )];
			}

			template<typename Size>
			constexpr const_reference operator[]( Size idx ) const noexcept {
				daw::exception::dbg_precondition_check<std::out_of_range>( in_range( static_cast<size_t>( idx ), 0ULL, N ) );
				return m_data[static_cast<size_type>( idx )];
			}

			constexpr size_type const &size( ) const noexcept {
				return m_idx;
			}

			constexpr size_type &size( ) noexcept {
				return m_idx;
			}

			constexpr bool empty( ) const noexcept {
				return m_idx == 0;
			}

			constexpr iterator begin( ) noexcept {
				return m_data.begin( );
			}

			constexpr const_iterator begin( ) const noexcept {
				return m_data.begin( );
			}

			constexpr const_iterator cbegin( ) const noexcept {
				return m_data.cbegin( );
			}

			constexpr iterator end( ) noexcept {
				return m_data.begin( ) + static_cast<ptrdiff_t>( m_idx );
			}

			constexpr const_iterator end( ) const noexcept {
				return m_data.begin( ) + static_cast<ptrdiff_t>( m_idx );
			}

			constexpr const_iterator cend( ) const noexcept {
				return m_data.cbegin( ) + static_cast<ptrdiff_t>( m_idx );
			}

			constexpr size_type capacity( ) const noexcept {
				return N;
			}

			constexpr bool full( ) noexcept {
				return size( ) > capacity( );
			}

			constexpr void sign_flip( ) noexcept {
				m_sign =
				  m_sign == sign_t::positive ? sign_t::negative : sign_t::positive;
			}

			constexpr void push_back( value_type v ) {
				daw::exception::dbg_precondition_check<std::overflow_error>(
				  not full( ) );

				m_data[m_idx++] = v;
			}

			constexpr reference back( ) {
				daw::exception::dbg_precondition_check<std::out_of_range>(
				  !empty( ), "Attempt to access value on empty stack" );
				return m_data[m_idx - 1];
			}

			constexpr const_reference back( ) const {
				daw::exception::dbg_precondition_check<std::out_of_range>(
				  !empty( ), "Attempt to access value on empty stack" );
				return m_data[m_idx - 1];
			}

			constexpr value_type pop_back( ) {
				daw::exception::dbg_precondition_check<std::overflow_error>(
				  !empty( ), "Attempt to pop empty stack" );
				auto result = back( );
				back( ) = 0;
				--m_idx;
				return result;
			}
		};

		template<typename T, size_t BitsNeeded>
		constexpr size_t elements_needed( ) noexcept {
			size_t const bits_in_T = bsizeof<T>;
			size_t result = ( BitsNeeded + ( bits_in_T / 2 ) ) / bits_in_T;
			if( result == 0 ) {
				return 1;
			}
			return result;
		}

		template<typename T, typename U>
		constexpr auto rdiv( T lhs, U rhs ) noexcept {
			auto result = lhs / rhs;
			if( result * rhs < lhs ) {
				++result;
			}
			return result;
		}

		constexpr auto overflow( uintmax_t &carry ) noexcept {
			using value_t = half_max_t<uintmax_t>;
			auto result = low_part( carry );
			carry >>= bsizeof<value_t>;
			return result;
		}

		template<typename value_t, size_t N>
		constexpr void add( impl::bigint_storage_t<value_t, N> &lhs, value_t rhs,
		                    size_t index = 0 ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			daw::exception::precondition_check( rhs >= 0 );

			auto carry = static_cast<uintmax_t>( rhs );

			for( ; index <lhs.size( ); ++index ) {
				carry += lhs[index];
				lhs[index] = overflow( carry );
			}
			if( carry > 0 ) {
				lhs.push_back( static_cast<value_t>( carry ) );
			}
		}

		template<typename value_t, size_t N>
		constexpr void sub( impl::bigint_storage_t<value_t, N> &lhs,
		                    value_t ) noexcept( !MAY_THROW_EXCEPTIONS ) {
			/*
			      auto const lower_mask =
			          daw::get_left_mask<uintmax_t>( bsizeof<uintmax_t> -
			   bsizeof<value_t> );

			      if( lhs_idx == 0 ) {
			        daw::exception::precondition_check( rhs == 0, "We should never be
			   calling sub with a rhs > lhs and lhs == 0" ); return;
			      }
			      if( static_cast<size_t>( bit_pos ) <= lhs_idx ) {

			      }
			      uintmax_t carry = rhs;
			      for( size_t n=static_cast<size_t>( bit_pos ); n<idx; ++n ) {
			        if( lhs[n] >= carry ) {
			          lhs[n] -= carry;
			        } else {

			        }
			        carry += lhs[n];
			        lhs[n] = static_cast<value_t>( carry bitand lower_mask );
			        carry >>= bsizeof<value_t>;
			      }
			      if( carry > 0 ) {
			        daw::exception::precondition_check<std::overflow_error>( idx <
			   lhs.size( ) ); lhs[idx++] = static_cast<value_t>( carry );
			      }
			      */
		}

		template<typename value_t, size_t N>
		constexpr void add( impl::bigint_storage_t<value_t, N> &lhs,
		                    uintmax_t rhs ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			auto const lower = low_part( rhs );
			if( lower > 0 ) {
				add( lhs, lower, 0 );
			}

			auto const higher = high_part( rhs );
			if( higher > 0 ) {
				add( lhs, higher, 1 );
			}
		}

		template<typename value_t, size_t N>
		constexpr void add( impl::bigint_storage_t<value_t, N> &lhs,
		                    impl::bigint_storage_t<value_t, N> const
		                      &rhs ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			for( size_t n = 0; n < rhs.size( ); ++n ) {
				add( lhs, rhs[n], n );
			}
		}

		template<typename value_t, size_t N>
		constexpr void mul( impl::bigint_storage_t<value_t, N> &lhs, value_t rhs,
		                    size_t index ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			if( rhs == 0ULL ) {
				lhs.clear( );
				return;
			}

			uintmax_t carry = 0;

			auto tmp = lhs;
			for( size_t pos = 0ULL; pos < lhs.size( ); ++pos ) {
				carry +=
				  static_cast<uintmax_t>( lhs[pos] ) * static_cast<uintmax_t>( rhs );
				tmp[pos] = overflow( carry );
			}
			if( carry > 0 ) {
				tmp.push_back( static_cast<value_t>( carry ) );
				carry = 0;
			}
			lhs = tmp;
			if( index > 0 ) {
				while (index > 0) {
					--index;
					for (size_t pos = 0; pos < lhs.size(); ++pos) {
						carry +=
								static_cast<uintmax_t>( lhs[pos] ) *
								static_cast<uintmax_t>( std::numeric_limits<value_t>::max());
						tmp[pos] = overflow(carry);
					}
				}
				if (carry > 0) {
					tmp.push_back(static_cast<value_t>( carry ));
				}
				lhs = tmp;
			}
		}

		template<typename value_t, size_t N>
		constexpr void mul( impl::bigint_storage_t<value_t, N> &lhs,
		                    uintmax_t rhs ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			auto tmp = lhs;

			mul( lhs, low_part( rhs ), 0 );
			mul( tmp, high_part( rhs ), 1 );
			add( lhs, tmp );
		}

		template<typename value_t, size_t N>
		constexpr void mul( impl::bigint_storage_t<value_t, N> &lhs,
		                    impl::bigint_storage_t<value_t, N> const
		                      &rhs ) noexcept( !MAY_THROW_EXCEPTIONS ) {

			if( lhs.empty( ) ) {
				return;
			}
			if( rhs.empty( ) ) {
				lhs.clear( );
				return;
			}

			bigint_storage_t<value_t, N> result{};
			result.push_back( 0 );

			for( size_t n = 0; n < rhs.size( ); ++n ) {
				auto tmp = lhs;
				mul( tmp, rhs[n], n );
				add( result, tmp );
			}
			lhs = std::move( result );
		}
	} // namespace impl

	// This will calculate log2( 10^value ) + 1
	// It is used to calculate how many bits are needed to represent a
	// base 10 number with value digits
	constexpr size_t bits_needed_for_digits( size_t value ) noexcept {
		double const log2_10 = 3.32192809488736218;
		// result = log2( 10^value ) + 1;
		return static_cast<size_t>( static_cast<double>( value ) * log2_10 ) + 1;
	}

	template<size_t Bits>
	struct bigint_t {
		using value_t = impl::half_max_t<uintmax_t>;

	private:
		static constexpr size_t m_capacity =
		  impl::elements_needed<value_t, Bits>( );

	public:
		static constexpr size_t bits_v = m_capacity * 8;

		static_assert( !daw::is_signed_v<value_t>,
		               "Unsupported T, must be unsigned" );
		static_assert( sizeof( value_t ) * 2 <= sizeof( uintmax_t ),
		               "T must be no more than the size of uintmax_t" );

	private:
		constexpr void sign_flip( ) noexcept {
			m_data.sign_flip( );
		}

		impl::bigint_storage_t<value_t, m_capacity + 1> m_data{};

	public:
		constexpr bigint_t( ) noexcept = default;

		template<typename SignedInteger,
		         std::enable_if_t<is_signed_v<remove_cvref_t<SignedInteger>>,
		                          std::nullptr_t> = nullptr>
		explicit constexpr bigint_t( SignedInteger v ) noexcept(
		  !MAY_THROW_EXCEPTIONS ) {

			m_data.m_sign = v < 0 ? sign_t::negative : sign_t::positive;
			uintmax_t value = 0;
			if( v == std::numeric_limits<intmax_t>::min( ) ) {
				// On intmax_t one cannot multiple min( ) by -1 as max( ) is abs( min(
				// ) ) - 1
				value =
				  static_cast<uintmax_t>( std::numeric_limits<intmax_t>::max( ) ) +
				  1ULL;
			} else {
				value =
				  static_cast<uintmax_t>( v * static_cast<intmax_t>( m_data.m_sign ) );
			}

			size_t elem_needed =
			  impl::rdiv( bsizeof<SignedInteger>, bsizeof<value_t> );

			while( elem_needed > 0 ) {
				--elem_needed;
				m_data.push_back( impl::overflow( value ) );
			}
			daw::exception::dbg_precondition_check( value == 0 );
		}

		template<typename UnsignedInteger,
		         std::enable_if_t<!is_signed_v<remove_cvref_t<UnsignedInteger>>,
		                          std::nullptr_t> = nullptr>
		explicit constexpr bigint_t( UnsignedInteger v ) noexcept(
		  !MAY_THROW_EXCEPTIONS ) {

			uintmax_t value = v;
			m_data.m_sign = value < 0 ? sign_t::negative : sign_t::positive;
			value *= static_cast<uintmax_t>( m_data.m_sign );

			while( value > 0 ) {
				m_data.push_back( impl::overflow( value ) );
			}
			daw::exception::dbg_precondition_check( value == 0 );
		}

		// TODO: add octal/hex/binary via 0o, 0x, 0b
		template<typename CharT>
		explicit constexpr bigint_t( basic_string_view<CharT> str ) noexcept(
		  !MAY_THROW_EXCEPTIONS ) {

			str = daw::parser::trim_left( str );
			daw::exception::precondition_check( !str.empty( ) );
			if( str.front( ) == '-' ) {
				str.remove_prefix( );
				m_data.m_sign = sign_t::negative;
			} else {
				if( str.front( ) == '+' ) {
					str.remove_prefix( );
				}
				m_data.m_sign = sign_t::positive;
			}

			// TODO non char input and other bases
			while( !str.empty( ) and daw::parser::is_number( str.front( ) ) ) {
				auto digit =
				  static_cast<uintmax_t>( impl::to_digit( str.pop_front( ) ) );
				impl::mul( m_data, 10UL );
				impl::add( m_data, digit );
			}
		}

		template<typename CharT, size_t N>
		explicit constexpr bigint_t( CharT const ( &str )[N] )
		  : bigint_t(
		      daw::basic_string_view<CharT>( str, str[N - 1] == 0 ? N - 1 : N ) ) {}

		explicit constexpr operator intmax_t( ) const
		  noexcept( !MAY_THROW_EXCEPTIONS ) {

			daw::exception::precondition_check(
			  m_data.empty( ) or
			  ( m_data.size( ) - 1 ) * sizeof( value_t ) < sizeof( intmax_t ) );

			intmax_t result = 0;

			auto pos = static_cast<intmax_t>( m_data.size( ) );
			while( --pos >= 0 ) {
				result <<= bsizeof<value_t>;
				result |= m_data[pos];
			}
			// TODO make sure this is not going to be intmax_t::min( )
			if( m_data.m_sign == sign_t::negative ) {
				result = -result;
			}
			return result;
		}

		static constexpr size_t capacity( ) noexcept {
			return m_capacity;
		}

		constexpr size_t size( ) const noexcept {
			return m_data.size( );
		}

		constexpr value_t const &operator[]( size_t idx ) const noexcept {
			return m_data[idx];
		}

		template<size_t>
		friend struct bigint_t;

		template<typename Integer,
		         std::enable_if_t<!is_integral_v<remove_cvref_t<Integer>>,
		                          std::nullptr_t> = nullptr>
		constexpr bigint_t &
		operator*=( Integer &&value ) noexcept( !MAY_THROW_EXCEPTIONS ) {
			impl::mul( m_data, bigint_t( std::forward<Integer>( value ) ) );
			return *this;
		}

		template<typename Integer,
		         std::enable_if_t<is_integral_v<remove_cvref_t<Integer>>,
		                          std::nullptr_t> = nullptr>
		constexpr bigint_t operator*( Integer &&value ) const
		  noexcept( !MAY_THROW_EXCEPTIONS ) {

			auto result = bigint_t( std::forward<Integer>( value ) );
			impl::mul( result.m_data, m_data );
			return result;
		}

		template<typename Integer,
		         std::enable_if_t<is_integral_v<remove_cvref_t<Integer>>,
		                          std::nullptr_t> = nullptr>
		constexpr bigint_t &
		operator+=( Integer &&value ) noexcept( !MAY_THROW_EXCEPTIONS ) {
			impl::add( m_data, bigint_t( std::forward<Integer>( value ) ) );
			return *this;
		}

		template<typename Integer,
		         std::enable_if_t<is_integral_v<remove_cvref_t<Integer>>,
		                          std::nullptr_t> = nullptr>
		constexpr bigint_t operator+( Integer &&value ) const
		  noexcept( !MAY_THROW_EXCEPTIONS ) {
			auto result = *this;
			impl::add( result, bigint_t( std::forward<Integer>( value ) ) );
			return result;
		}
	};

	template<size_t base10_digits>
	using bigint_digits_t =
	  bigint_t<daw::bits_needed_for_digits( base10_digits )>;

	namespace impl {
		template<size_t N>
		constexpr std::true_type is_bigint_test( bigint_t<N> const & ) noexcept;

		template<size_t N>
		constexpr std::true_type is_bigint_test( bigint_t<N> && ) noexcept;

		constexpr std::false_type is_bigint_test( ... ) noexcept;
	} // namespace impl

	template<typename T>
	using is_bigint_t =
	  decltype( impl::is_bigint_test( std::declval<remove_cvref_t<T>>( ) ) );

	template<typename T>
	constexpr bool const is_bigint_v = is_bigint_t<T>::value;

	template<
	  typename BigIntLhs, typename BigIntRhs,
	  std::enable_if_t<all_true_v<is_bigint_v<BigIntLhs>, is_bigint_v<BigIntRhs>>,
	                   std::nullptr_t> = nullptr>
	constexpr bool operator==( BigIntLhs &&lhs, BigIntRhs &&rhs ) noexcept {

		if( lhs.size( ) != rhs.size( ) ) {
			if( lhs.size( ) == 0 ) {
				for( size_t n = 0; n < rhs.size( ); ++n ) {
					if( rhs[n] != 0 ) {
						return false;
					}
				}
				return true;
			} else if( rhs.size( ) == 0 ) {
				for( size_t n = 0; n < lhs.size( ); ++n ) {
					if( lhs[n] != 0 ) {
						return false;
					}
				}
				return true;
			}
			return false;
		}
		for( size_t n = 0; n < lhs.size( ); ++n ) {
			if( lhs[n] != rhs[n] ) {
				return false;
			}
		}
		return true;
	}

	template<
	  typename BigInt, typename Integer,
	  std::enable_if_t<all_true_v<is_bigint_v<BigInt>, is_integral_v<Integer>>,
	                   std::nullptr_t> = nullptr>
	constexpr bool operator==( BigInt &&lhs, Integer &&rhs ) noexcept {
		using rhs_bigint = bigint_t<bsizeof<Integer>>;

		return std::forward<BigInt>( lhs ) ==
		       rhs_bigint( std::forward<Integer>( rhs ) );
	}

	template<
	  typename BigInt, typename Integer,
	  std::enable_if_t<all_true_v<is_bigint_v<BigInt>, is_integral_v<Integer>>,
	                   std::nullptr_t> = nullptr>
	constexpr bool operator==( Integer &&lhs, BigInt &&rhs ) noexcept {
		using lhs_bigint = bigint_t<bsizeof<Integer>>;

		return lhs_bigint( std::forward<Integer>( lhs ) ) ==
		       std::forward<BigInt>( rhs );
	}
} // namespace daw
