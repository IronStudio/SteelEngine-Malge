#include "se/malge/operations.hpp"



namespace se::malge
{
	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>)
	se::malge::Vector<T, N> operator*(const se::malge::Matrix<T, N> &matrix, const se::malge::Vector<U, N> &vector) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				se::malge::Vector<T, N> result {};

				auto lc1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix))};
				auto lc2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix) + 4)};
				auto lc3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix) + 8)};
				auto lc4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix) + 12)};

				auto re1 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&vector))};
				auto re2 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&vector) + 1)};
				auto re3 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&vector) + 2)};
				auto re4 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&vector) + 3)};

				auto mul1 {se::malge::simd::mul<T> (lc1, re1)};
				auto mul2 {se::malge::simd::mul<T> (lc2, re2)};
				auto mul3 {se::malge::simd::mul<T> (lc3, re3)};
				auto mul4 {se::malge::simd::mul<T> (lc4, re4)};

				auto tmp1 {se::malge::simd::add<T> (mul1, mul2)};
				auto tmp2 {se::malge::simd::add<T> (mul3, mul4)};
				auto res {se::malge::simd::add<T> (tmp1, tmp2)};

				se::malge::simd::store<T> (res, reinterpret_cast<T*> (&result));

				return result;
			}

			else {
		#endif
		
		
		se::malge::Vector<T, N> result {0};

		for (se::malge::Uint8 r {0}; r < N; ++r) {
			T value {0};
			for (se::malge::Uint8 i {0}; i < N; ++i)
				value += *(reinterpret_cast<const T*> (&matrix) + 4 * i + r) * *(reinterpret_cast<const U*> (&vector) + i);

			*(reinterpret_cast<T*> (&result) + r) = value;
		}

		return result;


		#ifdef SE_MALGE_VECTORIZE
			}
		#endif
	}



} // namespace se::malge
