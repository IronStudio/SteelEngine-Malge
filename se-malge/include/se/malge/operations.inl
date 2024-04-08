#include "se/malge/operations.hpp"



namespace se::malge
{
	template <typename T, typename U, se::malge::Uint8 N>
	requires se::malge::IsValidMatrix<T, N> && se::malge::IsValidVector<T, N> && se::malge::IsValidVector<U, N>
	se::malge::Vector<T, N> operator*(const se::malge::Matrix<T, N> &matrix, const se::malge::Vector<U, N> &vector) {

	}



} // namespace se::malge
