#pragma once

#include <type_traits>

#include <LC/math/concepts/concepts.hpp>

namespace lc::priv
{
	class MyVectorSignature {};

	template <class T>
	concept CanBeDereferencedOnce = requires (T v) { { *v }; };

	/*template <class From, class To>
	consteval bool _canBeDereferencedInto(void)
	{
		if consteval ()
		{

		}
		return true;
	}*/

	template <class T>
	concept CanBeDereferencedInto = requires (T v) { { *v }; };

	template <class _ValueType, class _Field, bool _isStaticSize>
	class MyVectorBasicProperties : public MyVectorSignature
	{
	public:

		// tells whether the vector has a compile time fixed size or not
		static constexpr inline bool isStaticSize = _isStaticSize;

		// The internal vale type of the components: it could be double or float, if you want as
		// well as double* for example
		using ValueType = _ValueType;

		// The "field" of the value type for example double, float, int.
		// Note that this is not the mathematical definition of "field", it could
		// be "string", "char" or whathever you want
		using Field = _Field;
	};

template <class T>
concept MyVectorType = std::is_base_of_v<MyVectorSignature, T>; //std::derived_from<T, MyVectorSignature>;


#define v_dot_x(v, x) v.x
#define _CONCEPT_HAS_COMPONENT_CONVERTIBLE_TO(component)        \
	template <class V, class TypeConversion = ConvertibleToAnyType>          \
	concept Has_ ## component ## _component = requires (V v)    \
	{                                                           \
		{ v_dot_x(v, component) } -> std::convertible_to<TypeConversion>;    \
	}

	namespace __details__
	{
		// TODO_MINOR sposta
		class ConvertibleToAnyType
		{
		public:
			ConvertibleToAnyType() {}

			template <class T>
			ConvertibleToAnyType(T) {}
		};

		_CONCEPT_HAS_COMPONENT_CONVERTIBLE_TO(x);
		_CONCEPT_HAS_COMPONENT_CONVERTIBLE_TO(y);
		_CONCEPT_HAS_COMPONENT_CONVERTIBLE_TO(z);
	}

	template <class Vec, class ConversionTo = __details__::ConvertibleToAnyType>
	concept Vector2ContainerOfConvertibleTo =
		__details__::Has_x_component<Vec, ConversionTo> &&
		__details__::Has_y_component<Vec, ConversionTo> &&
		!__details__::Has_z_component<Vec, ConversionTo>;

	template <class Vec, class ConversionTo = __details__::ConvertibleToAnyType>
	concept Vector3ContainerOfConvertibleTo =
		__details__::Has_x_component<Vec, ConversionTo> &&
		__details__::Has_y_component<Vec, ConversionTo> &&
		__details__::Has_z_component<Vec, ConversionTo>;

	template <class Vec>
	concept isAnyVector =
		__details__::Has_x_component<Vec> &&
		__details__::Has_y_component<Vec>;

#undef v_dot_x

}

namespace lc
{
	template <class Field, class Type>// TODO sposta
	inline constexpr auto& deRef(Type& valueOrDereferentiable)
	{
		if constexpr (std::is_same_v<std::remove_cvref_t<Type>, Field>)
			return valueOrDereferentiable;
		else
			return deRef<Field>(*valueOrDereferentiable);
	}
	template <class Field, class Type>// TODO sposta
	inline constexpr auto& deRef_ref(Type& valueOrDereferentiable) { return deRef<Field>(valueOrDereferentiable); }
	template <class Field, class Type>
	inline constexpr auto& deRef(const Type& valueOrDereferentiable)
	{
		if constexpr (std::is_same_v<std::remove_cvref_t<Type>, Field>)
			return valueOrDereferentiable;
		else
			return deRef<Field>(*valueOrDereferentiable);
	}
}
