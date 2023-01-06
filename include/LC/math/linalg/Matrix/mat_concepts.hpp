#pragma once

#include <LC/config/config.h>
#include <type_traits>

#ifdef LC_ENABLE_CONCEPTS
#include <LC/math/concepts/concepts>
#endif // LC_ENABLE_CONCEPTS

namespace lc::math::geometry::__details__
{
	class MyMatrixSignature {};

#ifdef LC_ENABLE_CONCEPTS
	template <class T>
	concept MyMatrixType = std::is_base_of_v<MyMatrixSignature, T>; //std::derived_from<T, MyVectorSignature>;
#else
	template <class T>
	constexpr bool MyMatrixType = std::is_base_of_v<MyMatrixSignature, T>;
#endif
}
