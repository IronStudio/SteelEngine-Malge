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
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	T &MatrixRow<T, N>::operator[] (se::malge::Uint8 column) {
		SE_MALGE_ASSERT(column >= 0 && column < N, "You can't access a column of a matrix that doesn't exist");
		return *(reinterpret_cast<T**> (this)[column]);
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	const T &MatrixRow<T, N>::operator[] (se::malge::Uint8 column) const {
		SE_MALGE_ASSERT(column >= 0 && column < N, "You can't access a column of a matrix that doesn't exist");
		return *(reinterpret_cast<const T**> (this)[column]);
	}






	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename ...Args>
	SE_MALGE_REQUIRES((se::malge::IsMathType<Args> && ...) && (sizeof...(Args) <= N*N))
	Matrix<T, N>::Matrix(Args ...args) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T>) {
				auto reg {se::malge::simd::setZero<T> ()};
				_mm_store_ps(reinterpret_cast<T*> (this), reg);
				_mm_store_ps(reinterpret_cast<T*> (this) + 4, reg);
				_mm_store_ps(reinterpret_cast<T*> (this) + 8, reg);
				_mm_store_ps(reinterpret_cast<T*> (this) + 12, reg);
			}

			else {
		#endif

		memset(this, 0, sizeof(T) * 16);

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		if constexpr (sizeof...(Args) == 1) {
			reinterpret_cast<T*> (this)[0] = std::get<0> (std::forward_as_tuple (args...));
			reinterpret_cast<T*> (this)[5] = std::get<0> (std::forward_as_tuple (args...));

			if constexpr (N >= 3)
				reinterpret_cast<T*> (this)[10] = std::get<0> (std::forward_as_tuple (args...));
			
			if constexpr (N == 4)
				reinterpret_cast<T*> (this)[15] = std::get<0> (std::forward_as_tuple (args...));
		}

		else if constexpr (sizeof...(Args) != 0) {

			se::malge::Uint8 r {0};
			se::malge::Uint8 c {0};

			([this, &r, &c, &args] () {
				*(reinterpret_cast<T*> (this) + c * 4 + r) = args;
				++c;
				if (c >= N) {
					c = 0;
					++r;
				}
			} (), ...);

		}
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
	Matrix<T, N>::Matrix(const se::malge::Matrix<U, N> &matrix) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto reg1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix))};
				auto reg2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 4)};
				auto reg3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 8)};
				auto reg4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 12)};
				se::malge::simd::store<T> (reg1, reinterpret_cast<T*> (this));
				se::malge::simd::store<T> (reg2, reinterpret_cast<T*> (this) + 4);
				se::malge::simd::store<T> (reg3, reinterpret_cast<T*> (this) + 8);
				se::malge::simd::store<T> (reg4, reinterpret_cast<T*> (this) + 12);
			}

			else {
		#endif

		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * 16);
		}

		else {
			for (se::malge::Uint8 i {0}; i < 16; ++i)
				reinterpret_cast<T*> (this)[i] = static_cast<T> (reinterpret_cast<const U*> (&matrix)[i]);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
	Matrix<T, N>::Matrix(se::malge::Matrix<U, N> &&matrix) noexcept {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto reg1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix))};
				auto reg2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 4)};
				auto reg3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 8)};
				auto reg4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 12)};
				se::malge::simd::store<T> (reg1, reinterpret_cast<T*> (this));
				se::malge::simd::store<T> (reg2, reinterpret_cast<T*> (this) + 4);
				se::malge::simd::store<T> (reg3, reinterpret_cast<T*> (this) + 8);
				se::malge::simd::store<T> (reg4, reinterpret_cast<T*> (this) + 12);
			}

			else {
		#endif

		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * 16);
		}

		else {
			for (se::malge::Uint8 i {0}; i < 16; ++i)
				reinterpret_cast<T*> (this)[i] = static_cast<T> (reinterpret_cast<const U*> (&matrix)[i]);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator=(const se::malge::Matrix<U, N> &matrix) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto reg1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix))};
				auto reg2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 4)};
				auto reg3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 8)};
				auto reg4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 12)};
				se::malge::simd::store<T> (reg1, reinterpret_cast<T*> (this));
				se::malge::simd::store<T> (reg2, reinterpret_cast<T*> (this) + 4);
				se::malge::simd::store<T> (reg3, reinterpret_cast<T*> (this) + 8);
				se::malge::simd::store<T> (reg4, reinterpret_cast<T*> (this) + 12);
			}

			else {
		#endif

		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * 16);
		}

		else {
			for (se::malge::Uint8 i {0}; i < 16; ++i)
				reinterpret_cast<T*> (this)[i] = static_cast<T> (reinterpret_cast<const U*> (&matrix)[i]);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator=(se::malge::Matrix<U, N> &&matrix) noexcept {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto reg1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix))};
				auto reg2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 4)};
				auto reg3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 8)};
				auto reg4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (matrix) + 12)};
				se::malge::simd::store<T> (reg1, reinterpret_cast<T*> (this));
				se::malge::simd::store<T> (reg2, reinterpret_cast<T*> (this) + 4);
				se::malge::simd::store<T> (reg3, reinterpret_cast<T*> (this) + 8);
				se::malge::simd::store<T> (reg4, reinterpret_cast<T*> (this) + 12);
			}

			else {
		#endif

		if constexpr (std::is_same_v<T, U>) {
			memcpy(this, &matrix, sizeof(T) * 16);
		}

		else {
			for (se::malge::Uint8 i {0}; i < 16; ++i)
				reinterpret_cast<T*> (this)[i] = static_cast<T> (reinterpret_cast<const U*> (&matrix)[i]);
		}

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	se::malge::MatrixRow<T, N> Matrix<T, N>::operator[] (se::malge::Uint8 row) {
		SE_MALGE_ASSERT(row >= 0 && row < N, "You can't access a row of a matrix that doesn't exist");
		se::malge::MatrixRow<T, N> matrixRow {};
		reinterpret_cast<T**> (&matrixRow)[0] = reinterpret_cast<T*> (this) + row;
		reinterpret_cast<T**> (&matrixRow)[1] = reinterpret_cast<T*> (this) + row + 4;
		reinterpret_cast<T**> (&matrixRow)[2] = reinterpret_cast<T*> (this) + row + 8;
		reinterpret_cast<T**> (&matrixRow)[3] = reinterpret_cast<T*> (this) + row + 12;
		return matrixRow;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	se::malge::MatrixRow<T, N> Matrix<T, N>::operator[] (se::malge::Uint8 row) const {
		SE_MALGE_ASSERT(row >= 0 && row < N, "You can't access a row of a matrix that doesn't exist");
		se::malge::MatrixRow<T, N> matrixRow {};
		reinterpret_cast<const T**> (&matrixRow)[0] = reinterpret_cast<const T*> (this) + row;
		reinterpret_cast<const T**> (&matrixRow)[1] = reinterpret_cast<const T*> (this) + row + 4;
		reinterpret_cast<const T**> (&matrixRow)[2] = reinterpret_cast<const T*> (this) + row + 8;
		reinterpret_cast<const T**> (&matrixRow)[3] = reinterpret_cast<const T*> (this) + row + 12;
		return matrixRow;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
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
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
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
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<U, N>)
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator*=(const se::malge::Matrix<U, N> &matrix) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				T *outputMatrix {reinterpret_cast<T*> (this)};
				se::malge::Matrix<T, N> result {};

				if (this == &matrix)
					outputMatrix = reinterpret_cast<T*> (&result);

				auto lc1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this))};
				auto lc2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 4)};
				auto lc3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 8)};
				auto lc4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 12)};


				for (se::malge::Uint8 i {0}; i < N; ++i) {
					auto re1 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&matrix) + i * 4)};
					auto re2 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&matrix) + i * 4 + 1)};
					auto re3 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&matrix) + i * 4 + 2)};
					auto re4 {se::malge::simd::loadScalar<T> (reinterpret_cast<const T*> (&matrix) + i * 4 + 3)};


					auto mul1 {se::malge::simd::mul<T> (lc1, re1)};
					auto mul2 {se::malge::simd::mul<T> (lc2, re2)};
					auto mul3 {se::malge::simd::mul<T> (lc3, re3)};
					auto mul4 {se::malge::simd::mul<T> (lc4, re4)};

					auto tmp1 {se::malge::simd::add<T> (mul1, mul2)};
					auto tmp2 {se::malge::simd::add<T> (mul3, mul4)};
					auto res {se::malge::simd::add<T> (tmp1, tmp2)};

					se::malge::simd::store<T> (res, outputMatrix + 4 * i);
				}

				if (this == &matrix)
					*this = result;
			}

			else {
		#endif
		
		
		se::malge::Matrix<T, N> result {0};

		for (se::malge::Uint8 c {0}; c < N; ++c) {
			for (se::malge::Uint8 r {0}; r < N; ++r) {
				T value {0};
				for (se::malge::Uint8 i {0}; i < N; ++i)
					value += *(reinterpret_cast<const T*> (this) + c * 4 + i) * *(reinterpret_cast<const U*> (&matrix) + i * 4 + r);

				*(reinterpret_cast<T*> (&result) + c * 4 + r) = value;
			}
		}

		*this = result;


		#ifdef SE_MALGE_VECTORIZE
			}
		#endif


		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	template <typename U>
	SE_MALGE_REQUIRES(se::malge::IsMathType<U>)
	const se::malge::Matrix<T, N> &Matrix<T, N>::operator*=(U scalar) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto c1 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this))};
				auto c2 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 4)};
				auto c3 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 8)};
				auto c4 {se::malge::simd::load<T> (reinterpret_cast<const T*> (this) + 12)};
				auto s {se::malge::simd::loadScalar<T> (&scalar)};
				c1 = se::malge::simd::mul<T> (c1, s);
				c2 = se::malge::simd::mul<T> (c2, s);
				c3 = se::malge::simd::mul<T> (c3, s);
				c4 = se::malge::simd::mul<T> (c4, s);
				se::malge::simd::store<T> (c1, reinterpret_cast<T*> (this));
				se::malge::simd::store<T> (c2, reinterpret_cast<T*> (this + 4));
				se::malge::simd::store<T> (c3, reinterpret_cast<T*> (this + 8));
				se::malge::simd::store<T> (c4, reinterpret_cast<T*> (this + 12));
			}

			else {
		#endif
		

		for (se::malge::Uint8 c {0}; c < N; ++c) {
			for (se::malge::Uint8 r {0}; r < N; ++r) {
				*(reinterpret_cast<T*> (this) + c * 4 + r) *= scalar;
			}
		}


		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsValidMatrix<U, N>)
	se::malge::Matrix<T, N> operator+(se::malge::Matrix<T, N> lhs, const se::malge::Matrix<U, N> &rhs) {
		return lhs += rhs;
	}



	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsValidMatrix<U, N>)
	se::malge::Matrix<T, N> operator-(se::malge::Matrix<T, N> lhs, const se::malge::Matrix<U, N> &rhs) {
		return lhs -= rhs;
	}



	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsValidMatrix<U, N>)
	se::malge::Matrix<T, N> operator*(const se::malge::Matrix<U, N> &lhs, se::malge::Matrix<T, N> rhs) {
		return rhs *= lhs;
	}



	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsMathType<U>)
	se::malge::Matrix<T, N> operator*(se::malge::Matrix<T, N> matrix, U scalar) {
		return matrix *= scalar;
	}



	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsMathType<U>)
	se::malge::Matrix<T, N> operator*(U scalar, se::malge::Matrix<T, N> matrix) {
		return matrix *= scalar;
	}



	template <typename T, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N>)
	std::ostream &operator<<(std::ostream &stream, const se::malge::Matrix<T, N> &matrix) {
		for (se::malge::Uint8 r {0}; r < N; ++r) {
			stream << "|";

			for (se::malge::Uint8 c {0}; c < N; ++c) {
				stream << reinterpret_cast<const T*> (&matrix)[c * 4 + r];
				if (c != N - 1)
					stream << " ";
			}

			stream << "|";
			if (r != N - 1)
				stream << "\n";
		}

		return stream;
	}



} // namespace se::malge
