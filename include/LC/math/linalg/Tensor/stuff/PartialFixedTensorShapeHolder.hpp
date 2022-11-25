
#pragma once

#include "TensorIndexShape.hpp"
#include "TensorIndex.hpp"

namespace lc
{
	// used to store the shape of a non fully statically determined tensor shape
	template <TShapeType _Shape>
	struct PartialFixedTensorShapeHolder
	{
	private:
		using This = PartialFixedTensorShapeHolder<_Shape>;
	public:
		
		using Shape = TensorShape<_Shape::shape.rank()>;
		static inline constexpr Shape staticShape = _Shape::shape;
		inline static constexpr size_t variableSizeCount = variable_size_dims_count(staticShape);
		constexpr Shape shape() const noexcept;
		constexpr size_t elementsCount() const noexcept { return shape().elementsCount(); }
		static constexpr size_t rank() { return This::staticShape.rank(); }
		constexpr size_t offsetOf(const TensorIndex<This::rank()>& indexes) const;

	protected:

		static constexpr size_t freeSizeInternalIndex(size_t d);

		static constexpr size_t freeSizeInternalIndex2index(size_t d);

		constexpr size_t& freeSizeAt(size_t d);

		constexpr const size_t& freeSizeAt(size_t d) const;

		static constexpr bool is_dim_assignable(size_t index);

		static constexpr bool is_dim_ok_for_reshape(const Shape& shape);

		constexpr void mem_shape(const Shape& shape);

	private:
		// TODO to solve the problem {#vector-optimization} we could store
		// one size less, and then use the total elements count provided by the derived class in some way
		// (CRTP)
		size_t m_freeSizes[variableSizeCount] = {};
	};
}

// ================================================================================================================================
// ================================================================================================================================
//                                                             IMPL
// ================================================================================================================================
// ================================================================================================================================

namespace lc
{
	template <TShapeType _Shape>
	inline constexpr typename PartialFixedTensorShapeHolder<_Shape>::Shape PartialFixedTensorShapeHolder<_Shape>::shape() const noexcept
	{
		auto shape = staticShape;
		for (size_t i = 0; i < variableSizeCount; ++i)
			shape[this->freeSizeInternalIndex2index(i)] = m_freeSizes[i];
		return shape;
	}

	template <TShapeType _Shape>
	inline constexpr size_t PartialFixedTensorShapeHolder<_Shape>::freeSizeInternalIndex(size_t d)
	{
		// TODO with external function, near variable_size_dims_count
		return std::count_if(
			staticShape.begin(),
			staticShape.begin() + std::min<size_t>(d, std::ranges::size(staticShape)),
			[](const TensorDim& dim) { return dim.is_variable(); }
		);
	}

	template <TShapeType _Shape>
	inline constexpr size_t PartialFixedTensorShapeHolder<_Shape>::freeSizeInternalIndex2index(size_t d)
	{
		size_t count = 0;
		for (size_t i = 0; i < staticShape.size(); ++i)
			if (staticShape[i].is_variable())
			{
				if (count == d)
					return i;
				else
					++count;
			}
		assert(0);
		return 0; // TODO error
	}

	template <TShapeType _Shape>
	inline constexpr size_t& PartialFixedTensorShapeHolder<_Shape>::freeSizeAt(size_t d)
	{
		return m_freeSizes[this->freeSizeInternalIndex(d)];
	}

	template <TShapeType _Shape>
	inline constexpr const size_t& PartialFixedTensorShapeHolder<_Shape>::freeSizeAt(size_t d) const
	{
		return m_freeSizes[this->freeSizeInternalIndex(d)];
	}

	template <TShapeType _Shape>
	inline constexpr bool PartialFixedTensorShapeHolder<_Shape>::is_dim_assignable(size_t index)
	{
		return staticShape[index].is_variable();
	}

	template <TShapeType _Shape>
	inline constexpr bool PartialFixedTensorShapeHolder<_Shape>::is_dim_ok_for_reshape(const Shape& shape)
	{
		for (size_t i = 0; i < staticShape.rank(); ++i)
		{
			const TensorDim& dim = shape[i];
			const TensorDim& staticDim = staticShape[i];

			// check for conflicts
			if (dim.is_fixed() && staticDim.is_fixed())
				if (dim.value() != staticDim.value())
					return false;

			// if assignable
			//if (is_dim_assignable(i))
			//	if (!dim.is_fixed())
			//		return false;
		}

		return true;
	}

	template <TShapeType _Shape>
	inline constexpr void PartialFixedTensorShapeHolder<_Shape>::mem_shape(const Shape& shape)
	{
		assert(this->is_dim_ok_for_reshape(shape));

		for (size_t i = 0; i < staticShape.rank(); ++i) {
			const TensorDim& dim = shape[i];
			const TensorDim& staticDim = staticShape[i];

			// if assignable
			if (is_dim_assignable(i))
				freeSizeAt(i) = dim.is_fixed() ? dim.value() : 0;
		}
	}
	
	template <TShapeType _Shape>
	constexpr size_t PartialFixedTensorShapeHolder<_Shape>::offsetOf(const TensorIndex<This::rank()>& indexes) const
	{
		size_t off = 0;
		size_t prod = 1;

		for (size_t _i = This::rank(); _i > 0; --_i)
		{
			const auto i = _i - 1;

			off += indexes[i] * prod;
			prod *= this->shape()[i].value();
		}

		return off;
	}
}