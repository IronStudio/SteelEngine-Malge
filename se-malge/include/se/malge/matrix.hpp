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
	struct MatrixColumn {
		public:
			inline T &operator[] (se::malge::Uint8 i);
			inline const T &operator[] (se::malge::Uint8 i) const;

		private:
			T _padding[4];
	};



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	class SE_MALGE_MATRIX_ALIGNMENT(T) Matrix {
		public:
			template <typename ...Args>
			requires (se::malge::IsMathType<Args> && ...) && (sizeof...(Args) >= 0 && sizeof...(Args) <= N * N)
			Matrix(Args ...args);
			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			Matrix(const se::malge::Matrix<U, N> &matrix);

			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator=(const se::malge::Matrix<U, N> &matrix);

			inline se::malge::MatrixColumn<T, N> &operator[] (se::malge::Uint8 i);
			inline const se::malge::MatrixColumn<T, N> &operator[] (se::malge::Uint8 i) const;


			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator+=(const se::malge::Matrix<U, N> &matrix);

			template <typename U>
			requires se::malge::IsValidMatrix<U, N>
			const se::malge::Matrix<T, N> &operator-=(const se::malge::Matrix<U, N> &matrix);



		private:
			se::malge::MatrixColumn<T, N> m_columns[N];
	};



	template <typename T, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N>
	std::ostream &operator<<(std::ostream &stream, const se::malge::Matrix<T, N> &matrix);



} // namespace se::malge



#include "se/malge/matrix.inl"