#pragma once

#include <cinttypes>
#include <type_traits>



namespace se::malge
{
	using Int8  = int8_t;
	using Uint8 = uint8_t;
	using Int16  = int16_t;
	using Uint16 = uint16_t;
	using Int32  = int32_t;
	using Uint32 = uint32_t;
	using Int64  = int64_t;
	using Uint64 = uint64_t;

	using Int = se::malge::Int32;
	using Uint = se::malge::Uint32;


	using Float32 = float;
	using Float64 = double;
	using Float = se::malge::Float32;


	template <typename T>
	concept IsMathType =
		std::is_same_v<T, se::malge::Int8> ||
		std::is_same_v<T, se::malge::Uint8> ||
		std::is_same_v<T, se::malge::Int16> ||
		std::is_same_v<T, se::malge::Uint16> ||
		std::is_same_v<T, se::malge::Int32> ||
		std::is_same_v<T, se::malge::Uint32> ||
		std::is_same_v<T, se::malge::Int64> ||
		std::is_same_v<T, se::malge::Uint64> ||
		std::is_same_v<T, se::malge::Int> ||
		std::is_same_v<T, se::malge::Uint> ||
		std::is_same_v<T, se::malge::Float32> ||
		std::is_same_v<T, se::malge::Float64> ||
		std::is_same_v<T, se::malge::Float>;


} // namespace se::malge
