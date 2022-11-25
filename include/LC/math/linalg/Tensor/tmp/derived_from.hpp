#pragma once


namespace lc
{
	/*// see https://stackoverflow.com/questions/65620758/c-check-if-class-derived-from-template-base-class-which-has-integral-template
	// in particular https://stackoverflow.com/questions/22592419/checking-if-a-class-inherits-from-any-template-instantiation-of-a-template#answer-22592618

	MODEL:

	template<template<class> class T, class U>
	struct isDerivedFrom
	{
	private:
		template<class V>
		static decltype(static_cast<const T<V>&>(std::declval<U>()), std::true_type{}) test(const T<V>&);

		static std::false_type test(...);
	public:
		static constexpr bool value = decltype(isDerivedFrom::test(std::declval<U>()))::value;
	};*/
}

#define DERIVED_FROM