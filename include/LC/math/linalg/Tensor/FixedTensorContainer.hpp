
#pragma once

#include <vector>

// for ostream, see https://en.cppreference.com/w/cpp/feature_test
#ifdef __cpp_lib_char8_t
#include <ostream>
#endif // __cpp_lib_char8_t

// !!!
#include <type_traits>

#include "stuff/TensorIndex.hpp"
#include "stuff/PartialFixedTensorShapeHolder.hpp"

// !!!
#include "tmp/string_utils.hpp"
#include "stuff/init_list_utils.hpp"
#include "stuff/reshaper.hpp"
#include "stuff/TMP_TENSOR_IMPL.hpp"

namespace lc
{
	// ================================================================
	//              FixedTensorContainerData_variable
	// ================================================================

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	struct FixedTensorContainerData_variable : PartialFixedTensorShapeHolder<_Shape>
	{
	private:
		using Base = PartialFixedTensorShapeHolder<_Shape>;
	public:
		using Shape = typename Base::Shape;
		using Base::staticShape;
		using Base::variableSizeCount;

		static constexpr size_t rank() { return staticShape.rank(); }

		constexpr Ty* data() { return m_data.data(); }
		constexpr const Ty* data() const { return m_data.data(); }

		constexpr size_t offsetOf(const std::span<const size_t, rank()>& indexes) const;

		constexpr FixedTensorContainerData_variable(const typename TShape2InitList<_Shape, Ty>::initializer_list& initList);

		constexpr void reshape(const Shape& shape);

	private:

	private:
		// TODO {#vector-optimization} the last part of the shape that is static, should be threated separately, for example:
		// if shape is { 3, {}, 3, 3, {}, 5, 5 }
		// we shoudl actually store a vector of a 5x5 matrix with dim { 3, {}, 3, 3, {} }. In this way,
		// we can save 1 size_t because it will be encoded in the std::vector size. For example,
		// if the user requires a tensor of shape { {} } it should behave exaclty like std::vector<>
		// by inheriting from it, while { {}, 5 } could designate a matrix with 5 columns and the container
		// should behave like a std::vector of Vec5.
		// Remember that we have to remove the FixedRankShapeHolder and unify these two calsses since, if we inherit
		// from two calsses, aditional space would be likely to be used.
		std::vector<Ty> m_data;
	};

	// this is a temporary specialization, see #vector-optimization
	template <template <class T> class _TImpl, class Ty>
	struct FixedTensorContainerData_variable<_TImpl, Ty, TShape<variable_size>> : std::vector<double>
	{
		using Shape = TensorShape<1>;
		static inline constexpr Shape staticShape = TShape<variable_size>::shape;
		inline static constexpr size_t variableSizeCount = 1;
		constexpr Shape shape() const noexcept { Shape shape; shape[0] = this->size(); return shape; }
		static constexpr size_t rank() { return 1; }

		constexpr void reshape(const Shape& shape) {
			assert(shape[0].is_fixed());
			this->resize(shape[0].is_fixed() ? shape[0].value() : 0);
		}

		constexpr size_t offsetOf(const std::span<const size_t, 1>& indexes) const { return indexes[0]; }
	};

	// ================================================================
	//                FixedTensorContainerData_static
	// ================================================================

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	//requires (!contains_variable_size(_shape))
	struct FixedTensorContainerData_static//<_TImpl, Ty, _rank, _shape>
	{
		using Shape = TensorShape<_Shape::shape.rank()>;
		inline static constexpr size_t variableSizeCount = 0;
		static inline constexpr Shape staticShape = _Shape::shape;
		static constexpr auto shape() noexcept { return staticShape; }
		static constexpr size_t rank() { return staticShape.rank(); }

		constexpr Ty* data() { return m_data.data(); }
		constexpr const Ty* data() const { return m_data.data(); }

		static constexpr size_t offsetOf(const std::span<const size_t, rank()>& indexes);

	private:
		std::array<Ty, elemCount(staticShape)> m_data;
	};
	// TODO specialize of single dim array

	// ================================================================
	//                    FixedTensorContainer
	// ================================================================
	
	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	struct FixedTensorContainer : std::conditional_t<contains_variable_size(_Shape::shape), FixedTensorContainerData_variable<_TImpl, Ty, _Shape>, FixedTensorContainerData_static<_TImpl, Ty, _Shape>>
	{
		using Base = std::conditional_t<contains_variable_size(_Shape::shape), FixedTensorContainerData_variable<_TImpl, Ty, _Shape>, FixedTensorContainerData_static<_TImpl, Ty, _Shape>>;

		using Base::Base;

		//static inline constexpr bool ccc = contains_variable_size(_shape);
		using Index = std::array<size_t, _Shape::shape.rank()>;
		using IndexSpan = std::span<size_t, _Shape::shape.rank()>;
		using CIndexSpan = std::span<const size_t, _Shape::shape.rank()>;
		using PartialIndex = std::array<TensorDim, _Shape::shape.rank()>;

		static inline constexpr bool has_fixed_rank() { return true; }

		using value_type = Ty;

		constexpr Ty& operator[](const Index& idx) {
			return this->data()[this->offsetOf(idx)];
		}

		constexpr const Ty& operator[](const Index& idx) const {
			return this->data()[this->offsetOf(idx)];
		}

		constexpr Ty& operator[](size_t idx) requires (_Shape::shape.rank() == 1) {
			return this->data()[idx];
		}
	};
}

// ================================================================================================================================
// ================================================================================================================================
//                                                           PRINTING
// ================================================================================================================================
// ================================================================================================================================

#ifdef __cpp_lib_char8_t

template <template <class T> class _TImpl, class Ty, lc::TensorDim _S>
std::ostream& operator<<(std::ostream& os, const lc::FixedTensorContainer<_TImpl, Ty, lc::TShape<_S>>& tensor) {
	os << "( ";
	for (auto it = tensor.begin(); it != tensor.end(); ++it)
		os << *it << (std::next(it) == tensor.end() ? " " : ", ");
	os << ")";
	return os;
}

namespace lc::tmp_priv
{
	struct stream_matrix_options
	{
		bool hideZeros = false;
		bool framed = true;
		std::string separator = "";
	};

	template <template <class T> class _TImpl, class Ty, lc::TensorDim _S1, lc::TensorDim _S2>
	void stream_matrix(std::ostream& os, const lc::FixedTensorContainer<_TImpl, Ty, lc::TShape<_S1, _S2>>& tensor, const stream_matrix_options& options = {})
	{
		auto shape = tensor.shape();
		const auto N = shape[0].value();
		const auto M = shape[1].value();

		using namespace std::string_literals;

		std::vector<std::string> datacolumns(M);
		{
			for (int i = 0; i < N; ++i)
				for (int j = 0; j < M; ++j)
				{
					std::string sep = j == M - 1 ? "" : options.separator;
					// TODO columns[j + 1] += lc::stream_to_string(tensor[i][j]) + ' ' + ((i + 1 == N) ? "" : "\n");
					datacolumns[j] +=
						((tensor[{(size_t)i, (size_t)j}] == Ty() && options.hideZeros) ?
							(" "s + "."s + sep) :
							(" "s + lc::stream_to_string(tensor[{(size_t)i, (size_t)j}]) + sep))
						+ ((i + 1 == N) ? "" : "\n");
				}
		}

		if (options.framed)
		{
			std::string leftColumn, rightColumn;
			for (int i = 0; i < N; ++i)
			{
				if (N == 1)
				{
					leftColumn += "<";
					rightColumn += " >";
					break;
				}

				leftColumn += (i == 0 ? "/\n" : (i == N - 1 ? "\\" : "|\n"));
				rightColumn += (i == 0 ? " \\\n" : (i == N - 1 ? " /" : " |\n"));
			}
			datacolumns.insert(datacolumns.begin(), leftColumn);
			datacolumns.insert(datacolumns.end(), rightColumn);
		}

		os << lc::put_beside(datacolumns, lc::TerminalHAlign::Right);
	}
}
namespace lc
{
	template <template <class T> class _TImpl, class T, TensorDim _N, TensorDim _M>
	struct tmp_matrix_hide_zeors {

		using Mat = lc::FixedTensorContainer<_TImpl, T, TShape<_N, _M>>;

		tmp_matrix_hide_zeors(const Mat& mat) : matrix(mat) {};

		const Mat& matrix;
	};

	template <template <class T> class _TImpl, class T, TensorDim _N, TensorDim _M>
	auto hide_zeors(const lc::FixedTensorContainer<_TImpl, T, TShape<_N, _M>>& mat) {
		return tmp_matrix_hide_zeors<_TImpl, T, _N, _M>(mat);
	}
}

template <template <class T> class _TImpl, class Ty, lc::TensorDim _S1, lc::TensorDim _S2>
std::ostream& operator<<(std::ostream& os, const lc::FixedTensorContainer<_TImpl, Ty, lc::TShape<_S1, _S2>>& tensor)
{
	lc::tmp_priv::stream_matrix(os, tensor);
	return os;
}

template <template <class T> class _TImpl, class Ty, lc::TensorDim _S1, lc::TensorDim _S2>
std::ostream& operator<<(std::ostream& os, const lc::tmp_matrix_hide_zeors<_TImpl, Ty, _S1, _S2>& tensor)
{
	lc::tmp_priv::stream_matrix(os, tensor.matrix, { .hideZeros = true });
	return os;
}

#endif // __cpp_lib_char8_t

// ================================================================================================================================
// ================================================================================================================================
//                                                             IMPL
// ================================================================================================================================
// ================================================================================================================================


namespace lc
{

	// ================================================================
	//              FixedTensorContainerData_variable
	// ================================================================

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	inline constexpr size_t FixedTensorContainerData_variable<_TImpl, Ty, _Shape>::offsetOf(const std::span<const size_t, rank()>& indexes) const
	{

		size_t off = 0;
		size_t prod = 1;

		for (size_t _i = rank(); _i > 0; --_i)
		{
			const auto i = _i - 1;

			off += indexes[i] * prod;
			prod *= this->shape()[i].value();
		}

		return off;
	}

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	inline constexpr FixedTensorContainerData_variable<_TImpl, Ty, _Shape>::FixedTensorContainerData_variable(const typename TShape2InitList<_Shape, Ty>::initializer_list& initList)
	{
		using SubT = typename TShape2InitList<_Shape, Ty>::initializer_list::value_type;
		const auto shape = toInitShape<TensorShape, rank(), SubT>(initList);

		this->reshape(shape);

		iterateNestedInitList<TensorIndex, rank(), SubT>(initList, [this](const auto& idx, const auto& value) constexpr {
			this->data()[this->offsetOf(idx)] = value;
			});
	}

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	inline constexpr void FixedTensorContainerData_variable<_TImpl, Ty, _Shape>::reshape(const Shape& _shape)
	{
		const Shape shape = [&]() constexpr {
			Shape shape = {};
			for (size_t i = 0; i < shape.rank(); ++i)
				shape[i] = _shape[i].is_fixed() ? _shape[i].value() : this->shape()[i].value();
			return shape;
		}();

		const size_t oldSize = m_data.size();
		const size_t newSize = elemCount(shape);
		const bool preResize = newSize > oldSize;

		if (preResize)
			m_data.resize(newSize);
		{
			const auto currshape = this->shape();
			std::array<size_t, staticShape.rank()> arrCurrShape; for (size_t i = 0; i < staticShape.rank(); ++i) arrCurrShape[i] = currshape[i].value();
			std::array<size_t, staticShape.rank()> arrNewShape; for (size_t i = 0; i < staticShape.rank(); ++i) arrNewShape[i] = shape[i].value();
			//this->reshape_data<staticShape.rank()>(arrCurrShape, arrNewShape, this->data(), this->data());
			TensorDataReshaper<rank()>::template reshape_data<Ty>(arrCurrShape, arrNewShape, this->data(), this->data());

			// store the new shape
			this->mem_shape(shape);
		}
		if (!preResize)
			m_data.resize(newSize);
	}

	// ================================================================
	//                FixedTensorContainerData_static
	// ================================================================

	template <template <class T> class _TImpl, class Ty, TShapeType _Shape>
	constexpr size_t FixedTensorContainerData_static<_TImpl, Ty, _Shape>::offsetOf(const std::span<const size_t, rank()>& indexes)
	{

		size_t off = 0;
		size_t prod = 1;

		for (size_t _i = rank(); _i > 0; ++_i)
		{
			const auto i = _i - 1;

			off += indexes[i] * prod;
			prod *= shape()[i].value();
		}

		return off;
	}
}