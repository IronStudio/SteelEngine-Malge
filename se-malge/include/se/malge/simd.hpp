#pragma once

#ifndef SE_MALGE_VECTORIZE
	#error Cant use SIMD instructions if vectorization is disabled
#endif

#ifdef SE_MALGE_ARCHITECTURE_X86
	#ifndef SE_MALGE_AVX
		#include <nmmintrin.h>
	#else
		#include <immintrin.h>
	#endif
#endif



#include <type_traits>

#include "se/malge/types.hpp"



namespace se::malge::simd
{
	#ifdef SE_MALGE_ARCHITECTURE_X86
		template <typename T>
		concept IsValidSIMD = std::is_same_v<T, se::malge::Int32> ||
			std::is_same_v<T, se::malge::Float32>;
	#endif


	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	struct Register;


	#ifdef SE_MALGE_ARCHITECTURE_X86
		template <>
		struct Register<se::malge::Int32> {
			using Type = __m128i;
		};

		template <>
		struct Register<se::malge::Float32> {
			using Type = __m128;
		};
	#endif


	#ifdef SIMD_TYPE
		#undef SIMD_TYPE
	#endif
	#define SIMD_TYPE(T) typename se::malge::simd::Register<T>::Type


	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) load(const T data[4]);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) setZero();

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) loadScalar(const T *scalar);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	void store(SIMD_TYPE(T) a, T data[4]);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	T convertSingleLaneToScalar(SIMD_TYPE(T) a);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) add(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) sub(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) mul(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) dot(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) cross(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires std::is_same_v<T, se::malge::Float32>
	SIMD_TYPE(T) ssqrt(SIMD_TYPE(T) a);



	#ifdef SE_MALGE_ARCHITECTURE_X86
		template <>
		inline SIMD_TYPE(se::malge::Int32) load<se::malge::Int32> (const se::malge::Int32 data[4]) {
			return _mm_load_si128((__m128i*)data);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) load<se::malge::Float> (const se::malge::Float data[4]) {
			return _mm_load_ps(data);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) setZero<se::malge::Int32> () {
			return _mm_setzero_si128();
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) setZero<se::malge::Float> () {
			return _mm_setzero_ps();
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) loadScalar<se::malge::Int32> (const se::malge::Int32 *scalar) {
			return _mm_cvtps_epi32(_mm_load_ps1((const float*)scalar));
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) loadScalar<se::malge::Float> (const se::malge::Float *scalar) {
			return _mm_load_ps1(scalar);
		}



		template <>
		inline void store<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, se::malge::Int32 data[4]) {
			_mm_store_si128((__m128i*)data, a);
		}

		template <>
		inline void store<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, se::malge::Float data[4]) {
			_mm_store_ps(data, a);
		}



		template <>
		se::malge::Int32 convertSingleLaneToScalar<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a) {
			return _mm_cvtsi128_si32(a);
		}

		template <>
		se::malge::Float convertSingleLaneToScalar<se::malge::Float> (SIMD_TYPE(se::malge::Float) a) {
			return _mm_cvtss_f32(a);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) add<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, SIMD_TYPE(se::malge::Int32) b) {
			return _mm_add_epi32(a, b);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) add<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, SIMD_TYPE(se::malge::Float) b) {
			return _mm_add_ps(a, b);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) sub<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, SIMD_TYPE(se::malge::Int32) b) {
			return _mm_sub_epi32(a, b);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) sub<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, SIMD_TYPE(se::malge::Float) b) {
			return _mm_sub_ps(a, b);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) mul<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, SIMD_TYPE(se::malge::Int32) b) {
			return _mm_mul_epi32(a, b);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) mul<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, SIMD_TYPE(se::malge::Float) b) {
			return _mm_mul_ps(a, b);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) dot<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, SIMD_TYPE(se::malge::Int32) b) {
			__m128i c {_mm_mul_epi32(a, b)};
			return _mm_hadd_epi32(c, c);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) dot<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, SIMD_TYPE(se::malge::Float) b) {
			__m128 c {_mm_mul_ps(a, b)};
			__m128 shuffle {_mm_movehdup_ps(c)};
			c = _mm_add_ps(c, shuffle);
			shuffle = _mm_movehl_ps(shuffle, c);
			return _mm_add_ss(c, shuffle);
		}



		template <>
		inline SIMD_TYPE(se::malge::Int32) cross<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, SIMD_TYPE(se::malge::Int32) b) {
			__m128i firstA {_mm_shuffle_epi32(a, _MM_SHUFFLE(3, 0, 2, 1))};
			__m128i secondA {_mm_shuffle_epi32(a, _MM_SHUFFLE(3, 1, 0, 2))};
			__m128i firstB {_mm_shuffle_epi32(b, _MM_SHUFFLE(3, 1, 0, 2))};
			__m128i secondB {_mm_shuffle_epi32(b, _MM_SHUFFLE(3, 0, 2, 1))};
			firstA = _mm_mul_epi32(firstA, firstB);
			secondA = _mm_mul_epi32(secondA, secondB);
			return _mm_sub_epi32(firstA, secondA);
		}

		template <>
		inline SIMD_TYPE(se::malge::Float) cross<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, SIMD_TYPE(se::malge::Float) b) {
			__m128 firstA {_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1))};
			__m128 secondA {_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))};
			__m128 firstB {_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))};
			__m128 secondB {_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))};
			firstA = _mm_mul_ps(firstA, firstB);
			secondA = _mm_mul_ps(secondA, secondB);
			return _mm_sub_ps(firstA, secondA);
		}



		template <>
		SIMD_TYPE(se::malge::Float) ssqrt<se::malge::Float> (SIMD_TYPE(se::malge::Float) a) {
			return _mm_sqrt_ss(a);
		}
	#endif



	#undef SIMD_TYPE

	
} // namespace se::malge::simd
