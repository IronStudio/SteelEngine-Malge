#include "se/malge/vector.hpp"

#include <cmath>
#include <cstring>
#include <tuple>

#include "se/malge/assert.hpp"

#ifdef SE_MALGE_VECTORIZE
	#include "se/malge/simd.hpp"
#endif



namespace se::malge
{
	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	Vector<T, N>::Vector() :
		se::malge::Vector<T, N> (static_cast<T> (0))
	{
		
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename Fill>
	requires se::malge::IsMathType<Fill>
	Vector<T, N>::Vector(Fill fill) {
		reinterpret_cast<T*> (this)[0] = static_cast<T> (fill);
		reinterpret_cast<T*> (this)[1] = static_cast<T> (fill);
		if constexpr (N == 3) {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (fill);
			reinterpret_cast<T*> (this)[3] = static_cast<T> (0);
		}

		else if constexpr (N == 4) {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (fill);
			reinterpret_cast<T*> (this)[3] = static_cast<T> (fill);
		}

		else {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (0);
			reinterpret_cast<T*> (this)[3] = static_cast<T> (0);
		}
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename ...Args>
	requires (se::malge::IsMathType<Args> && ...) && (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
	Vector<T, N>::Vector(Args ...args) {
		reinterpret_cast<T*> (this)[0] = static_cast<T> (std::get<0> (std::forward_as_tuple(args...)));
		reinterpret_cast<T*> (this)[1] = static_cast<T> (std::get<1> (std::forward_as_tuple(args...)));

		if constexpr (sizeof...(Args) == 3) {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (std::get<2> (std::forward_as_tuple(args...)));
			reinterpret_cast<T*> (this)[3] = static_cast<T> (0);
		}

		else if constexpr (sizeof...(Args) == 4) {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (std::get<2> (std::forward_as_tuple(args...)));
			reinterpret_cast<T*> (this)[3] = static_cast<T> (std::get<3> (std::forward_as_tuple(args...)));
		}

		else {
			reinterpret_cast<T*> (this)[2] = static_cast<T> (0);
			reinterpret_cast<T*> (this)[3] = static_cast<T> (0);
		}
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U, se::malge::Uint8 M, typename ...Args>
	requires se::malge::IsValidVector<U, M> && (se::malge::IsMathType<Args> && ...) && (N >= M) && (sizeof...(Args) <= N - M)
	Vector<T, N>::Vector(const se::malge::Vector<U, M> &vector, Args ...args) {
		if constexpr (M == 2) {
			*this = se::malge::Vector<T, N> (vector.x, vector.y, args...);
		}

		else if constexpr (M == 3) {
			*this = se::malge::Vector<T, N> (vector.x, vector.y, vector.y, args...);
		}

		else {
			*this = se::malge::Vector<T, N> (vector.x, vector.y, vector.y, vector.z);
		}
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsValidVector<U, N>
	const se::malge::Vector<T, N> &Vector<T, N>::operator=(const se::malge::Vector<U, N> &vector) {
		memcpy(this, &vector, sizeof(T) * N);
		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsMathType<U>
	const se::malge::Vector<T, N> &Vector<T, N>::operator+=(const se::malge::Vector<U, N> &vector) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load(reinterpret_cast<const T*> (this))};
				auto b {se::malge::simd::load(reinterpret_cast<const T*> (&vector))};
				auto c {se::malge::simd::add<T> (a, b)};
				se::malge::simd::store(c, reinterpret_cast<T*> (this));
			}

			else {
		#endif

		for (se::malge::Uint8 i {0}; i < N; ++i)
			reinterpret_cast<T*> (this)[i] += reinterpret_cast<const U*> (&vector)[i];

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsMathType<U>
	const se::malge::Vector<T, N> &Vector<T, N>::operator-=(const se::malge::Vector<U, N> &vector) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load(reinterpret_cast<const T*> (this))};
				auto b {se::malge::simd::load(reinterpret_cast<const T*> (&vector))};
				auto c {se::malge::simd::sub<T> (a, b)};
				se::malge::simd::store(c, reinterpret_cast<T*> (this));
			}

			else {
		#endif

		for (se::malge::Uint8 i {0}; i < N; ++i)
			reinterpret_cast<T*> (this)[i] -= reinterpret_cast<const U*> (&vector)[i];

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsMathType<U>
	const se::malge::Vector<T, N> &Vector<T, N>::operator*=(const se::malge::Vector<U, N> &vector) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load(reinterpret_cast<const T*> (this))};
				auto b {se::malge::simd::load(reinterpret_cast<const T*> (&vector))};
				auto c {se::malge::simd::mul<T> (a, b)};
				se::malge::simd::store(c, reinterpret_cast<T*> (this));
			}

			else {
		#endif

		for (se::malge::Uint8 i {0}; i < N; ++i)
			reinterpret_cast<T*> (this)[i] *= reinterpret_cast<const U*> (&vector)[i];

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsMathType<U>
	const se::malge::Vector<T, N> &Vector<T, N>::operator*=(U scalar) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load(reinterpret_cast<const T*> (this))};
				auto b {se::malge::simd::loadScalar(&scalar)};
				auto c {se::malge::simd::mul<T> (a, b)};
				se::malge::simd::store(c, reinterpret_cast<T*> (this));
			}

			else {
		#endif

		for (se::malge::Uint8 i {0}; i < N; ++i)
			reinterpret_cast<T*> (this)[i] *= scalar;

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif

		return *this;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	template <typename U>
	requires se::malge::IsMathType<U>
	const se::malge::Vector<T, N> &Vector<T, N>::operator/=(U scalar) {
		SE_MALGE_ASSERT(scalar != 0, "Can't divide vector by 0");
		return *this *= (1.f / scalar);
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	T &Vector<T, N>::operator[](se::malge::Uint8 i) {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid vector index");
		return reinterpret_cast<T*> (this)[i];
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	const T &Vector<T, N>::operator[](se::malge::Uint8 i) const {
		SE_MALGE_ASSERT(i >= 0 && i < N, "i is not a valid vector index");
		return reinterpret_cast<const T*> (this)[i];
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> operator+(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs += rhs;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> operator-(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs -= rhs;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> operator*(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs *= rhs;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsMathType<U>
	se::malge::Vector<T, N> operator*(se::malge::Vector<T, N> lhs, U scalar) {
		return lhs *= scalar;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsMathType<U>
	se::malge::Vector<T, N> operator*(U scalar, se::malge::Vector<T, N> lhs) {
		return lhs *= scalar;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsMathType<U>
	se::malge::Vector<T, N> operator/(se::malge::Vector<T, N> lhs, U scalar) {
		return lhs /= scalar;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	T dot(const se::malge::Vector<T, N> &lhs, const se::malge::Vector<U, N> &rhs) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load<T> (reinterpret_cast<const T*> (&lhs))};
				auto b {se::malge::simd::load<T> (reinterpret_cast<const T*> (&rhs))};
				a = se::malge::simd::dot<T> (a, b);
				return se::malge::simd::convertSingleLaneToScalar<T> (a);
			}

			else {
		#endif

		T result {0};
		for (se::malge::Uint8 i {0}; i < N; ++i)
			result += reinterpret_cast<const T*> (&lhs)[i] * static_cast<T> (reinterpret_cast<const U*> (&lhs)[i]);
		return result;

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif
	}



	template <typename T, typename U>
	requires se::malge::IsValidVector<T, 3> && se::malge::IsValidVector<U, 3>
	se::malge::Vector<T, 3> cross(const se::malge::Vector<T, 3> &lhs, const se::malge::Vector<U, 3> &rhs) {
		se::malge::Vector<T, 3> result {};

		#ifdef SE_MALGE_VECTORIZE
			if constexpr (se::malge::simd::IsValidSIMD<T> && std::is_same_v<T, U>) {
				auto a {se::malge::simd::load(reinterpret_cast<const T*> (&lhs))};
				auto b {se::malge::simd::load(reinterpret_cast<const T*> (&rhs))};
				auto c {se::malge::simd::cross<T> (a, b)};
				se::malge::simd::store(c, reinterpret_cast<T*> (&result));
			}

			else {
		#endif

		result.x = lhs.y * static_cast<U> (rhs.z) - lhs.z * static_cast<U> (rhs.y);
		result.y = lhs.z * static_cast<U> (rhs.x) - lhs.x * static_cast<U> (rhs.z);
		result.z = lhs.x * static_cast<U> (rhs.y) - lhs.y * static_cast<U> (rhs.x);

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif


		return result;
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	T length2(const se::malge::Vector<T, N> &vector) {
		return se::malge::dot(vector, vector);
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	T length(const se::malge::Vector<T, N> &vector) {
		#ifdef SE_MALGE_VECTORIZE
			if constexpr (std::is_same_v<T, se::malge::Float32>) {
				auto a {se::malge::simd::load<T> (reinterpret_cast<const T*> (&vector))};
				a = se::malge::simd::dot<T> (a, a);
				a = se::malge::simd::ssqrt<T> (a);
				return se::malge::simd::convertSingleLaneToScalar<T> (a);
			}

			else {
		#endif

			return sqrt(se::malge::length2(vector));

		#ifdef SE_MALGE_VECTORIZE
			}
		#endif
	}



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	std::ostream &operator<<(std::ostream &stream, const se::malge::Vector<T, N> &vector) {
		stream << "(";
		if constexpr (N == 2) {
			stream << +vector.x << ", " << +vector.y;
		}

		else if constexpr (N == 3) {
			stream << +vector.x << ", " << +vector.y << ", " << +vector.z;
		}

		else {
			stream << +vector.x << ", " << +vector.y << ", " << +vector.z << ", " << +vector.w;
		}

		stream << ")";
		return stream;
	}



} // namespace se::malge
