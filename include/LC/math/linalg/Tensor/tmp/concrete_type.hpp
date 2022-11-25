#pragma once

// for `std::conditional`
#include <type_traits>


namespace lc
{
	// TODO sposta in una libreria diversa


	// used as placeholder to indicate that a type is itself
	class Itself {};
	
	// This alias can be used to get the concrete type of an object, example
	// ```cpp
	// template <class FinalClass = lc::Itself>
	// class SomeDerivableClass {
	// public:
	//     using ConcreteType = lc::GetConcreteType<SomeDerivableClass, FinalClass>
	// 
	//     ConcreteType& operator+=(int) { /*...*/ }
	// };
	template <class ThisClass, class ConcretePointer>
	using GetConcreteType = typename
		std::conditional<
			std::is_same_v<ConcretePointer, Itself>,
			ThisClass,
			ConcretePointer
		>::type;
}