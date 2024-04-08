#pragma once

#include "se/malge/matrix.hpp"
#include "se/malge/requires.hpp"
#include "se/malge/types.hpp"
#include "se/malge/vector.hpp"



namespace se::malge
{
	template <typename T, typename U, se::malge::Uint8 N>
	SE_MALGE_REQUIRES(se::malge::IsValidMatrix<T, N> && se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>)
	se::malge::Vector<T, N> operator*(const se::malge::Matrix<T, N> &matrix, const se::malge::Vector<U, N> &vector);
	
} // namespace se::malge



#include "se/malge/operations.inl"