
#pragma once

#include <array>
#include <span>

#include "../tmp/stream_vector_like.hpp"

#include "Tensor_dims_index.hpp"

namespace lc
{
	template <size_t _rank, class T>
	class TensorIndexImpl : public std::array<T, _rank>
	{
		using Base = std::array<T, _rank>;
	public:

		constexpr TensorIndexImpl() : Base({}) {};
		constexpr TensorIndexImpl(const TensorIndexImpl&) = default;
		constexpr TensorIndexImpl(TensorIndexImpl&&) = default;
		constexpr TensorIndexImpl(const Base& base) : Base(base) {};
		constexpr TensorIndexImpl(Base&& base) : Base(std::move(base)) {};
		constexpr TensorIndexImpl(const std::span<const T, _rank>& span) : TensorIndexImpl() {
			auto it = this->begin();
			auto spanIt = span.begin();
			for (; it != this->end() && spanIt != span.end(); ++it, ++spanIt)
				*it = *spanIt;
		};
		// TOOD written by copilot, correct? constexpr TensorIndexImpl(const size_t(&arr)[_rank]) : Base(arr) {};
		constexpr TensorIndexImpl(const std::initializer_list<T>& list) : Base({}) {
			for (size_t i = 0; i < list.size() && i < _rank; ++i)
				// TODO BAD! do not use next
				this->operator[](i) = *std::next(list.begin(), i);
		}
		constexpr TensorIndexImpl(std::initializer_list<T>&& list) : Base({}) {
			for (size_t i = 0; i < list.size() && i < _rank; ++i)
				// TODO BAD! do not use next
				this->operator[](i) = std::move(*std::next(list.begin(), i));
		}

		constexpr TensorIndexImpl& operator=(const TensorIndexImpl&) = default;
		constexpr TensorIndexImpl& operator=(TensorIndexImpl&&) = default;
		constexpr TensorIndexImpl& operator=(const Base& base) { Base::operator=(base); return *this; }
		constexpr TensorIndexImpl& operator=(Base&& base) { Base::operator=(std::move(base)); return *this; }
		constexpr TensorIndexImpl& operator=(std::span<const T, _rank> span) { Base::operator=(span.begin(), span.end()); return *this; }

		constexpr TensorIndexImpl<_rank - 1, T> tail() const requires (_rank > 1) {
			TensorIndexImpl<_rank - 1, T> result;
			for (T i = 1; i < _rank; ++i)
				result[i - 1] = (*this)[i];
			return result;
		}

		constexpr std::span<T, _rank - 1> tailSpan() requires (_rank > 1) {
			return std::span<T, _rank - 1>(this->data() + 1, _rank - 1);
		}

		constexpr std::span<const T, _rank - 1> tailSpan() const requires (_rank > 1) {
			return std::span<const T, _rank - 1>(this->data() + 1, _rank - 1);
		}

	private:
	};

	template <size_t _rank> using TensorIndex = TensorIndexImpl<_rank, size_t>;
	template <size_t _rank> using PartialTensorIndex = TensorIndexImpl<_rank, TensorPartialIndexValue>;
	

	// TODO Variable lenght tensor index (comunque constexpr)
}

template <size_t _rank, class T>
std::ostream& operator<<(std::ostream& os, const lc::TensorIndexImpl<_rank, T>& index) {
	stream_vector_like(os, index);
	return os;
}