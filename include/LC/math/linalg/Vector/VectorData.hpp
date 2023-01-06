#pragma once

#include <algorithm>
#include <type_traits>
#include <stdexcept>      // std::out_of_range
#include <exception>
#include <string>
#include <cassert>

//#include <LC/config/config.h>
#include <LC/math/concepts/concepts.hpp>
//#include <LC/math/common.hpp>
#include <LC/math/math_defines.h>

#include "vector_concepts.hpp"

#include <array>
#include <functional>
#include <type_traits>

namespace lc::priv
{


#ifndef NDEBUG
#define LC_TMP_VECTOR_ITERATOR_CHECKS true
#else
#define LC_TMP_VECTOR_ITERATOR_CHECKS false
#endif // !NDEBUG

	// TODO remove? never used and useless?
	template <typename R, typename... Args>
	R return_type_of(R(*)(Args...)) { return *((R*)nullptr); };

	/**
	 * @brief An iterator calss for static size vectors
	 * 
	 * This is a random access iterator.
	 * 
	 * Note that in debug configuration, it will check that the iterator is not out of range and this implies that this
	 * class does not store the pointer only. When not in Debug, only a pointer is stored
	 * 
	 * @tparam Field the value type
	 * @tparam T the type of the actual stored values (They could be different from Field, for example `Field*`)
	*/
	template <class Field, class T>
	class MyVectorDataFixedIterator
	{
		using _Myself = MyVectorDataFixedIterator<Field, T>;
	public:

		//using iterator_category = std::contiguous_iterator_tag; // std::random_access_iterator_tag; // std::forward_iterator_tag;
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Field;
		using difference_type = std::ptrdiff_t;
		//using pointer = Field*;
#ifdef __clang_version__
		using reference = afgRT<decltype(deRef_ref<Field, T>)>;
#else
		using reference = typename decltype(std::function{ deRef_ref<Field, T> })::result_type;
#endif
		// https://stackoverflow.com/questions/53673442/simplest-way-to-determine-return-type-of-function
		// TODO RIMETTI QUESTO MA NON FUNZIONA CON CLANG using reference = typename decltype(std::function{ deRef_ref<Field, T> })::result_type;
		//using reference = Field&;
		using pointer = std::remove_reference_t<reference>*;

		// ================================
		//          CONSTRUCTORS
		// ================================

		constexpr MyVectorDataFixedIterator() = default;
		constexpr MyVectorDataFixedIterator(const MyVectorDataFixedIterator&) = default;
		constexpr MyVectorDataFixedIterator(MyVectorDataFixedIterator&&) = default;

#if LC_TMP_VECTOR_ITERATOR_CHECKS
		/**
		 * @brief Constructor from pointer, begin and end.
		 * 
		 * This constructor is enabled only in `Debug` configuration.
		 * 
		 * @param ptr the pointer to the element
		 * @param _begin the pointer to the first element
		 * @param _end past-the-end pointer
		*/
		constexpr MyVectorDataFixedIterator(T* ptr, T* _begin, T* _end);
#else
		/**
		 * @brief Constructor from pointer
		 * 
		 * This constructor is enabled only in **non** `Debug` configurations.
		 * 
		 * @param ptr 
		*/
		constexpr MyVectorDataFixedIterator(T* ptr);
#endif

		/**
		 * @brief Copy assignment
		 * @param other
		 * @return `*this`
		*/
		constexpr MyVectorDataFixedIterator& operator=(const MyVectorDataFixedIterator& other) = default;

		/**
		 * @brief Move assignment
		 * @param other
		 * @return `*this`
		*/
		constexpr MyVectorDataFixedIterator& operator=(MyVectorDataFixedIterator&& other) = default;

		// ================================
		//           OPERATORS
		// ================================

		constexpr auto operator<=>(const MyVectorDataFixedIterator& rhs) const { return m_ptr <=> rhs.m_ptr; }
		constexpr bool operator==(const MyVectorDataFixedIterator& rhs) const { return m_ptr == rhs.m_ptr; }

		// prefix
		constexpr MyVectorDataFixedIterator& operator++();

		// postfix
		constexpr MyVectorDataFixedIterator operator++(int);

		// prefix
		constexpr MyVectorDataFixedIterator& operator--();

		// postfix
		constexpr MyVectorDataFixedIterator operator--(int);

		constexpr MyVectorDataFixedIterator& operator+=(typename MyVectorDataFixedIterator::difference_type n);

		constexpr MyVectorDataFixedIterator operator+(typename MyVectorDataFixedIterator::difference_type n) const;

		constexpr MyVectorDataFixedIterator& operator-=(typename MyVectorDataFixedIterator::difference_type n);

		constexpr MyVectorDataFixedIterator operator-(typename MyVectorDataFixedIterator::difference_type n) const;

		constexpr difference_type operator-(const MyVectorDataFixedIterator& right) const { return this->m_ptr - right.m_ptr; }

		constexpr reference operator[](difference_type i) const;

		explicit constexpr operator pointer() const { return &(*this)[0]; };
		explicit constexpr operator reference() const { return (*this)[0]; };

		pointer operator->() const
		{
			LC_ASSERT_M(("MyVectorDataFixedIterator::operator-> out of bound", this->m_ptr >= this->m_begin || this->m_ptr < this->m_end));
			return &deRef<Field>(this->m_ptr);
		}
		reference operator*() const
		{
			assert(("MyVectorDataFixedIterator::operator* out of bound", this->m_ptr >= this->m_begin || this->m_ptr < this->m_end));
			T* p = m_ptr;
			return deRef<Field>(this->m_ptr);
			return deRef<Field>(p);
		}

	private:
		T* m_ptr = nullptr;

#if LC_TMP_VECTOR_ITERATOR_CHECKS
		T* m_begin = nullptr, * m_end = nullptr;
#endif

	};

	class MyVectorDataFixedSignature {};

	template <class T>
	concept MyVectorDataFixedType = std::is_base_of_v<MyVectorDataFixedSignature, T>;

	/*class MyVectorDataFixedSignature {};

	template <class T>
	struct _extract_MyVectorDataFixed_signature
	{
		using signature = T::_my_signature;
	};

	template <class T>
	concept MyVectorDataFixedType = requires(T x) {
		{ typename _extract_MyVectorDataFixed_signature<T>::signature{} } -> std::same_as<MyVectorDataFixedSignature>;
	};*/

	template <size_t N, class Field, class T>
	class VectorDataFixed
	{
		using _Myself = VectorDataFixed<N, Field, T>;
		/*using _my_signature = MyVectorDataFixedSignature; template <class T> friend class _extract_MyVectorDataFixed_signature;*/
	public:

		using iterator = MyVectorDataFixedIterator<Field, T>;
		using const_iterator = MyVectorDataFixedIterator<Field, const T>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// ================================
		//          CONSTRUCTORS
		// ================================

		// default constructor
		constexpr VectorDataFixed();

		// copy constructor
		constexpr VectorDataFixed(const VectorDataFixed&) = default;

		// move constructor
		constexpr VectorDataFixed(VectorDataFixed&&) = default;

		// homogeneus vector
		constexpr VectorDataFixed(const Field& value)
			requires (std::same_as<Field, T>);

		template <std::convertible_to<Field> T2>
		constexpr VectorDataFixed(const std::initializer_list<T2>& list)
			requires (std::same_as<Field, T>);

		template <std::ranges::range Iterable>
		requires std::same_as<Field, T> && requires (Iterable container) {
			{ *container.begin() } -> std::convertible_to<Field>;
		}
		constexpr VectorDataFixed(const Iterable& container);

		constexpr VectorDataFixed(const T& x, const T& y) requires (N == 2);
		constexpr VectorDataFixed(const T& x, const T& y, const T& z) requires (N == 3);
		constexpr VectorDataFixed(const T& x, const T& y, const T& z, const T& t) requires (N == 4);
		constexpr VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u) requires (N == 5);
		constexpr VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u, const T& v) requires (N == 6);
		constexpr VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u, const T& v, const T& w) requires (N == 7);

		//constexpr VectorDataFixed()

		template <size_t N2, class Field2, class T2>
		requires std::convertible_to<Field2, Field>
		constexpr VectorDataFixed(const VectorDataFixed<N2, Field2, T2>& rhs) :
			_Myself{}
		{
			auto it = this->begin();
			auto otherIt = rhs->begin();
			for (; it != this->end() && otherIt != rhs.end(); ++it, ++otherIt)
				*it = *otherIt;
		}

		constexpr VectorDataFixed& operator=(const VectorDataFixed& rhs);

		template <size_t N2, class Field2, class T2>
		requires std::convertible_to<Field2, Field>
		constexpr VectorDataFixed& operator=(const VectorDataFixed<N2, Field2, T2>& rhs);

		template <size_t N2, class Field2, class T2>
		requires std::convertible_to<Field2, Field>
		constexpr VectorDataFixed& operator=(VectorDataFixed<N2, Field2, T2>&& rhs);

		// ================================
		//             ACCESS
		// ================================

		static constexpr int size(void) { return N; };
		static constexpr bool empty(void) { return N == 0; }
		static constexpr int max_size(void) { return N; };

		constexpr iterator begin(void);
		constexpr const_iterator begin(void) const;
		constexpr const_iterator cbegin(void) const;
		constexpr reverse_iterator rbegin(void);
		constexpr const_reverse_iterator rbegin(void) const;
		constexpr const_reverse_iterator crbegin(void) const;

		constexpr iterator end(void);
		constexpr const_iterator end(void) const;
		constexpr const_iterator cend(void) const;
		constexpr reverse_iterator rend(void);
		constexpr const_reverse_iterator rend(void) const;
		constexpr const_reverse_iterator crend(void) const;

		constexpr Field& front(void);
		constexpr const Field& front(void) const;
		constexpr const Field& cfront(void) const;

		constexpr Field& back(void);
		constexpr const Field& back(void) const;
		constexpr const Field& cback(void) const;

		constexpr VectorDataFixed<N, T, T>& asRawContainer(void);
		constexpr const VectorDataFixed<N, T, T>& asRawContainer(void) const;
		constexpr const VectorDataFixed<N, T, T>& asConstRawContainer(void) const;

		constexpr Field& at(std::size_t i);
		constexpr const Field& at(std::size_t i) const;
		constexpr const Field& cat(std::size_t i) const;

		constexpr Field& operator[] (std::size_t i);
		constexpr const Field& operator[] (std::size_t i) const;

		using ArrayNT = T[N];
		using ArrayNTR = ArrayNT&;
		using CArrayNTR = const ArrayNT&;

		constexpr ArrayNTR data(void);
		constexpr CArrayNTR data(void) const;
		constexpr CArrayNTR cdata(void) const;

		constexpr Field& x(void) requires (N >= 1); constexpr const Field& x(void) const requires (N >= 1);
		constexpr Field& y(void) requires (N >= 2); constexpr const Field& y(void) const requires (N >= 2);
		constexpr Field& z(void) requires (N >= 3); constexpr const Field& z(void) const requires (N >= 3);
		constexpr Field& t(void) requires (N >= 4); constexpr const Field& t(void) const requires (N >= 4);
		constexpr Field& u(void) requires (N >= 5); constexpr const Field& u(void) const requires (N >= 5);
		constexpr Field& v(void) requires (N >= 6); constexpr const Field& v(void) const requires (N >= 6);
		constexpr Field& w(void) requires (N >= 7); constexpr const Field& w(void) const requires (N >= 7);

	private:
		T m_data[(N > 0 ? N : 1)];
	};

	//std::ranges::begin

	//VectorDataFixed<3, float, float> v({ 1, 2, 3 });

	/*constexpr bool a = MyVectorDataFixedType<VectorDataFixed<3, int, int>>;
	constexpr bool a = MyVectorDataFixedType<int>;*/
}

// ================================================================================================================================
// ================================================================================================================================
//                                                              INL
// ================================================================================================================================
// ================================================================================================================================


namespace lc::priv
{
	template <class Field, class T>
#if LC_TMP_VECTOR_ITERATOR_CHECKS
	inline constexpr MyVectorDataFixedIterator<Field, T>::MyVectorDataFixedIterator(T* ptr, T* _begin, T* _end) :
		m_ptr(ptr), m_begin(_begin), m_end(_end)
	{
		assert(("MyVectorIterator_data created with invalid parameters", ptr >= _begin && ptr <= _end));
	};
#else
	inline constexpr MyVectorDataFixedIterator<Field, T>::MyVectorDataFixedIterator(T* ptr) :
		m_ptr(ptr)
	{
	};
#endif

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T>& MyVectorDataFixedIterator<Field, T>::operator++()
	{
#if LC_TMP_VECTOR_ITERATOR_CHECKS
		assert(("MyVectorDataFixedIterator::operator++ failed, cannot increment past-the-end iterators", this->m_ptr < this->m_end));
#endif
		++(this->m_ptr);
		return *this;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T> MyVectorDataFixedIterator<Field, T>::operator++(int)
	{
		MyVectorDataFixedIterator tmp(*this);
		++(*this);
		return tmp;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T>& MyVectorDataFixedIterator<Field, T>::operator--()
	{
		assert(("MyVectorDataFixedIterator::operator-- failed", this->m_ptr > this->m_begin));
		--(this->m_ptr);
		return *this;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T> MyVectorDataFixedIterator<Field, T>::operator--(int)
	{
		MyVectorDataFixedIterator tmp(*this);
		--(*this);
		return tmp;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T>& MyVectorDataFixedIterator<Field, T>::operator+=(typename MyVectorDataFixedIterator::difference_type n)
	{
#if LC_TMP_VECTOR_ITERATOR_CHECKS
		assert(("MyVectorDataFixedIterator::operator+= out of bound", this->m_ptr + n <= this->m_end));
#endif
		(this->m_ptr) += n;
		return *this;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T> MyVectorDataFixedIterator<Field, T>::operator+(typename MyVectorDataFixedIterator::difference_type n) const
	{
		MyVectorDataFixedIterator tmp(*this);
		tmp += n;
		return tmp;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T>& MyVectorDataFixedIterator<Field, T>::operator-=(typename MyVectorDataFixedIterator::difference_type n)
	{
		assert(("MyVectorDataFixedIterator::operator-= out of bound", this->m_ptr - n >= this->m_begin));
		(this->m_ptr) -= n;
		return *this;
	}

	template <class Field, class T>
	inline constexpr MyVectorDataFixedIterator<Field, T> MyVectorDataFixedIterator<Field, T>::operator-(typename MyVectorDataFixedIterator::difference_type n) const
	{
		MyVectorDataFixedIterator tmp(*this);
		tmp -= n;
		return tmp;
	}

	template <class Field, class T>
	inline constexpr typename MyVectorDataFixedIterator<Field, T>::reference MyVectorDataFixedIterator<Field, T>::operator[](difference_type i) const
	{
		if constexpr (LC_TMP_VECTOR_ITERATOR_CHECKS)
			if (this->m_ptr + i >= this->m_end)
				throw std::out_of_range("MyVectorIterator::operator[], index out of range");
		return deRef<Field>(this->m_ptr[i]);
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed() :
		// value initialization (default constructor or zeors for foundamental types)
		m_data{ {} }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const Field& value)
		requires (std::same_as<Field, T>)
		: _Myself{}
	{
		for (auto& x : *this)
			x = value;
	}

	template <size_t N, class Field, class T>
	template <std::convertible_to<Field> T2>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const std::initializer_list<T2>& list)
		requires (std::same_as<Field, T>)
		: _Myself{}
	{
		auto it = this->begin();
		auto listIt = list.begin();
		for (; it != this->end() && listIt != list.end(); ++it, ++listIt)
			*it = *listIt;
	}

	template <size_t N, class Field, class T>
	template <std::ranges::range Iterable>
	requires std::same_as<Field, T>&& requires (Iterable container) {
		{ *container.begin() } -> std::convertible_to<Field>;
	}
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const Iterable& container) :
		_Myself{}
	{
		auto it = this->begin();
		auto otherIt = container.begin();

		for (; it != this->end() && otherIt != container.end(); ++it, ++otherIt)
			*it = *otherIt;
	}

#if LC_TMP_VECTOR_ITERATOR_CHECKS
#define _LC_TMP_INIT_ITERATOR(_ptr, _begin, _end) {_ptr, _begin, _end}
#else
#define _LC_TMP_INIT_ITERATOR(_ptr, _begin, _end) _ptr
#endif

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::iterator VectorDataFixed<N, Field, T>::begin(void)
	{
		return _LC_TMP_INIT_ITERATOR(m_data, m_data, m_data + N);
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_iterator VectorDataFixed<N, Field, T>::begin(void) const
	{
		return _LC_TMP_INIT_ITERATOR(m_data, m_data, m_data + N);
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_iterator VectorDataFixed<N, Field, T>::cbegin(void) const
	{
		return this->begin();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::reverse_iterator VectorDataFixed<N, Field, T>::rbegin(void)
	{
		return (reverse_iterator)this->end();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_reverse_iterator VectorDataFixed<N, Field, T>::rbegin(void) const
	{
		return (const_reverse_iterator)this->end();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_reverse_iterator VectorDataFixed<N, Field, T>::crbegin(void) const
	{
		return this->rbegin();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::iterator VectorDataFixed<N, Field, T>::end(void)
	{
		return _LC_TMP_INIT_ITERATOR(m_data + N, m_data, m_data + N);
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_iterator VectorDataFixed<N, Field, T>::end(void) const
	{
		return _LC_TMP_INIT_ITERATOR(m_data + N, m_data, m_data + N);
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_iterator VectorDataFixed<N, Field, T>::cend(void) const
	{
		return this->end();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::reverse_iterator VectorDataFixed<N, Field, T>::rend(void)
	{
		return (reverse_iterator)this->begin();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_reverse_iterator VectorDataFixed<N, Field, T>::rend(void) const
	{
		return (const_reverse_iterator)this->begin();
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::const_reverse_iterator VectorDataFixed<N, Field, T>::crend(void) const
	{
		return this->rend();
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::front(void)
	{
		return *this->begin();
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::front(void) const
	{
		return *this->begin();
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::cfront(void) const
	{
		return this->front();
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::back(void)
	{
		return *this->end();
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::back(void) const
	{
		return *this->end();
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::cback(void) const
	{
		return this->back();
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, T, T>& VectorDataFixed<N, Field, T>::asRawContainer(void)
	{
		return *reinterpret_cast<VectorDataFixed<N, T, T>*>(this);
	}

	template <size_t N, class Field, class T>
	inline constexpr const VectorDataFixed<N, T, T>& VectorDataFixed<N, Field, T>::asRawContainer(void) const
	{
		return *reinterpret_cast<const VectorDataFixed<N, T, T>*>(this);
	}

	template <size_t N, class Field, class T>
	inline constexpr const VectorDataFixed<N, T, T>& VectorDataFixed<N, Field, T>::asConstRawContainer(void) const
	{
		return this->asRawContainer();
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::at(std::size_t i)
	{
		if (i >= N)
			throw std::out_of_range("VectorDataFixed::at index out of range");
		return deRef<Field>(m_data[i]);
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::at(std::size_t i) const
	{
		if (i >= N)
			throw std::out_of_range("VectorDataFixed::at index out of range");
		return deRef<Field>(m_data[i]);
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::cat(std::size_t i) const
	{
		return this->at(i);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::operator[] (std::size_t i)
	{
		return deRef<Field>(m_data[i]);
	}

	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::operator[] (std::size_t i) const
	{
		return deRef<Field>(m_data[i]);
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::ArrayNTR VectorDataFixed<N, Field, T>::data(void)
	{
		return m_data;
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::CArrayNTR VectorDataFixed<N, Field, T>::data(void) const
	{
		return m_data;
	}

	template <size_t N, class Field, class T>
	inline constexpr typename VectorDataFixed<N, Field, T>::CArrayNTR VectorDataFixed<N, Field, T>::cdata(void) const
	{
		return this->data();
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::x(void)
		requires (N >= 1)
	{
		return deRef<Field>(m_data[0]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::x(void) const
		requires (N >= 1)
	{
		return deRef<Field>(m_data[0]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::y(void)
		requires (N >= 2)
	{
		return deRef<Field>(m_data[1]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::y(void) const
		requires (N >= 2)
	{
		return deRef<Field>(m_data[1]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::z(void)
		requires (N >= 3)
	{
		return deRef<Field>(m_data[2]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::z(void) const
		requires (N >= 3)
	{
		return deRef<Field>(m_data[2]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::t(void)
		requires (N >= 4)
	{
		return deRef<Field>(m_data[3]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::t(void) const
		requires (N >= 4)
	{
		return deRef<Field>(m_data[3]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::u(void)
		requires (N >= 5)
	{
		return deRef<Field>(m_data[4]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::u(void) const
		requires (N >= 5)
	{
		return deRef<Field>(m_data[4]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::v(void)
		requires (N >= 6)
	{
		return deRef<Field>(m_data[5]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::v(void) const
		requires (N >= 6)
	{
		return deRef<Field>(m_data[5]);
	}

	template <size_t N, class Field, class T>
	inline constexpr Field& VectorDataFixed<N, Field, T>::w(void)
		requires (N >= 7)
	{
		return deRef<Field>(m_data[6]);
	}
	template <size_t N, class Field, class T>
	inline constexpr const Field& VectorDataFixed<N, Field, T>::w(void) const
		requires (N >= 7)
	{
		return deRef<Field>(m_data[6]);
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y) requires (N == 2) :
		m_data{ x, y }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y, const T& z) requires (N == 3) :
		m_data{ x, y, z }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y, const T& z, const T& t) requires (N == 4) :
		m_data{ x, y, z, t }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u) requires (N == 5) :
		m_data{ x, y, z, t, u }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u, const T& v) requires (N == 6) :
		m_data{ x, y, z, t, u, v }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>::VectorDataFixed(const T& x, const T& y, const T& z, const T& t, const T& u, const T& v, const T& w) requires (N == 7) :
		m_data{ x, y, z, t, u, v, w }
	{
	}

	template <size_t N, class Field, class T>
	inline constexpr VectorDataFixed<N, Field, T>& VectorDataFixed<N, Field, T>::operator=(const VectorDataFixed& rhs)
	{
		std::copy(rhs.m_data, rhs.m_data + N, m_data);
		return *this;
	}

	template <size_t N, class Field, class T>
	template <size_t N2, class Field2, class T2>
	requires std::convertible_to<Field2, Field>
	inline constexpr VectorDataFixed<N, Field, T>& VectorDataFixed<N, Field, T>::operator=(const VectorDataFixed<N2, Field2, T2>& rhs)
	{
		auto it = this->begin();
		auto otherIt = rhs->begin();
		for (; it != this->end() && otherIt != rhs.end(); ++it, ++otherIt)
			*it = *otherIt;
		return *this;
	}

	template <size_t N, class Field, class T>
	template <size_t N2, class Field2, class T2>
	requires std::convertible_to<Field2, Field>
	inline constexpr VectorDataFixed<N, Field, T>& VectorDataFixed<N, Field, T>::operator=(VectorDataFixed<N2, Field2, T2>&& rhs)
	{
		return this->operator=((const VectorDataFixed<N2, Field2, T2>&)rhs);
	}
}
