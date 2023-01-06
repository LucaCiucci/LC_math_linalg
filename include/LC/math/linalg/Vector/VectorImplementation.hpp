#pragma once

#include <vector>// ??
#include <type_traits>
#include <exception>
#include <ostream>
#include <string>
#include <sstream>
#include <concepts>

#include <numbers>

#include <cmath>
using std::sqrt;
using std::abs;
using std::pow;
using std::max;
using std::min;

#include <LC/math/concepts/concepts.hpp>
#include <LC/math/math_defines.h>

#include "vector_concepts.hpp"

#include "../tmp_utils.hpp"

namespace lc
{
	constexpr int DynamicSize = -1;

	// TODO sposta
	class Itself {};
	template <class ThisClass, class ConcretePointer>
	using GetConcretePointer = typename std::conditional<std::is_same_v<ConcretePointer, Itself>, ThisClass, ConcretePointer>::type;
//#define VectorImpl<Field, Container, _IsStaticSize, StaticSize> GetConcretePointer<THIS_CLASS, ConcretePointer>

	template <class T>
	inline constexpr bool isVectorial = lc::math::concepts::VectorialType<T>;

	template <class T>
	inline consteval bool isScalar_f(void)
	{

		return lc::math::concepts::ScalarType<T>;
	}
	template <class T>
	constexpr bool isScalar = isScalar_f<T>();

	// ================================================================
	//                    VECTOR IMPLEMENTATION
	// ================================================================

	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize = 0>
	class VectorImpl :
		public Container
	{
		using BASE = Container;
	public:

		// ================================
		//             INFO
		// ================================

		static constexpr bool isStaticSize(void);

		static constexpr bool isTheFieldVectorial(void);

		static constexpr bool isTheFieldScalar(void);

	public:

		using BASE::BASE;

		// ================================
		//            UTILITY
		// ================================

		// set all components to zero
		constexpr VectorImpl& setToZero(void) requires isVectorial<Field>;

		// set components
		constexpr VectorImpl& setAllComponentsTo(Field c);

		// return the opposite
		constexpr VectorImpl opposite(void) const requires isVectorial<Field>;

		// set to opposite (la NASA usa reflect per la riflessione delle coordinate (1, 2, 3) -> (3, 2, 1))
		constexpr VectorImpl& flip(void) requires isVectorial<Field>;

		// TODO reflect

		// opposite (same as flip)
		constexpr VectorImpl operator-(void) const requires isVectorial<Field>;

		// Scales the vector so that this->norm() = 1 (||this|| = 1)
		constexpr VectorImpl& normalize(void) requires lc::math::concepts::ScalarType<Field>;

		// returns a normalized vector (calls normalize() on a copy)
		constexpr VectorImpl versor(void) const requires lc::math::concepts::ScalarType<Field>;

		// Scales so that || ||_2 = r
		constexpr VectorImpl& setNorm(Field r) requires lc::math::concepts::ScalarType<Field>;

		// Check if a vector is null (all zeros)
		constexpr bool isNull(void) const requires VectorialType<Field> && requires (Field x) { { x != x }; };

		// Check if a vector is null with a norm within a certain tolerance
		constexpr bool isNull(Field epsylon) const requires lc::math::concepts::ScalarType<Field>;

		// Distance to another vector
		constexpr auto dist(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field>;

		// Angle between two vectors
		constexpr auto ang(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field>;

		// Midpoint of two vectors
		constexpr VectorImpl midpoint(const VectorImpl& other) const requires VectorialType<Field>;

		// ================================
		//         MATH OPERATORS
		// ================================

		// Sum of the components
		// prima c'era questo:
		// requires requires (Field x) { { x += x }; }
		// che in realt� non garantisce che Field() sia zero
		constexpr Field componentSum(void) const requires VectorialType<Field>;

		// Euclidean norm ( ||x||_2 )
		constexpr auto magnitude(void) const requires VectorialType<Field>;

		// Euclidean norm ( ||x||_2 )
		constexpr auto lenght(void) const requires lc::math::concepts::ScalarType<Field>;

		// Euclidean norm ( ||x||_2 )
		constexpr auto norm(void) const requires lc::math::concepts::ScalarType<Field>;

		// Squared Euclidean norm ( ||x||_2^2 )
		constexpr auto norm2(void) const requires lc::math::concepts::ScalarType<Field>;

		// Manhattan norm ( ||x||_1 )
		constexpr auto manhattanNorm(void) const requires lc::math::concepts::ScalarType<Field>;

		// p-norm ( ||x||_p )
		// By deault it returns ||x||_2
		constexpr auto pNorm(Field p = 2) const requires lc::math::concepts::ScalarType<Field>;

		// Maximum norm ( ||x||_Inf )
		constexpr auto maximumNorm(void) const requires lc::math::concepts::ScalarType<Field>;

		// Zero norm ( ||x||_0 )
		// returns  #{x_i != 0}
		constexpr bool zeroNorm(void) const requires lc::math::concepts::ScalarType<Field>;

		constexpr VectorImpl cross(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field> && (StaticSize == 3);

		constexpr VectorImpl vector(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field> && (StaticSize == 3);

		// ================================
		//     OPERATIONAL ASSIGNMENTS
		// ================================

		// Addition assignment
		constexpr VectorImpl& operator+=(const VectorImpl<Field, Container, _IsStaticSize, StaticSize>& other)
			requires requires (Field x) { { x += x }; };

		// Addition assignment
		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires ( ( StaticSize == StaticSize2 ) && (_IsStaticSize == _IsStaticSize2) ) && requires (Field x, Field2 y) { { x += y }; }
		constexpr VectorImpl& operator+=(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& other);

		// Addition assignment
		constexpr VectorImpl& operator+=(const Field& other) requires requires (Field x) { { x += x }; };

		// Subtraction assignment
		constexpr VectorImpl& operator-=(const VectorImpl<Field, Container, _IsStaticSize, StaticSize>& other)
			requires requires (Field x) { { x -= x }; };

		// Addition assignment
		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2) ) && requires (Field x, Field2 y) { { x -= y }; }
		constexpr VectorImpl& operator-=(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& other);

		// Addition assignment
		constexpr VectorImpl& operator-=(const Field& other) requires requires (Field x) { { x -= x }; };

		// Scaling assignment
		// This operator is tempalte because we could have a vector of int {2, 4, 6} that we want to multiply by 0.5 for example
		template <class Field2>
		constexpr VectorImpl& operator*=(const Field2& other)
			requires requires (Field x, Field2 y) { { x *= y }; };

		// Scaling assignment
		template <class Field2>
		constexpr VectorImpl& operator/=(const Field2& other)
			requires requires (Field x, Field2 y) { { x /= y }; };

		// ================================
		//           OPERATORS
		// ================================

		// Sum
		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires requires (Field x, Field2 y) { { x + y }; }
		constexpr auto operator+(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const;

		// sum
		constexpr auto operator+(const Field& rhs) const requires requires (Field x) { { x + x }; } && (!_IsStaticSize || StaticSize > 0);

		// Subtraction
		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires requires (Field x, Field2 y) { { x - y }; }
		constexpr auto operator-(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const;

		// Subtraction
		constexpr auto operator-(const Field& rhs) const requires requires (Field x) { { x - x }; } && (!_IsStaticSize || StaticSize > 0);

		// Scaling
		template <class Field2>
		requires (lc::math::concepts::ScalarType<Field2> && requires (Field x, Field2 y) { { x * y }; })
		constexpr auto operator*(const Field2& rhs) const;

		// Scaling
		template <class Field2>
		requires lc::math::concepts::ScalarType<Field2>&& requires (Field x, Field2 y) { { x / y }; }
		constexpr auto operator/(const Field2& rhs) const;

		// Dot product
		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { {x * y}; }
		constexpr auto operator*(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const;

		template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
		requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { {x* y}; }
		constexpr auto dot(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const;

		// ================================
		//           COMPARISONS
		// ================================

		////////////////////////////////////////////////////////////////
		/// \brief Test if equal to
		////////////////////////////////////////////////////////////////
		constexpr bool operator== (const VectorImpl& right) const;

		////////////////////////////////////////////////////////////////
		/// \brief Test if equal to
		////////////////////////////////////////////////////////////////
		constexpr bool operator!= (const VectorImpl& right) const { return !(*this == right); };

		// ================================
		//          COMPONENTWISE
		// ================================

		// TODO moltiplicazione e divisione componentwise!

		// TODO sposta

		// {1, 2, 3} -> returns {f(1), f(2), f(3)}
		constexpr VectorImpl componentwise(Field function(Field)) const;

		// {a1, a2, a3} {b1, b2, b3} -> returns {f(a1, ba), f(a2, b2), f(a3, b3)}
		constexpr VectorImpl componentwise(Field function(Field, Field), const VectorImpl& right) const;

		// {a1, a2, a3} -> apply {f(1), f(2), f(3)} and returns this
		constexpr VectorImpl& apply_componentwise(Field function(Field));

		// {a1, a2, a3} {b1, b2, b3} -> apply {f(a1, ba), f(a2, b2), f(a3, b3)} and returns this
		constexpr VectorImpl& apply_componentwise(Field function(Field, Field), const VectorImpl& right);

		// ================================
		//           STATIC DATA
		// ================================
	};

// ================================================================================================================================
// ================================================================================================================================
//                                                              INL
// ================================================================================================================================
// ================================================================================================================================

//#define _LC_TMP_TEMPLATE template <class Field, class Container, class _ConcretePointer>
#define _LC_TMP_THIS_CLASS VectorImpl<Field, Container, _IsStaticSize, StaticSize>::THIS_CLASS

	// ================================================================
	//                    VECTOR IMPLEMENTATION
	// ================================================================

	// ================================
	//             INFO
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::isStaticSize(void)
	{
		return _IsStaticSize;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::isTheFieldVectorial(void)
	{
		return isVectorial<Field>;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::isTheFieldScalar(void)
	{
		return isScalar<Field>;
	}

	// ================================
	//            UTILITY
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::setToZero(void) requires isVectorial<Field>
	{
		for (auto& x : *this)
			deRef<Field>(x) = Field();
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::setAllComponentsTo(Field c)
	{
		for (auto& x : *this)
			deRef<Field>(x) = c;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::opposite(void) const requires isVectorial<Field>
	{
		auto tmp = *this;
		return tmp.flip();
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::flip(void) requires isVectorial<Field>
	{
		for (auto& x : *this)
			deRef<Field>(x) = -deRef<Field>(x);
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-(void) const requires isVectorial<Field>
	{
		return this->opposite();
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::normalize(void) requires lc::math::concepts::ScalarType<Field>
	{
		Field n = this->norm();

		if (n == Field()) [[unlikely]]
			this->setToZero();
		else [[likely]]
			* this /= n;

		return *this;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::versor(void) const requires lc::math::concepts::ScalarType<Field>
	{
		auto tmp = *this;
		tmp.normalize();
		return tmp;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::setNorm(Field r) requires lc::math::concepts::ScalarType<Field>
	{
		Field n = this->norm();

		if (n == Field()) [[unlikely]]
			this->setToZero();
		else [[likely]]
			* this *= r / n;

		return *this;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::isNull(void) const requires VectorialType<Field> && requires (Field x) { { x != x }; }
	{
		for (const auto& x : *this)
			if (x != Field())
				return false;
		return true;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::isNull(Field epsylon) const requires lc::math::concepts::ScalarType<Field>
	{
		return this->norm2() <= sqr(abs(epsylon));
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::dist(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field>
	{
		return (*this - other).norm();
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::ang(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field>
	{
		auto cos_v = this->dot(other) / (this->norm() * other.norm());
		using R = decltype(::acos(cos_v));
		if (cos_v > 1)
			return (R)0;
		if (cos_v < -1)
			return (R)std::numbers::pi_v<R>;
		return ::acos(cos_v);
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::midpoint(const VectorImpl& other) const requires VectorialType<Field>
	{
		return (*this + other) / Field(2);
	};

	// ================================
	//         MATH OPERATORS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr Field VectorImpl<Field, Container, _IsStaticSize, StaticSize>::componentSum(void) const requires VectorialType<Field>
	{
		Field sum_v = Field();
		for (const auto& x : *this)
			sum_v += deRef<Field>(x);
		return sum_v;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::magnitude(void) const requires VectorialType<Field>
	{
		return this->norm();
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::lenght(void) const requires lc::math::concepts::ScalarType<Field>
	{
		return this->norm();
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::norm(void) const requires lc::math::concepts::ScalarType<Field>
	{
		return ::sqrt(this->norm2());
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::norm2(void) const requires lc::math::concepts::ScalarType<Field>
	{
		auto sum_v = decltype(sqr(abs(Field())))();
		for (const auto& x : *this)
			sum_v += sqr(abs(deRef<Field>(x)));// TODO ottimizza: in alcuni casi non serrve abs (serve solo per complessi e ipercomplessi)
		return sum_v;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::manhattanNorm(void) const requires lc::math::concepts::ScalarType<Field>
	{
		auto sum_v = decltype(::abs(Field()))();
		for (const auto& x : *this)
			sum_v += ::abs(deRef<Field>(x));
		return sum_v;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::pNorm(Field p) const requires lc::math::concepts::ScalarType<Field>
	{
		// TODO meglio il tipo
		auto sum_v = decltype(abs(Field()))();
		for (const auto& x : *this)
			sum_v += ::pow(::abs(deRef<Field>(x)), p);
		return ::pow(sum_v, Field(1) / p);
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::maximumNorm(void) const requires lc::math::concepts::ScalarType<Field>
	{
		auto max_v = decltype(abs(Field()))();
		for (const auto& x : *this)
			max_v = ::max(::abs(deRef<Field>(x)), max_v);
		return max_v;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::zeroNorm(void) const requires lc::math::concepts::ScalarType<Field>
	{
		for (const auto& x : *this)
			if (deRef<Field>(x) != Field(0))
				return true;
		return false;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::cross(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field> && (StaticSize == 3)
	{
		VectorImpl result;
		result.x() = this->y() * other.z() - this->z() * other.y();
		result.y() = this->z() * other.x() - this->x() * other.z();
		result.z() = this->x() * other.y() - this->y() * other.x();
		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::vector(const VectorImpl& other) const requires lc::math::concepts::ScalarType<Field> && (StaticSize == 3)
	{
		return this->cross(other);
	}

	// ================================
	//     OPERATIONAL ASSIGNMENTS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator+=(const VectorImpl<Field, Container, _IsStaticSize, StaticSize>& other)
		requires requires (Field x) { { x += x }; }
	{
		return this->operator+=<Field, Container, _IsStaticSize, StaticSize>(other);
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { { x += y }; }
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator+=(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& other)
	{
		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() != other.size()) [[unlikely]]
				throw std::invalid_argument("VectorImpl::operator+= requires the sizes to be the same");

		LC_ASSERT_M(this->size() == other.size(), "VectorImpl::operator+= requires the sizes to be the same");

		auto& lhs = *this;
		for (size_t i = 0; i < this->size(); i++)
			deRef<Field>(lhs[i]) += deRef<Field2>(other[i]);

		//auto it = this->begin();
		//auto otherIt = other.begin();
		//for (; it != this->end() && otherIt != other.end(); ++it, ++otherIt)
		//	deRef<Field>(*it) += deRef<Field2>(*otherIt);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator+=(const Field& other)
		requires requires (Field x) { { x += x }; }
	{
		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() < 1) [[unlikely]]
				throw std::invalid_argument("VectorImpl::operator+=(Field) requires the vector the have a size of at least 1");

		for (auto& x : *this)
			deRef<Field>(x) += other;
		return *this;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-=(const VectorImpl<Field, Container, _IsStaticSize, StaticSize>& other)
		requires requires (Field x) { { x -= x }; }
	{
		return this->operator-=<Field, Container, _IsStaticSize, StaticSize>(other);
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { { x -= y }; }
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-=(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& other)
	{
		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() != other.size()) [[unlikely]]
				throw std::invalid_argument("VectorImpl::operator-= requires the sizes to be the same");

		LC_ASSERT_M(this->size() == other.size(), "VectorImpl::operator-= requires the sizes to be the same");

		auto& lhs = *this;
		for (size_t i = 0; i < this->size(); i++)
			deRef<Field>(lhs[i]) -= deRef<Field2>(other[i]);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-=(const Field& other)
		requires requires (Field x) { { x -= x }; }
	{
		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() < 1) [[unlikely]]
				throw std::invalid_argument("VectorImpl::operator-=(Field) requires the vector the have a size of at least 1");

		for (auto& x : *this)
			deRef<Field>(x) -= other;
		return *this;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator*=(const Field2& other)
		requires requires (Field x, Field2 y) { { x *= y }; }
	{
		for (auto& x : *this)
			deRef<Field>(x) *= other;
		return *this;
	};

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator/=(const Field2& other)
		requires requires (Field x, Field2 y) { { x /= y }; }
	{
		for (auto& x : *this)
			deRef<Field>(x) /= other;
		return *this;
	};

	// ================================
	//           OPERATORS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires requires (Field x, Field2 y) { { x + y }; }
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator+(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const
	{
		auto& lhs = *this;

		using RField = decltype(Field() + Field2());

		// this operator outputs a static size vector only and only if both of the input vectors are static size;
		constexpr const bool IsStaticSize = _IsStaticSize && _IsStaticSize2;

		// if we output a static size vector, we have to specify the size...
		constexpr size_t NewStaticSize = IsStaticSize ? MAX(StaticSize, StaticSize2) : 0;

		// ... and create the vector ...
		// TODO std::vector?????
		using _TMP_BASE = std::conditional_t<IsStaticSize, ::lc::priv::VectorDataFixed<NewStaticSize, RField, RField>, std::vector<RField>>;
		using Vec = VectorImpl<RField, _TMP_BASE, IsStaticSize, NewStaticSize>;
		Vec result;

		// ... otherwise we have to properly resize the vector
		if constexpr (!IsStaticSize)
			result.resize(std::max(lhs.size(), rhs.size()));

		for (size_t i = 0; i < result.size(); i++)
		{
			if (i < lhs.size() && i < rhs.size())
			{
				result[i] = deRef<Field>(lhs[i]) + deRef<Field2>(rhs[i]);
				continue;
			}

			if (i < lhs.size())
				result[i] = deRef<Field>(lhs[i]);
			if (i < rhs.size())
				result[i] = deRef<Field2>(rhs[i]);
		}

		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator+(const Field& rhs) const requires requires (Field x) { { x + x }; } && (!_IsStaticSize || StaticSize > 0)
	{
		auto& lhs = *this;

		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() < 1)
				throw std::invalid_argument("VectorImpl::operator+ for dynamic size vectors requires size >= 1");

		using RField = decltype(Field() + Field());
		using _TMP_BASE = std::conditional_t<VectorImpl::isStaticSize(), ::lc::priv::VectorDataFixed<StaticSize, RField, RField>, std::vector<RField>>;
		using Vec = VectorImpl<RField, _TMP_BASE, _IsStaticSize, StaticSize>;
		Vec result;

		if constexpr (!VectorImpl::isStaticSize())
			result.resize(this->size());

		LC_ASSERT_M(result.size() == this->size(), "VectorImpl::operator+ internal error, result vector has wrong size, this is an implementation error");

		for (size_t i = 0; i < this->size(); i++)
			result[i] = deRef<Field>(lhs[i]) + rhs;

		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires requires (Field x, Field2 y) { { x - y }; }
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const
	{
		return *this + (-rhs);// TOOD requires sbagliati
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator-(const Field& rhs) const requires requires (Field x) { { x - x }; } && (!_IsStaticSize || StaticSize > 0)
	{
		return *this + (-rhs);// TOOD requires sbagliati
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2>
	requires (lc::math::concepts::ScalarType<Field2>&& requires (Field x, Field2 y) { { x* y }; })
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator*(const Field2& rhs) const
	{
		auto& lhs = *this;

		using RField = decltype(Field() * Field2());
		using _TMP_BASE = std::conditional_t<VectorImpl::isStaticSize(), ::lc::priv::VectorDataFixed<StaticSize, RField, RField>, std::vector<RField>>;
		using Vec = VectorImpl<RField, _TMP_BASE, _IsStaticSize, StaticSize>;
		Vec result;

		if constexpr (!VectorImpl::isStaticSize())
			result.resize(this->size());

		LC_ASSERT_M(result.size() == this->size(), "VectorImpl::operator* (scaling) internal error, result vector has wrong size, this is an implementation error");

		for (size_t i = 0; i < this->size(); i++)
			result[i] = deRef<Field>(lhs[i]) * rhs;

		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2>
	requires lc::math::concepts::ScalarType<Field2>&& requires (Field x, Field2 y) { { x / y }; }
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator/(const Field2& rhs) const
	{
		auto& lhs = *this;

		using RField = decltype(Field() / Field2());
		using _TMP_BASE = std::conditional_t<VectorImpl::isStaticSize(), ::lc::priv::VectorDataFixed<StaticSize, RField, RField>, std::vector<RField>>;
		using Vec = VectorImpl<RField, _TMP_BASE, _IsStaticSize, StaticSize>;
		Vec result;

		if constexpr (!VectorImpl::isStaticSize())
			result.resize(this->size());

		LC_ASSERT_M(result.size() == this->size(), "VectorImpl::operator/ (scaling) internal error, result vector has wrong size, this is an implementation error");

		for (size_t i = 0; i < this->size(); i++)
			result[i] = deRef<Field>(lhs[i]) / rhs;

		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { {x* y}; }
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator*(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const
	{
		auto& lhs = *this;

		if constexpr (!VectorImpl::isStaticSize())
			if (this->size() != rhs.size()) [[unlikely]]
				throw std::invalid_argument("VectorImpl::operator* (dot) requires the sizes to be the same");

		auto result = decltype(Field() * Field2())();

		for (size_t i = 0; i < this->size(); i++)
			result += deRef<Field>(lhs[i]) * deRef<Field2>(rhs[i]);

		return result;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	template <class Field2, class Container2, bool _IsStaticSize2, size_t StaticSize2>
	requires ((StaticSize == StaticSize2) && (_IsStaticSize == _IsStaticSize2)) && requires (Field x, Field2 y) { {x* y}; }
	inline constexpr auto VectorImpl<Field, Container, _IsStaticSize, StaticSize>::dot(const VectorImpl<Field2, Container2, _IsStaticSize2, StaticSize2>& rhs) const
	{
		return this->operator*(rhs);
	}

	// ================================
	//           COMPARISONS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr bool VectorImpl<Field, Container, _IsStaticSize, StaticSize>::operator== (const VectorImpl& right) const
	{
		// TODO specializzazioni ottimizzate

		auto it = this->begin();
		auto otherIt = right.begin();
		for (; it != this->end() && otherIt != right.end(); ++it, ++otherIt)
			if (deRef<Field>(*it) != deRef<Field>(*otherIt))
				return false;
		// TODO for successivi per dimensioni differenti in template

		return true;
	}

	// ================================
	//          COMPONENTWISE
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::componentwise(Field function(Field)) const
	{
		auto v = *this;
		for (auto& x : v)
			deRef<Field>(x) = function(deRef<Field>(x));
		return v;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize> VectorImpl<Field, Container, _IsStaticSize, StaticSize>::componentwise(Field function(Field, Field), const VectorImpl& right) const
	{
		auto v = *this;
		auto it = v.begin();
		auto otherIt = right.begin();
		for (; it != v.end() && otherIt != right.end(); it++, otherIt++)
			deRef<Field>(*it) = function(deRef<Field>(*it), deRef<Field>(*otherIt));
		return v;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::apply_componentwise(Field function(Field))
	{
		auto& v = *this;
		for (auto& x : v)
			deRef<Field>(x) = function(deRef<Field>(x));
		return *(VectorImpl<Field, Container, _IsStaticSize, StaticSize>*)this;
	}

	////////////////////////////////////////////////////////////////
	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	inline constexpr VectorImpl<Field, Container, _IsStaticSize, StaticSize>& VectorImpl<Field, Container, _IsStaticSize, StaticSize>::apply_componentwise(Field function(Field, Field), const VectorImpl& right)
	{
		auto& v = *this;
		auto it = v.begin();
		auto otherIt = right.end();
		for (; it != v.end() && otherIt != right.end(); it++, otherIt++)
			deRef<Field>(*it) = function(deRef<Field>(*it), deRef<Field>(*otherIt));
		return *(VectorImpl<Field, Container, _IsStaticSize, StaticSize>*)this;
	}

	// ================================
	//           STATIC DATA
	// ================================

#undef _LC_TMP_THIS_CLASS

}

namespace std
{
	template <class T>
	std::string to_string(T* ptr)
	{
		std::stringstream ss;
		ss << ptr;
		return ss.str();
	}

	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	requires requires(Field x) { { std::to_string(x) } -> std::same_as<std::string>; }
		inline constexpr std::string to_string(const lc::VectorImpl<Field, Container, _IsStaticSize, StaticSize>& v, int summarizationThreshold = 10, int maxColumns = 100)
	{
		std::vector<std::string> elements;

		size_t size = v.size();
		bool summarize = summarizationThreshold > 0 && v.size() >= summarizationThreshold;

		size_t position = 0;
		size_t maxLenght = 0;
		std::string s;
		for (auto it = v.begin(); it != v.end(); it++, position++)
		{
			s.clear();
			s += " ";

			if (summarize && position >= 3 && position < size - 3)
			{
				s += "...";

				while (position < size - 4)
				{
					position++;
					it++;
				}
			}
			else
			{
				s += std::to_string(*it);
				if constexpr (!std::is_same_v<Field, std::remove_cvref_t<decltype(*it)>>)
				{
					s += " { ";
					s += std::to_string(lc::deRef<Field>(*it));
					s += " }";
				}
			}

			maxLenght = std::max(s.size(), maxLenght);

			elements.push_back(s);
		}

		for (auto& s : elements)
		{
			std::string space;
			for (int i = 0; i < maxLenght - s.size(); i++)
				space += " ";
			s = space + s;
		}

		s = "(";
		std::string row;

		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			row += *it;

			if (it == elements.end() - 1)
				row += ")";
			else
				row += ",";

			if (row.size() > maxColumns)
			{
				if (s.size() > 1)
					s += "\n ";
				s += row; row.clear();
			}
		}

		if (s.size() > 1)
			s += "\n ";
		s += row; row.clear();

		return s;
	}

	template <class Field, class Container, bool _IsStaticSize, size_t StaticSize>
	//LC_REQUIRES(requires(Field x, std::ostream os) { { os << x }; })
		inline constexpr std::ostream& operator<<(std::ostream& os, const lc::VectorImpl<Field, Container, _IsStaticSize, StaticSize>& v)
	{
		os << "(";

		for (auto it = v.begin(); it != v.end(); it++)
		{
			os << *it;
			if (it == v.end() - 1)
				os << ")";
			else
				os << "; ";
		}

		return os;
	}
}
