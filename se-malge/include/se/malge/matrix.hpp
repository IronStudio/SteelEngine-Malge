#pragma once

#include <ostream>

#include "se/malge/types.hpp"



namespace se::malge
{
	template <typename T, se::malge::Uint8 N>
	concept IsValidMatrix = se::malge::IsMathType<T> && (N >= 2 && N <= 4);



	#ifdef SE_MALGE_VECTORIZE

		template <typename T>
		requires se::malge::IsMathType<T>
		struct MatrixAlignment {
			static constexpr se::malge::Uint64 alignment {16};
		};

		#ifdef SE_MALGE_AVX
			template <>
			struct MatrixAlignment<se::malge::Float64> {
				static constexpr se::malge::Uint64 alignment {32};
			};
		#endif

		#define SE_MALGE_MATRIX_ALIGNMENT(T) alignas(se::malge::MatrixAlignment<T>::alignment)

	#else
		#define SE_MALGE_MATRIX_ALIGNMENT(T)
	#endif



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	class MatrixRow {
		public:
			T &operator[] (se::malge::Uint8 column);
			const T &operator[] (se::malge::Uint8 column) const;

		private:
			T *__padding[4];
	};




	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	class SE_MALGE_MATRIX_ALIGNMENT(T) Matrix {
		public:
			template <typename ...Args>
			requires (se::malge::IsMathType<Args> && ...) && (sizeof...(Args) <= N*N)
			Matrix(Args ...args);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			Matrix(const se::malge::Matrix<U, N> &matrix);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			Matrix(se::malge::Matrix<U, N> &&matrix) noexcept;

			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator=(const se::malge::Matrix<U, N> &matrix);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator=(se::malge::Matrix<U, N> &&matrix) noexcept;


			se::malge::MatrixRow<T, N> operator[] (se::malge::Uint8 row);
			se::malge::MatrixRow<T, N> operator[] (se::malge::Uint8 row) const;

			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator+=(const se::malge::Matrix<U, N> &matrix);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator-=(const se::malge::Matrix<U, N> &matrix);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator*=(const se::malge::Matrix<U, N> &matrix);


		private:
			T __padding[16];
	};




	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	se::malge::Matrix<T, N> operator+(se::malge::Matrix<T, N> lhs, const se::malge::Matrix<T, N> &rhs);
	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	se::malge::Matrix<T, N> operator-(se::malge::Matrix<T, N> lhs, const se::malge::Matrix<T, N> &rhs);
	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	se::malge::Matrix<T, N> operator*(const se::malge::Matrix<T, N> &lhs, se::malge::Matrix<T, N> rhs);



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	std::ostream &operator<<(std::ostream &stream, const se::malge::Matrix<T, N> &matrix);



	template <typename T>
	requires se::malge::IsValidMatrix<T, 2>
	using Mat2    = se::malge::Matrix<T, 2>;
	using Mat2i8  = se::malge::Mat2<se::malge::Int8>;
	using Mat2u8  = se::malge::Mat2<se::malge::Uint8>;
	using Mat2i16 = se::malge::Mat2<se::malge::Int16>;
	using Mat2u16 = se::malge::Mat2<se::malge::Uint16>;
	using Mat2i32 = se::malge::Mat2<se::malge::Int32>;
	using Mat2u32 = se::malge::Mat2<se::malge::Uint32>;
	using Mat2i64 = se::malge::Mat2<se::malge::Int64>;
	using Mat2u64 = se::malge::Mat2<se::malge::Uint64>;
	using Mat2i   = se::malge::Mat2<se::malge::Int>;
	using Mat2u   = se::malge::Mat2<se::malge::Uint>;
	using Mat2f32 = se::malge::Mat2<se::malge::Float32>;
	using Mat2f64 = se::malge::Mat2<se::malge::Float64>;
	using Mat2f   = se::malge::Mat2<se::malge::Float>;

	template <typename T>
	requires se::malge::IsValidMatrix<T, 3>
	using Mat3    = se::malge::Matrix<T, 3>;
	using Mat3i8  = se::malge::Mat3<se::malge::Int8>;
	using Mat3u8  = se::malge::Mat3<se::malge::Uint8>;
	using Mat3i16 = se::malge::Mat3<se::malge::Int16>;
	using Mat3u16 = se::malge::Mat3<se::malge::Uint16>;
	using Mat3i32 = se::malge::Mat3<se::malge::Int32>;
	using Mat3u32 = se::malge::Mat3<se::malge::Uint32>;
	using Mat3i64 = se::malge::Mat3<se::malge::Int64>;
	using Mat3u64 = se::malge::Mat3<se::malge::Uint64>;
	using Mat3i   = se::malge::Mat3<se::malge::Int>;
	using Mat3u   = se::malge::Mat3<se::malge::Uint>;
	using Mat3f32 = se::malge::Mat3<se::malge::Float32>;
	using Mat3f64 = se::malge::Mat3<se::malge::Float64>;
	using Mat3f   = se::malge::Mat3<se::malge::Float>;

	template <typename T>
	requires se::malge::IsValidMatrix<T, 4>
	using Mat4    = se::malge::Matrix<T, 4>;
	using Mat4i8  = se::malge::Mat4<se::malge::Int8>;
	using Mat4u8  = se::malge::Mat4<se::malge::Uint8>;
	using Mat4i16 = se::malge::Mat4<se::malge::Int16>;
	using Mat4u16 = se::malge::Mat4<se::malge::Uint16>;
	using Mat4i32 = se::malge::Mat4<se::malge::Int32>;
	using Mat4u32 = se::malge::Mat4<se::malge::Uint32>;
	using Mat4i64 = se::malge::Mat4<se::malge::Int64>;
	using Mat4u64 = se::malge::Mat4<se::malge::Uint64>;
	using Mat4i   = se::malge::Mat4<se::malge::Int>;
	using Mat4u   = se::malge::Mat4<se::malge::Uint>;
	using Mat4f32 = se::malge::Mat4<se::malge::Float32>;
	using Mat4f64 = se::malge::Mat4<se::malge::Float64>;
	using Mat4f   = se::malge::Mat4<se::malge::Float>;


} // namespace se::malge



#include "se/malge/matrix.inl"