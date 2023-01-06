#pragma once

#include "./vectorial.hpp"

namespace lc
{
	template <int, class>
	class Differential;
}

namespace lc::math::concepts
{
	// https://stackoverflow.com/questions/53684991/why-cant-concept-refinement-use-the-terse-syntax

	////////////////////////////////////////////////////////////////
	/*template <class T>
	concept FieldC =
		//std::movable<std::remove_cvref_t<T>> &&
		//std::copyable<std::remove_cvref_t<T>> && // non funziona?!?!?
		HasInternalAddition<std::remove_cvref_t<T>> &&
		HasInternalAddition<std::remove_cvref_t<T>> &&
		Multiplicable<std::remove_cvref_t<T>> &&
		Dividible<std::remove_cvref_t<T>>;*/
	template <class T>
	concept FieldC =
		VectorialType<T> &&
		MultipliesTo<T, T, T> &&
		DividesTo<T, T, T> &&
		HasDotNullElementType<T> &&
		//std::convertible_to<T, double> &&
		std::convertible_to<double, T>
		;// non è ancora sufficiente

	namespace __details__
	{
		template <class T>
		constexpr bool markedAsScalar =
			std::floating_point<T> ||// float and double
			std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, void>;// other common types
			// ...

		template <int N, class T>
		constexpr bool markedAsScalar<Differential<N, T>> = true;
	}


	// poi quando usero' i moduli questo sistema non va piu' bene! trova un altro sistema
#define LC_MARK_AS_SCALAR(T) namespace lc::math::concepts::__details__ { template <> concept markedAsScalar<T> = true;\
	static_assert(FieldC<T>, "\nCannot mark as a scalar: the type \"" LC_STRINGIZE(T) "\" is not a (lc::math::)FieldC, use LC_MARK_AS_SCALAR_FORCED(" LC_STRINGIZE(T) ") to ignore this error"); }
#define LC_MARK_AS_SCALAR_FORCED(T) namespace lc::math::concepts::__details__ { template <> concept markedAsScalar<T> = true; }

	template <class T>
	concept ScalarType = LC_CONCEPTS_INTELLISENSE_VALUE_DISABLED(/*FieldC<T> && */__details__::markedAsScalar<T>, true);
}

