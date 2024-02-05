#include "se/malge/vector.hpp"

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
		reinterpret_cast<T*> (this)[2] = static_cast<T> (fill);
		reinterpret_cast<T*> (this)[3] = static_cast<T> (fill);
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
	se::malge::Vector<T, N> &operator+(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs += rhs;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> &operator-(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs -= rhs;
	}



	template <typename T, se::malge::Uint8 N, typename U>
	requires se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> &operator*(se::malge::Vector<T, N> lhs, const se::malge::Vector<U, N> &rhs) {
		return lhs *= rhs;
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
