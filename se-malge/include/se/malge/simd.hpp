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
	void store(SIMD_TYPE(T) a, T data[4]);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) add(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) sub(SIMD_TYPE(T) a, SIMD_TYPE(T) b);

	template <typename T>
	requires se::malge::simd::IsValidSIMD<T>
	SIMD_TYPE(T) mul(SIMD_TYPE(T) a, SIMD_TYPE(T) b);



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
		inline void store<se::malge::Int32> (SIMD_TYPE(se::malge::Int32) a, se::malge::Int32 data[4]) {
			_mm_store_si128((__m128i*)data, a);
		}

		template <>
		inline void store<se::malge::Float> (SIMD_TYPE(se::malge::Float) a, se::malge::Float data[4]) {
			_mm_store_ps(data, a);
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
	#endif



	#undef SIMD_TYPE

	
} // namespace se::malge::simd
