#pragma once

#include <initializer_list>
#include <array>

namespace lc
{
	namespace priv_tmp
	{
		// !!!
		template <size_t _rank3> using UtilShape = std::array<size_t, _rank3>;
	}

	// TODO move
	template <TShapeType _Shape, class Ty>
	struct TShape2InitList {
		using initializer_list = std::initializer_list<typename TShape2InitList<typename _Shape::Tail, Ty>::initializer_list>;
	};
	template <class Ty>
	struct TShape2InitList<TShape<>, Ty> {
		using initializer_list = Ty;
	};

	template <template <size_t _rank2> class Shape, size_t _rank, class T>
	constexpr Shape<_rank> toInitShape(const std::initializer_list<T>& list) {

		if constexpr (_rank == 1)
		{
			Shape<1> shape = {};
			shape[0] = list.size();
			return shape;
		}
		else
		{
			std::array<size_t, _rank> shapeArr = {};
			shapeArr[0] = list.size();
			for (const auto& l : list)
			{
				using subType = typename T::value_type;
				//template <size_t _rank3> using UtilShape = std::array<size_t, _rank3>;
				const auto subShape = toInitShape<priv_tmp::UtilShape, _rank - 1, subType>(l);
				for (size_t i = 0; i < _rank - 1; ++i)
					shapeArr[i + 1] = std::max<size_t>(subShape[i], shapeArr[i + 1]);
			}
			Shape<_rank> shape = {};
			for (size_t i = 0; i < _rank; ++i)
				shape[i] = shapeArr[i];
			return shape;
		}
	}

	template <template <size_t _rank2> class Shape, size_t _rank, class T, class Func>
	constexpr void iterateNestedInitList(const std::initializer_list<T>& list, Func func) {
		if constexpr (_rank == 1)
		{
			Shape<1> index = {};
			auto it = list.begin();
			for (size_t i = 0; it != list.end(); ++i, ++it)
				func((index[0] = i, index), *it);
		}
		else
		{
			Shape<_rank> index = {};
			auto it = list.begin();
			for (size_t i = 0; it != list.end(); ++i, ++it)
			{
				using subType = typename T::value_type;
				index[0] = i;
				iterateNestedInitList<Shape, _rank - 1, subType>(*it, [&](const Shape<_rank - 1>& subIndex, const auto& value) constexpr {
					for (size_t i = 0; i < _rank - 1; ++i)
						index[i + 1] = subIndex[i];
					func(index, value);
				});
			}
		}
	}
}