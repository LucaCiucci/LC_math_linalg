#pragma once

#include "numbers.hpp"


namespace lc::math::concepts
{
	////////////////////////////////////////////////////////////////
	/// \concept lc::math::concepts::VectorialType
	///
	/// \ingroup math_concepts
	///
	/// \brief Vectorial quatity
	///
	/// This concept represents a class wich has some vetorial
	/// properties: sum, subctraction, scalar multiplication,
	/// null element (required to be T()), etc.\n
	///
	/// \see
	////////////////////////////////////////////////////////////////
#define _TMP_EXPRESSION                                                    \
	((std::floating_point<T> ||	std::integral<T>) && !std::same_as<T, void>) ||\
	(SumClosed<T>&& HasOpposite<T> && ScalableBy<T, double>&& DivisibleBy<T, double>&& HasOppositeType<T, T>&& HasSumNullElementType<T>)

	// TODO_MINOR mancano:
	// std::movable<std::remove_cvref_t<T>>&& \
	// std::copyable<std::remove_cvref_t<T>>&&

	template <class T>
	concept VectorialType = _TMP_EXPRESSION;
#undef _TMP_EXPRESSION
}




