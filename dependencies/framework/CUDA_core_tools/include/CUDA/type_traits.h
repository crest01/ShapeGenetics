


#ifndef INCLUDED_CUDA_TYPE_TRAITS
#define INCLUDED_CUDA_TYPE_TRAITS

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>


namespace CU
{
	template <typename T>
	struct alignment_of;
	
	template <>
	struct alignment_of<std::int8_t> : std::integral_constant<std::size_t, 1U> {};
	
	template <>
	struct alignment_of<std::uint8_t> : alignment_of<std::int8_t> {};
	
	template <>
	struct alignment_of<std::int16_t> : std::integral_constant<std::size_t, 2U> {};
	
	template <>
	struct alignment_of<std::uint16_t> : alignment_of<int16_t> {};
	
	template <>
	struct alignment_of<std::int32_t> : std::integral_constant<std::size_t, 4U> {};
	
	template <>
	struct alignment_of<std::uint32_t> : alignment_of<std::int32_t> {};
	
	template <>
	struct alignment_of<std::int64_t> : std::integral_constant<std::size_t, 8U> {};
	
	template <>
	struct alignment_of<std::uint64_t> : alignment_of<std::int64_t> {};
	
	template <>
	struct alignment_of<float> : std::integral_constant<std::size_t, 4U> {};
	
	template <>
	struct alignment_of<double> : std::integral_constant<std::size_t, 8U> {};
	
	template <typename T>
	struct alignment_of<T&> : alignment_of<T> {};
	
	template <typename T>
	struct alignment_of<T*>;
	
	template <typename T>
	struct alignment_of<T[]> : alignment_of<T> {};
	
	template <typename T, std::size_t N>
	struct alignment_of<T[N]> : alignment_of<T> {};
	
	
	template <typename T>
	constexpr auto alignment_of_v = alignment_of<T>::value;
}

#endif  // INCLUDED_CUDA_TYPE_TRAITS
