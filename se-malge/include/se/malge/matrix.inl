#include "se/malge/matrix.hpp"

#include <cstring>
#include <tuple>

#include "se/malge/assert.hpp"

#ifdef SE_MALGE_VECTORIZE
	#include "se/malge/simd.hpp"
#endif



namespace se::malge
{
	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	T &MatrixColumn<T, N>::operator[] (se::malge::Uint8 i) {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid matrix row index");
		return reinterpret_cast<T*> (this)[i];
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	const T &MatrixColumn<T, N>::operator[] (se::malge::Uint8 i) const {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid matrix row index");
		return reinterpret_cast<const T*> (this)[i];
	}






	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	template <typename ...Args>
	requires (se::malge::IsMathType<Args> && ...) && (sizeof...(Args) >= 0 && sizeof...(Args) <= N * N)
	Matrix<T, N>::Matrix(Args ...args) {
		if constexpr (sizeof...(Args) == 0) {
			memset(this, 0, sizeof(T) * 4 * N);
		}

		else if constexpr (sizeof...(Args) == 1) {
			memset(this, 0, sizeof(T) * 4 * N);
			*(reinterpret_cast<T*> (this) + 4 * 0 + 0) = std::get<0> (std::forward_as_tuple(args...));
			*(reinterpret_cast<T*> (this) + 4 * 1 + 1) = std::get<0> (std::forward_as_tuple(args...));
			if constexpr (N == 3) {
				*(reinterpret_cast<T*> (this) + 4 * 2 + 2) = std::get<0> (std::forward_as_tuple(args...));
			}

			else if constexpr (N == 4) {
				*(reinterpret_cast<T*> (this) + 4 * 2 + 2) = std::get<0> (std::forward_as_tuple(args...));
				*(reinterpret_cast<T*> (this) + 4 * 3 + 3) = std::get<0> (std::forward_as_tuple(args...));
			}
		}

		else {
			memset(this, 0, sizeof(T) * 4 * N);
			se::malge::Uint8 c {0};
			se::malge::Uint8 r {0};
			([&]() {
				*(reinterpret_cast<T*> (this) + 4 * c + r) = static_cast<T> (args);
				++r;
				if (r != N)
					return;
				
				r = 0;
				++c;
			} (), ...);
		}
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	template <typename U>
	requires se::malge::IsValidMatrix<U, N>
	Matrix<T, N>::Matrix(const se::malge::Matrix<U, N> &matrix) {
		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * N * 4);
		}

		else {
			for (se::malge::Uint8 c {0}; c < N; ++c) {
				for (se::malge::Uint8 r {0}; r < N; ++r)
					*(reinterpret_cast<T*> (this) + r + c * 4) = *(reinterpret_cast<const U*> (matrix) + r + c * 4);
			}
		}
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	template <typename U>
	requires se::malge::IsValidMatrix<U, N>
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator=(const se::malge::Matrix<U, N> &matrix) {
		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * N * 4);
		}

		else {
			for (se::malge::Uint8 c {0}; c < N; ++c) {
				for (se::malge::Uint8 r {0}; r < N; ++r)
					*(reinterpret_cast<T*> (this) + r + c * 4) = *(reinterpret_cast<const U*> (matrix) + r + c * 4);
			}
		}

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	se::malge::MatrixColumn<T, N> &Matrix<T, N>::operator[] (se::malge::Uint8 i) {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid matrix column index");
		return m_columns[i];
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	const se::malge::MatrixColumn<T, N> &Matrix<T, N>::operator[] (se::malge::Uint8 i) const {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid matrix column index");
		return m_columns[i];
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	template <typename U>
	requires se::malge::IsValidMatrix<U, N>
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator+=(const se::malge::Matrix<U, N> &matrix) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				typename se::malge::simd::Register<T>::Type a {};
				typename se::malge::simd::Register<T>::Type b {};

				for (se::malge::Uint8 c {0}; c < N; ++c) {
					a = se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 4 * c);
					b = se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix) + 4 * c);
					a = se::malge::simd::add<T> (a, b);
					se::malge::simd::store<T> (a, reinterpret_cast<T*> (this) + 4 * c);
				}
			}

			else {
		#endif
		
		for (se::malge::Uint8 c {0}; c < N; ++c) {
			for (se::malge::Uint8 r {0}; r < N; ++r)
				*(reinterpret_cast<T*> (this) + r + c * 4) += *(reinterpret_cast<const U*> (matrix) + r + c * 4);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	template <typename U>
	requires se::malge::IsValidMatrix<U, N>
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator-=(const se::malge::Matrix<U, N> &matrix) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				typename se::malge::simd::Register<T>::Type a {};
				typename se::malge::simd::Register<T>::Type b {};

				for (se::malge::Uint8 c {0}; c < N; ++c) {
					a = se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 4 * c);
					b = se::malge::simd::load<T> (reinterpret_cast<const T*> (&matrix) + 4 * c);
					a = se::malge::simd::sub<T> (a, b);
					se::malge::simd::store<T> (a, reinterpret_cast<T*> (this) + 4 * c);
				}
			}

			else {
		#endif
		
		for (se::malge::Uint8 c {0}; c < N; ++c) {
			for (se::malge::Uint8 r {0}; r < N; ++r)
				*(reinterpret_cast<T*> (this) + r + c * 4) -= *(reinterpret_cast<const U*> (matrix) + r + c * 4);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	std::ostream &operator<<(std::ostream &stream, const se::malge::Matrix<T, N> &matrix) {
		for (se::malge::Uint8 c {0}; c < N; ++c) {
			stream << "| ";
			for (se::malge::Uint8 r {0}; r < N; ++r) {
				stream << *(reinterpret_cast<const T*> (&matrix) + r + c * 4);
				if (r != N - 1)
					stream << ", ";
			}

			stream << " |";
			if (c != N - 1)
				stream << "\n";
		}

		return stream;
	}



} // namespace se::malge
