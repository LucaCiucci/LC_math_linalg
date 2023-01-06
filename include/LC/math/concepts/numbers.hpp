#pragma once

#include <cmath>

#include <concepts>

namespace lc::math::concepts
{
	////////////////////////////////////////////////////////////////
	// True if the result of types InLeft + InRight is Out
	template <class InLeft, class InRight, class Out>
	concept AddTo = requires (InLeft l, InRight r)
	{
		{ l + r } -> std::same_as<Out>;
	};

	////////////////////////////////////////////////////////////////
	template <class InLeft, class InRight, class Out>
	// True if the result of types InLeft - InRight is Out
	concept SubtractTo = requires (InLeft l, InRight r)
	{
		{ l - r } -> std::same_as<Out>;
	};

	////////////////////////////////////////////////////////////////
	// True if the result of types InLeft * InRight is Out
	template <class InLeft, class InRight, class Out>
	concept MultipliesTo = requires (InLeft l, InRight r)
	{
		{ l* r } -> std::same_as<Out>;
	};

	////////////////////////////////////////////////////////////////
	template <class InLeft, class InRight, class Out>
	// True if the result of types InLeft / InRight is Out
	concept DividesTo = requires (InLeft l, InRight r)
	{
		{ l / r } -> std::same_as<Out>;
	};

	////////////////////////////////////////////////////////////////
	template <class Left, class Right>
	// True if Left + Right = Left and Left += Right returns Left&
	concept IncrementableBy = /*AddTo<Left, Right, Left> &&*/ requires (Left l, Right r)
	{
		{ l += r };//->std::same_as<Left&>;
	};

	////////////////////////////////////////////////////////////////
	template <class Left, class Right>
	// True if Left - Right = Left and Left -= Right returns Left&
	concept DecrementableBy = /*SubtractTo<Left, Right, Left> &&*/ requires (Left l, Right r)
	{
		{ l -= r };//->std::same_as<Left&>;
	};

	////////////////////////////////////////////////////////////////
	template <class Left, class Right>
	// True if Left * Right = Left and Left *= Right returns Left&
	concept ScalableBy = /*MultipliesTo<Left, Right, Left> &&*/ requires (Left l, Right r)
	{
		{ l *= r };//->std::same_as<Left&>;
	};

	////////////////////////////////////////////////////////////////
	// True if Left / Right = Left and Left /= Right returns Left&
	template <class Left, class Right>
	concept DivisibleBy = /*DividesTo<Left, Right, Left> && */requires (Left l, Right r)
	{
		{ l /= r };//->std::same_as<Left&>;
	};

	////////////////////////////////////////////////////////////////
	// True if the Type is closed respect to the addition and subctraction
	template <class T>
	concept SumClosed =
		AddTo<T, T, T> && IncrementableBy<T, T> &&
		SubtractTo<T, T, T> && DecrementableBy<T, T>;

	////////////////////////////////////////////////////////////////
	// Ture if -In = Out
	template <class In, class Out = In>
	concept HasOppositeType = requires (In x)
	{
		{ -x } -> std::same_as<Out>;
	};

	////////////////////////////////////////////////////////////////
	// Ture if -In operator is available
	template <class In>
	concept HasOpposite = requires (In x)
	{
		{ -x };
	};

	namespace __details__
	{
		////////////////////////////////////////////////////////////////
		template <class T>
		static inline constexpr bool test_for_sum_null_element(void)
			requires requires() { {T(0.0)}; {T()}; }
		{
			auto test = [](T x = T()) { return x + T() == x && T() + x == x; };

			T()+ T() == T();

			return true;
				//test() &&
				//test(0.0) &&
				//test(42.0);
		}
	}

	////////////////////////////////////////////////////////////////
	//template <class T, bool TestFunc(void) = __details__::test_for_sum_null_element<T>>
	template <class T>
	concept HasSumNullElementType = SumClosed<T> && requires (T x)// && has "=="
	{
		//{ x == x } -> std::boolean;// su msvc ancora non funziona
		{ x == x } -> std::same_as<bool>;// su msvc ancora non funziona
	//} && TestFunc();
	};// TODO && __details__::test_for_sum_null_element<T>();

	namespace __details__
	{
		////////////////////////////////////////////////////////////////
		template <class T>
		static inline constexpr bool test_for_dot_null_element(void)
			requires requires() { {T(0.0)}; {T()}; }
		{
			auto test = [](T x = T()) { return x * T(1.0) == x && T(1.0) * x == x; };

			return
				test() &&
				test(0.0) &&
				test(1);
				test(42.0);
		}
	}

	////////////////////////////////////////////////////////////////
	template <class T>
	concept HasDotNullElementType = MultipliesTo<T, T, T> && requires (T x)// && has "=="
	{
		//{ x == x } -> std::boolean;// su msvc ancora non funziona
		{ x == x } -> std::same_as<bool>;// su msvc ancora non funziona
	} && __details__::test_for_dot_null_element<T>();

	// ================================================================
	//                          FUNCTIONS
	// ================================================================

	namespace __details__
	{
	}

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the abs function.
	// The function is searched in "lc", "std", and global namespaces
	template <class T>
	concept HasAbs = requires (T x) { { abs(x) }; } || requires (T x) { { std::abs(x) }; };// || requires (T x) { { ::abs(x) }; };

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the sqr function.
	// The function is searched in "lc", "std", and global namespaces
	//template <class T>
	//concept HasSqr = requires (T x) { { sqr(x) }; } || requires (T x) { { std::sqr(x) }; };// || requires (T x) { { ::sqr(x) }; };
	// TODO cmake non lo accetta

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the sqrt function.
	// The function is searched in "lc", "std", and global namespaces
	template <class T>
	concept HasSqrt = requires (T x) { { sqrt(x) }; } || requires (T x) { { std::sqrt(x) }; };// || requires (T x) { { ::sqrt(x) }; };

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the exp function.
	// The function is searched in "lc", "std", and global namespaces
	template <class T>
	concept HasExp = requires (T x) { { exp(x) }; } || requires (T x) { { std::exp(x) }; };// || requires (T x) { { ::exp(x) }; };

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the log function.
	// The function is searched in "lc", "std", and global namespaces
	template <class T>
	concept HasLog = requires (T x) { { log(x) }; } || requires (T x) { { std::log(x) }; };// || requires (T x) { { ::log(x) }; };

	////////////////////////////////////////////////////////////////
	// Checks if the type T can be passed to the pow function.
	// The function is searched in "lc", "std", and global namespaces
	template <class T>
	concept HasPow = requires (T x, T y) { { pow(x, y) }; } || requires (T x, T y) { { std::pow(x, y) }; };// || requires (T x, T y) { { ::pow(x, y) }; };


#undef _LC_TMP_SEARCH_FUNCTION
}

// TODO gli std::same_as sono troppo forti


