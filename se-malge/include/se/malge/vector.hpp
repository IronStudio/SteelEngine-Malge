#pragma once

#include <iostream>

#include "se/malge/types.hpp"



namespace se::malge
{
	template <typename T, se::malge::Uint8 N>
	concept IsValidVector = se::malge::IsMathType<T> && (N >= 2 && N <= 4);



	#ifdef SE_MALGE_VECTORIZE

		template <typename T>
		requires se::malge::IsMathType<T>
		struct VectorAlignment {
			static constexpr se::malge::Uint64 alignment {16};
		};

		#ifdef SE_MALGE_AVX
			template <>
			struct VectorAlignment<se::malge::Float64> {
				static constexpr se::malge::Uint64 alignment {32};
			};
		#endif

		#define SE_MALGE_VECTOR_ALIGNMENT(T) alignas(se::malge::VectorAlignment<T>::alignment)

	#else
		#define SE_MALGE_VECTOR_ALIGNMENT(T)
	#endif



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	struct VectorComponents;



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	class SE_MALGE_VECTOR_ALIGNMENT(T) Vector : public se::malge::VectorComponents<T, N> {
		public:
			Vector();
			template <typename Fill>
			requires se::malge::IsMathType<Fill>
			Vector(Fill fill);
			template <typename ...Args>
			requires (se::malge::IsMathType<Args> && ...) && (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
			Vector(Args ...args);
			template <typename U, se::malge::Uint8 M, typename ...Args>
			requires se::malge::IsValidVector<U, M> && (se::malge::IsMathType<Args> && ...) && (N >= M) && (sizeof...(Args) <= N - M)
			Vector(const se::malge::Vector<U, M> &vector, Args ...args);

			template <typename U>
			requires se::malge::IsValidVector<U, N>
			const se::malge::Vector<T, N> &operator=(const se::malge::Vector<U, N> &vector);


			inline T &operator[](se::malge::Uint8 i);
			inline const T &operator[](se::malge::Uint8 i) const;
	};



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidVector<T, N>
	std::ostream &operator<<(std::ostream &stream, const se::malge::Vector<T, N> &vector);



	template <typename T>
	requires se::malge::IsValidVector<T, 2>
	struct VectorComponents<T, 2> {
		union {
			T x, u;
		};

		union {
			T y, v;
		};


		private:
			T _padding[2];
	};



	template <typename T>
	requires se::malge::IsValidVector<T, 3>
	struct VectorComponents<T, 3> {
		union {
			T x, r;
		};

		union {
			T y, g;
		};

		union {
			T z, b;
		};


		private:
			T _padding[1];
	};



	template <typename T>
	requires se::malge::IsValidVector<T, 4>
	struct VectorComponents<T, 4> {
		union {
			T x, r;
		};

		union {
			T y, g;
		};

		union {
			T z, b;
		};

		union {
			T w, a;
		};
	};



	template <typename T>
	requires se::malge::IsValidVector<T, 2>
	using Vec2    = se::malge::Vector<T, 2>;
	using Vec2i8  = se::malge::Vec2<se::malge::Int8>;
	using Vec2u8  = se::malge::Vec2<se::malge::Uint8>;
	using Vec2i16 = se::malge::Vec2<se::malge::Int16>;
	using Vec2u16 = se::malge::Vec2<se::malge::Uint16>;
	using Vec2i32 = se::malge::Vec2<se::malge::Int32>;
	using Vec2u32 = se::malge::Vec2<se::malge::Uint32>;
	using Vec2i64 = se::malge::Vec2<se::malge::Int64>;
	using Vec2u64 = se::malge::Vec2<se::malge::Uint64>;
	using Vec2i   = se::malge::Vec2<se::malge::Int>;
	using Vec2u   = se::malge::Vec2<se::malge::Uint>;
	using Vec2f32 = se::malge::Vec2<se::malge::Float32>;
	using Vec2f64 = se::malge::Vec2<se::malge::Float64>;
	using Vec2f   = se::malge::Vec2<se::malge::Float>;

	template <typename T>
	requires se::malge::IsValidVector<T, 3>
	using Vec3    = se::malge::Vector<T, 3>;
	using Vec3i8  = se::malge::Vec3<se::malge::Int8>;
	using Vec3u8  = se::malge::Vec3<se::malge::Uint8>;
	using Vec3i16 = se::malge::Vec3<se::malge::Int16>;
	using Vec3u16 = se::malge::Vec3<se::malge::Uint16>;
	using Vec3i32 = se::malge::Vec3<se::malge::Int32>;
	using Vec3u32 = se::malge::Vec3<se::malge::Uint32>;
	using Vec3i64 = se::malge::Vec3<se::malge::Int64>;
	using Vec3u64 = se::malge::Vec3<se::malge::Uint64>;
	using Vec3i   = se::malge::Vec3<se::malge::Int>;
	using Vec3u   = se::malge::Vec3<se::malge::Uint>;
	using Vec3f32 = se::malge::Vec3<se::malge::Float32>;
	using Vec3f64 = se::malge::Vec3<se::malge::Float64>;
	using Vec3f   = se::malge::Vec3<se::malge::Float>;

	template <typename T>
	requires se::malge::IsValidVector<T, 4>
	using Vec4    = se::malge::Vector<T, 4>;
	using Vec4i8  = se::malge::Vec4<se::malge::Int8>;
	using Vec4u8  = se::malge::Vec4<se::malge::Uint8>;
	using Vec4i16 = se::malge::Vec4<se::malge::Int16>;
	using Vec4u16 = se::malge::Vec4<se::malge::Uint16>;
	using Vec4i32 = se::malge::Vec4<se::malge::Int32>;
	using Vec4u32 = se::malge::Vec4<se::malge::Uint32>;
	using Vec4i64 = se::malge::Vec4<se::malge::Int64>;
	using Vec4u64 = se::malge::Vec4<se::malge::Uint64>;
	using Vec4i   = se::malge::Vec4<se::malge::Int>;
	using Vec4u   = se::malge::Vec4<se::malge::Uint>;
	using Vec4f32 = se::malge::Vec4<se::malge::Float32>;
	using Vec4f64 = se::malge::Vec4<se::malge::Float64>;
	using Vec4f   = se::malge::Vec4<se::malge::Float>;



} // namespace se::malge



#include "se/malge/vector.inl"