
#pragma once

#include "DenseMatrix.hpp"

#include <cassert>

namespace lc::priv
{
	// ================================================================
	//             VECTOR - BEGIN-END ITERATOR - DATA
	// ================================================================

	// ================================
	//         CONSTRUCTORS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr VectorBeginEndIteratorData<Iterator>::VectorBeginEndIteratorData(Iterator begin, Iterator end) :
		m_begin(begin), m_end(end)// init iterators
	{
	}

	// ================================
	//            ACCESS
	// ================================

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr auto VectorBeginEndIteratorData<Iterator>::size(void) const
	{
		// TODO std::distance forse è più chiaro?
		return m_end - m_begin;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr Iterator& VectorBeginEndIteratorData<Iterator>::begin(void)
	{
		return m_begin;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const Iterator& VectorBeginEndIteratorData<Iterator>::begin(void) const
	{
		return m_begin;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const Iterator& VectorBeginEndIteratorData<Iterator>::cbegin(void) const
	{
		return m_begin;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr Iterator& VectorBeginEndIteratorData<Iterator>::end(void)
	{
		return m_end;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const Iterator& VectorBeginEndIteratorData<Iterator>::end(void) const
	{
		return m_end;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const Iterator& VectorBeginEndIteratorData<Iterator>::cend(void) const
	{
		return m_end;
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr auto& VectorBeginEndIteratorData<Iterator>::operator[](size_t index)
	{
		assert(index < this->size());

		return *(this->begin() + index);
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const auto& VectorBeginEndIteratorData<Iterator>::operator[](size_t index) const
	{
		assert(index < this->size());

		return *(this->begin() + index);
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr auto& VectorBeginEndIteratorData<Iterator>::at(size_t index)
	{
		if (index >= this->size())
			throw std::runtime_error("index out of bounds");

		// TODO potrebbe non funzionare sempre (es. liste), use std::advance?
		return *(this->begin() + index);
	}

	////////////////////////////////////////////////////////////////
	template <class Iterator>
	inline constexpr const auto& VectorBeginEndIteratorData<Iterator>::at(size_t index) const
	{
		if (index >= this->size())
			throw std::runtime_error("index out of bounds");

		// TODO potrebbe non funzionare sempre (es. liste), use std::advance?
		return *(this->begin() + index);
	}

	// ================================================================
	//                      DYNAMIC STRIDE ITERATOR
	// ================================================================

	// ================================
	//          CONSTRUCTORS
	// ================================

	////////////////////////////////////////////////////////////////
	/*template <class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator>::DynamicStrideIterator(const BaseIterator& it, const BaseIterator& end, size_t stride) :
		BaseIterator(it),
		m_stride(stride),
		m_end(end)
	{
	}

	// ================================
	//           OPERATORS
	// ================================

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator>& DynamicStrideIterator<BaseIterator>::operator++()
	{
		for (size_t i = 0; i < m_stride && m_end != *this; i++)
			++(*(BaseIterator*)this);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator> DynamicStrideIterator<BaseIterator>::operator++(int)
	{
		auto tmp = *this;

		++(*this);

		return tmp;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator>& DynamicStrideIterator<BaseIterator>::operator--()
	{
		if (m_end == *this)
			--(*(BaseIterator*)this);
		else
			for (size_t i = 0; i < m_stride; i++)
				--(*(BaseIterator*)this);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator> DynamicStrideIterator<BaseIterator>::operator--(int)
	{
		auto tmp = *this;

		--(*this);

		return tmp;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator>& DynamicStrideIterator<BaseIterator>::operator+=(typename BaseIterator::difference_type n)
		requires requires (BaseIterator x, BaseIterator::difference_type n) { { x += n } -> std::same_as<BaseIterator&>; }
	{
		for (size_t i = 0; i < n * m_stride && m_end != *this; i++)
			++(*(BaseIterator*)this);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator> DynamicStrideIterator<BaseIterator>::operator+(typename THIS_CLASS::difference_type n) const
		requires requires (BaseIterator x, BaseIterator::difference_type n) { { x + n } ->std::same_as<BaseIterator>; }
	{
		auto tmp = *this;
		tmp += n;
		return tmp;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator>& DynamicStrideIterator<BaseIterator>::operator-=(typename BaseIterator::difference_type n)
		requires requires (BaseIterator x, BaseIterator::difference_type n) { { x -= n} -> std::same_as<BaseIterator&>; }
	{
		if (m_end == *this && n != 0)
		{
			--(*(BaseIterator*)this);
			(*(BaseIterator*)this) -= ((n - 1) * m_stride);
		}
		else
			(*(BaseIterator*)this) -= (n * m_stride);

		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr DynamicStrideIterator<BaseIterator> DynamicStrideIterator<BaseIterator>::operator-(typename THIS_CLASS::difference_type n) const
		requires requires (BaseIterator x, BaseIterator::difference_type n) { { x - n} -> std::same_as<BaseIterator>; }
	{
		auto tmp = *this;
		tmp -= n;
		return tmp;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	inline constexpr typename DynamicStrideIterator<BaseIterator>::difference_type DynamicStrideIterator<BaseIterator>::operator-(THIS_CLASS right) const
	{
		assert(std::distance((BaseIterator&)right, (BaseIterator&)*this) % m_stride == 0);

		return std::distance((BaseIterator&)right, (BaseIterator&)*this) / m_stride;
	}

	////////////////////////////////////////////////////////////////
	template<class BaseIterator>
	constexpr auto DynamicStrideIterator<BaseIterator>::operator[](typename THIS_CLASS::difference_type n) const
		requires requires (BaseIterator x, BaseIterator::difference_type n, size_t stride) { { x[n * stride] }; }
	{
		return ((BaseIterator&)*this)[n * m_stride];
	}*/
}