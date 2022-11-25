#pragma once

// TODO move in another library

namespace lc
{
	// TODO sposta!!!
	/*template <typename... Args>
	struct OverloadSelector
	{
		template <typename R>
		static consteval auto get(R(*fcn)(Args...)) { return fcn; }

		template <class C, typename R>
		static consteval auto get(R(C::* fcn)(Args...)) { return fcn; }
	};

	template <class C, typename... Args>
	struct ClassOverloadSelector
	{
		template <typename R>
		static consteval auto get(R(C::* fcn)(Args...)) { return fcn; }
	};*/

	template <typename... Args>
	struct NonConstMemberOverload
	{
		template <typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)) const noexcept -> decltype(ptr)
		{
			return ptr;
		}

		template <typename R, typename T>
		static constexpr auto of(R(T::* ptr)(Args...)) noexcept -> decltype(ptr)
		{
			return ptr;
		}
	};

	template <typename... Args>
	struct ConstMemberOverload
	{
		template <typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const) const noexcept -> decltype(ptr)
		{
			return ptr;
		}

		template <typename R, typename T>
		static constexpr auto of(R(T::* ptr)(Args...) const) noexcept -> decltype(ptr)
		{
			return ptr;
		}
	};

	template <typename... Args>
	struct NonMemberOverload
	{
		template <typename R>
		constexpr auto operator()(R(*ptr)(Args...)) const noexcept -> decltype(ptr)
		{
			return ptr;
		}

		template <typename R>
		static constexpr auto of(R(*ptr)(Args...)) noexcept -> decltype(ptr)
		{
			return ptr;
		}
	};

	template <typename... Args>
	struct Overload : ConstMemberOverload<Args...>, NonConstMemberOverload<Args...>, NonMemberOverload<Args...>
	{
		using ConstMemberOverload<Args...>::of;
		using ConstMemberOverload<Args...>::operator();
		using NonConstMemberOverload<Args...>::of;
		using NonConstMemberOverload<Args...>::operator();
		using NonMemberOverload<Args...>::of;
		using NonMemberOverload<Args...>::operator();
	};

	template <typename... Args> constexpr inline Overload<Args...> overload = {};
	template <typename... Args> constexpr inline ConstMemberOverload<Args...> constMemberOverload = {};
	template <typename... Args> constexpr inline NonConstMemberOverload<Args...> nonConstMemberOverload = {};
	template <typename... Args> constexpr inline NonMemberOverload<Args...> nonMemberOverload = {};
}