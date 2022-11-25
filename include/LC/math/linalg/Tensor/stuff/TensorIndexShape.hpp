
#pragma once

#include <type_traits>
#include <ranges>
#include <iterator> // https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
#include <ostream> // for printing
#include <string>
#include <array>
#include <vector>
#include <variant>
#include <cassert>
#include <algorithm>

#include "../tmp/stream_vector_like.hpp"

#include "Tensor_dims_index.hpp"

namespace lc
{
	

	

	template <class Container>
	concept TensorIndexShapeContainer =
		std::ranges::range<Container> &&
		requires (Container c, size_t index) {
			{ c[index] } -> std::convertible_to<TensorDim>;
		};

	// ================================================================
	//                           INDEX SHAPE
	// ================================================================

	// ================================
	//        TENSOR INDEX SHAPE
	// ================================

	/**
	* @brief Represents the shape of a tensor i.e. the sizes of teir index
	* @tparam Container the container for the sizes
	*
	* This class is intened to be used as the final class of an index shape.
	* For example, if the tensor has a fixed number of indexes, the shape is
	* simply a container of the sizes of the indexes, this its size is fixed
	* and it could be implemented as a std::array. In other cases we might need
	* other containers, for example std::vector.
	*
	* Example:
	* ```cpp
	* using Shape = IndexShape<std::array<size_t, 3>>;
	* ```
	*
	* This class is unlikely to be used directly by the user, it is moslty
	* used for this library implementation.
	*/
	template <TensorIndexShapeContainer Container> // TODO check convertible to size_t, concept Container Convertible To 
	struct TensorIndexShapeImpl /*final*/ : Container
	{
		using Container::Container;

		constexpr size_t rank() const {
			return Container::size();
		}

		constexpr size_t elementsCount() const;
	};

	template <class T, size_t N> // TODO check convertible to size_t, concept Container Convertible To 
	struct TensorIndexShapeImpl<std::array<T, N>> /*final*/ : std::array<T, N>
	{
		// aggregate initialization
		
		static constexpr size_t rank() {
			return N;
			//return Container::size();
		}

		constexpr size_t elementsCount() const;
	};

	template <class T>
	struct is_TensorIndexShape : std::false_type {};
	
	template <TensorIndexShapeContainer Container>
	struct is_TensorIndexShape<TensorIndexShapeImpl<Container>> : std::true_type {};

	template <class T>
	concept TensorIndexShape = is_TensorIndexShape<T>::value;

	////////////////////////////////////////////////////////////////
	template <TensorIndexShape ShapeL, std::ranges::range ShapeR>
	constexpr bool equals(const ShapeL& shapeL, const ShapeR& shapeR)
	{
		// check that the elements are equals
		auto l_it = shapeL.begin();
		auto r_it = shapeR.begin();
		for (; l_it != shapeL.end() && r_it != shapeR.end(); ++l_it, ++r_it)
			if (*l_it != *r_it) // TODO maybe also the opposite?
				return false;

		// if they also have the same size (i.e. the iteraotrs are both "end")
		// the shapes are definetly equals
		if (l_it == shapeL.end() && r_it == shapeR.end())
			return true;

		// if they have different sizes, false
		return false;
	}

	////////////////////////////////////////////////////////////////
	template <TensorIndexShape ShapeL, std::ranges::range ShapeR>
	constexpr bool operator==(const ShapeL& shapeL, const ShapeR& shapeR)
	{
		return equals(shapeL, shapeR);
	}

	template <TensorIndexShapeContainer ShapeL> // TODO forse pure sugli altri serve concept tensorcontainer
	constexpr size_t elemCount(const ShapeL& shapeL)
	{
		size_t result = 1;

		// ! temporary, see reshaper.hpp
		//for (const TensorDim& s : shapeL)
		//	result *= s.value();
		for (size_t i = 0; i < shapeL.size(); ++i)
			result *= TensorDim(shapeL[i]).value();

		return result;

		// TODO forse si può fare return product(shapeL | transform(...));
	}

	template <TensorIndexShape ShapeL>
	constexpr bool contains_variable_size(const ShapeL& shapeL) {
		for (const TensorDim& s : shapeL)
			if (s.is_variable())
				return true;
		return false;
	}

	template <TensorIndexShape ShapeL>
	constexpr size_t variable_size_dims_count(const ShapeL& shapeL) {
		return std::count_if(shapeL.begin(), shapeL.end(), [](const TensorDim& dim) { return dim.is_variable(); });
	}

	template <TensorIndexShapeContainer Container>
	constexpr size_t TensorIndexShapeImpl<Container>::elementsCount() const {
		return ::lc::elemCount(*this);
	}

	template <class T, size_t N>
	constexpr size_t TensorIndexShapeImpl<std::array<T, N>>::elementsCount() const {
		return ::lc::elemCount(*this);
	}

	// ================================
	//     CONSTEVAL SHAPE CONTAINER
	// ================================
	/*

	// ================================
	//      STATIC INDEX SHAPE
	// ================================

	template <size_t... Sizes>
	using StaticIndexShape = TensorIndexShapeImpl<ConstevalShapeContainer<Sizes...>>;

	consteval size_t fixedSize(size_t size) { if (size == infinite_size) return 0; return size; }*/

	// Note: static shape container dropped: too complex and no visibile benefits

	template <size_t N> using ArrayShapeContainer = std::array<TensorDim, N>;
	template <size_t N> using FullArrayShapeContainer = std::array<size_t, N>;
	template <size_t N> using TensorShape = TensorIndexShapeImpl<ArrayShapeContainer<N>>;
	template <size_t N> using FullTensorShape = TensorIndexShapeImpl<FullArrayShapeContainer<N>>;

	template <TensorDim... Dims>
	struct TShape;
	
	template <TensorDim Dim, TensorDim... Dims>
	struct TShape<Dim, Dims...> : TensorShape<sizeof...(Dims) + 1>
	{
		// alias
		using ShapeType = TensorShape<sizeof...(Dims) + 1>;
		using FullShapeType = FullTensorShape<sizeof...(Dims) + 1>;
		using Base = ShapeType;

		//using TensorShape<sizeof...(Dims)>::TensorShape;
		constexpr TShape() : Base({ Dim, Dims... }) {}

		static inline constexpr ShapeType shape = { Dim, Dims... };
		static inline constexpr FullShapeType fullShape = { Dim.value(), Dims.value()...};

		using Tail = TShape<Dims...>;

		template <class Ty>
		using PlainArr = TShape<Dims...>::template PlainArr<Ty>[Dim.value()];
	};

	template <>
	struct TShape<> : TensorShape<0>
	{
		// alias
		using ShapeType = TensorShape<0>;
		using FullShapeType = FullTensorShape<0>;
		using Base = ShapeType;

		//using TensorShape<sizeof...(Dims)>::TensorShape;
		constexpr TShape() : Base({ }) {}

		static inline constexpr ShapeType shape = { };
		static inline constexpr FullShapeType fullShape = { };

		template <class Ty>
		using PlainArr = Ty;
	};

	template <class T> struct is_TShape : std::false_type {};
	template <TensorDim... Dims2> struct is_TShape<TShape<Dims2...>> : std::true_type {};
	template <class T> concept TShapeType = is_TShape<T>::value;

	template <size_t N> using tshape = TensorShape<N>; // useles?

	template <size_t N>
	requires (N > 1)
	// TODO the offset templated version
	constexpr auto shape_tail(const TensorShape<N>& shape) {
		TensorShape<N - 1> result;
		for (int i = 0; i < N - 1; ++i)
			result[i] = shape[i + 1];
		return result;
	}

	template <class T> struct is_TensorShape final : std::false_type {};
	template <size_t N> struct is_TensorShape<TensorShape<N>> final : std::true_type {};

	template <class T> concept TensorShapeType = is_TensorShape<T>::value;

	using VariableLenghtTensorShapeContainer = std::vector<size_t>;
	using VariableLenghtTensorShape = TensorIndexShapeImpl<VariableLenghtTensorShapeContainer>;
}

// ================================================================================================================================
// ================================================================================================================================
//                                                           I/O
// ================================================================================================================================
// ================================================================================================================================

std::ostream& operator<<(std::ostream& os, const lc::TensorDim& dim) {
	if (dim.is_fixed())
	{
		os << dim.value();
		return os;
	}

	if (dim.is_variable())
	{
		//os << "{variable}";
		os << "~";
		return os;
	}

	assert(0);
	return os;
}

template <lc::TensorIndexShape Shape>
std::ostream& operator<<(std::ostream& os, const Shape& shape) {
	stream_vector_like(os, shape);
	return os;
}

template <lc::TensorIndexShape Shape>
// TODO require editable
std::istream& operator>>(std::istream& is, Shape& shape) {

	constexpr bool resizable = requires(lc::VariableLenghtTensorShape ts, size_t s) { { ts.resize(s) } -> std::same_as<void>; };

	if constexpr (resizable)
		shape.resize(0);
	std::fill(shape.begin(), shape.end(), 0);

	// https://stackoverflow.com/questions/1403046/example-of-overloading-c-extraction-operator-to-parse-data

	// TODO ...

	return is;
}