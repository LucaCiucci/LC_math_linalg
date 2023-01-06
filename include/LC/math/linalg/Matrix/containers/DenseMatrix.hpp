#pragma once


#include <vector>

#include "../MatImplementation.hpp"

#if __has_include(<cereal/cereal.hpp>)
	#include <cereal/cereal.hpp>
	#include <cereal/types/vector.hpp>
#endif

namespace lc::priv
{
	// ================================================================
	//             VECTOR - BEGIN-END ITERATOR - DATA
	// ================================================================

	// TODO sposta
	// This class represents a storage method that is used to build a vector based on a begin-end iterator
	// instead of a fixed, dense or similar storage methods
	template <class Iterator>
	class VectorBeginEndIteratorData
	{
	public:

		// ================================
		//         CONSTRUCTORS
		// ================================

		// creates a vector storage from a BEGIN and END iterator
		constexpr VectorBeginEndIteratorData(Iterator begin, Iterator end);

		// default copy constructor
		constexpr VectorBeginEndIteratorData(const VectorBeginEndIteratorData&) = default;

		// default move constructor
		constexpr VectorBeginEndIteratorData(VectorBeginEndIteratorData&&) = default;

		// ================================
		//            ACCESS
		// ================================

		// returns the vector size as the distance between end/begin iterators
		// the return type is "auto" as it could depend on the iterator type
		constexpr auto size(void) const;

		// ============ begin =============

		// begin iterator
		constexpr Iterator& begin(void);

		// begin iterator (const)
		constexpr const Iterator& begin(void) const;

		// begin iterator (const)
		constexpr const Iterator& cbegin(void) const;

		// ============= end ==============

		// end iterator
		constexpr Iterator& end(void);

		// end iterator (const)
		constexpr const Iterator& end(void) const;

		// end iterator (const)
		constexpr const Iterator& cend(void) const;

		// =========== indexing ===========

		// access an element from index, no bounds checks
		constexpr auto& operator[](size_t index);

		// access an element from index, no bounds checks, const
		constexpr const auto& operator[](size_t index) const;

		// access an element from index with bounds ckecs
		constexpr auto& at(size_t index);

		// access an element from index with bounds ckecs, const
		constexpr const auto& at(size_t index) const;

		// ??
		//VectorDataBeginEndIteratorData<Iterator>* operator->(void)
		//{
		//	return this;
		//}
		//
		//const VectorDataBeginEndIteratorData<Iterator>* operator->(void) const
		//{
		//	return this;
		//}

		// ================================
		//             DATA
		// ================================
	private:
		Iterator m_begin, m_end;
	};

	// ================================================================
	//                   VECTOR - BEGIN-END ITERATOR
	// ================================================================

	// this creates a vector from the VectorBeginEndIteratorData storage defined above
	template <class Field, class Iterator>
	using VectorBeginEndIterator = lc::VectorImpl<Field, VectorBeginEndIteratorData<Iterator>, false>;

	// ================================================================
	//                      DYNAMIC STRIDE ITERATOR
	// ================================================================
	/*
	// this class is used to create an iterator based on another iterator but with a stride
	template <class BaseIterator>
	class DynamicStrideIterator : public BaseIterator
	{
		using THIS_CLASS = DynamicStrideIterator<BaseIterator>;
	public:

		// ================================
		//          CONSTRUCTORS
		// ================================

		// default copy constructor
		constexpr DynamicStrideIterator(const DynamicStrideIterator&) = default;

		// default move constructor
		constexpr DynamicStrideIterator(DynamicStrideIterator&&) = default;

		// creates an iterator from the base iterator and the stride, "end" is the past-the-end iterator
		constexpr DynamicStrideIterator(const BaseIterator& it, const BaseIterator& end, size_t stride);

		// ================================
		//           OPERATORS
		// ================================

		// pre increment
		constexpr THIS_CLASS& operator++();

		// post increment
		constexpr THIS_CLASS operator++(int);

		// pre decrement
		constexpr THIS_CLASS& operator--();

		// post decrement
		constexpr THIS_CLASS operator--(int);

		// increment
		constexpr THIS_CLASS& operator+=(typename BaseIterator::difference_type n)
			requires requires (BaseIterator x, BaseIterator::difference_type n) { { x += n } -> std::same_as<BaseIterator&>; };

		// increment
		constexpr THIS_CLASS operator+(typename THIS_CLASS::difference_type n) const
			requires requires (BaseIterator x, BaseIterator::difference_type n) { { x + n } -> std::same_as<BaseIterator>; };

		// decrement
		constexpr THIS_CLASS& operator-=(typename BaseIterator::difference_type n)
			requires requires (BaseIterator x, BaseIterator::difference_type n) { { x -= n } -> std::same_as<BaseIterator&>; };

		// decrement
		constexpr THIS_CLASS operator-(typename THIS_CLASS::difference_type n) const
			requires requires (BaseIterator x, BaseIterator::difference_type n) { { x - n } -> std::same_as<BaseIterator>; };

		// difference
		constexpr typename THIS_CLASS::difference_type operator-(THIS_CLASS right) const;

		// indexing
		constexpr auto operator[](typename THIS_CLASS::difference_type n) const
			requires requires (BaseIterator x, BaseIterator::difference_type n, size_t stride) { { x[n * stride] }; };

		// ================================
		// ================================

	private:
		const size_t m_stride;
		const BaseIterator m_end;
	};

	template <class BaseIterator>
	constexpr DynamicStrideIterator<BaseIterator> operator+(typename BaseIterator::difference_type n, DynamicStrideIterator<BaseIterator> it) { return it + n; }
	*/

	// ================================================================
	//                   ITERATOR TO VECTOR--------------------
	// ================================================================

	template <class Field, class SubIterator>
	class IteratorToVectorDataBeginEndIterator
	{
	private:
		using THIS_CLASS = IteratorToVectorDataBeginEndIterator<Field, SubIterator>;
		SubIterator m_it; std::ptrdiff_t m_subSize;
	public:
		using iterator_category = std::bidirectional_iterator_tag;// TODO dipendente da SubIterator
		using value_type = VectorBeginEndIterator<Field, SubIterator>;
		using difference_type = size_t;
		using pointer = VectorBeginEndIterator<Field, SubIterator>;
		using reference = VectorBeginEndIterator<Field, SubIterator>;

		// ================================
		//          CONSTRUCTORS
		// ================================

		IteratorToVectorDataBeginEndIterator(const THIS_CLASS& it) = default;
		IteratorToVectorDataBeginEndIterator(THIS_CLASS&&) = default;
		IteratorToVectorDataBeginEndIterator(SubIterator vectorStart, difference_type subSize):
			m_it(vectorStart), m_subSize(subSize)
		{
		};

		// ================================
		//           OPERATORS
		// ================================

		constexpr THIS_CLASS& operator++() // prefix
		{
			m_it += m_subSize;
			return *this;
		}
		constexpr THIS_CLASS operator++(int) // postfix
		{
			THIS_CLASS tmp(*this);
			++(*this);
			return tmp;
		}
		constexpr THIS_CLASS& operator--()
		{
			m_it -= m_subSize;
			return *this;
		}
		constexpr THIS_CLASS operator--(int)
		{
			THIS_CLASS tmp(*this);
			return (*this)--;
		}
		constexpr THIS_CLASS& operator+=(typename THIS_CLASS::difference_type n)
		{
			m_it += m_subSize * n;
			return *this;
		}
		constexpr THIS_CLASS operator+(typename THIS_CLASS::difference_type n) const
		{
			THIS_CLASS tmp(*this);
			tmp += n;
			return tmp;
		}
		constexpr THIS_CLASS& operator-=(typename THIS_CLASS::difference_type n)
		{
			m_it += m_subSize * n;
			return *this;
		}
		constexpr THIS_CLASS operator-(typename THIS_CLASS::difference_type n) const
		{
			THIS_CLASS tmp(*this);
			tmp -= n;
			return tmp;
		}

		constexpr typename THIS_CLASS::difference_type operator-(THIS_CLASS right) const { return this->m_it - right.m_it; }
		//constexpr typename THIS_CLASS::reference operator[](THIS::difference_type i) const { return this->m_ptr[i]; }
		constexpr bool operator==(THIS_CLASS right) const { return this->m_it == right.m_it; }
		constexpr bool operator!=(THIS_CLASS right) const { return this->m_it != right.m_it; }
		constexpr bool operator>(THIS_CLASS right) const { return this->m_it > right.m_it; }
		constexpr bool operator>=(THIS_CLASS right) const { return this->m_it >= right.m_it; }
		constexpr bool operator<(THIS_CLASS right) const { return this->m_it < right.m_it; }
		constexpr bool operator<=(THIS_CLASS right) const { return this->m_it <= right.m_it; }
		explicit constexpr operator THIS_CLASS::reference() const
		{
			return {m_it, m_it + m_subSize};
		}
		VectorBeginEndIterator<Field, SubIterator> operator->() const
		{
			return VectorBeginEndIterator<Field, SubIterator>(m_it, m_it + m_subSize);
		}
		VectorBeginEndIterator<Field, SubIterator> operator*() const
		{
			return VectorBeginEndIterator<Field, SubIterator>(m_it, m_it + m_subSize);
		}
	};
	template <class Field, class SubIterator>
	constexpr IteratorToVectorDataBeginEndIterator<Field, SubIterator> operator+(typename IteratorToVectorDataBeginEndIterator<Field, SubIterator>::difference_type n, IteratorToVectorDataBeginEndIterator<Field, SubIterator> it) { return it + n; }

	////////////////////////////////////////////////////////////////
	template <class Field>
	class DenseMatrixData
	{
		using THIS_CLASS = DenseMatrixData<Field>;
		template <class Field2>
		using FINAL_CLASS = lc::MatrixImpl<Field2, DenseMatrixData<Field2>>;
	public:

		// ================================
		//          CONSTRUCTORS
		// ================================

		// default copy constructor
		DenseMatrixData(const THIS_CLASS&) = default;

		// default move constructor
		DenseMatrixData(THIS_CLASS&&) = default;

		// create an empty matrix
		DenseMatrixData(size_t _N, size_t _M) :
			N(_N), M(_M), m_data(_N * _M)
		{
		}

		// creates a metrix filled with a value
		DenseMatrixData(size_t _N, size_t _M, Field c) :
			DenseMatrixData(_N, _M)
		{
			for (size_t i = 0; i < std::min(N, M); i++)
				this->at(i, i) = c;
		}

		DenseMatrixData& operator=(const THIS_CLASS&) = default;
		DenseMatrixData& operator=(THIS_CLASS&&) = default;

		auto rows(void) const { return N; }
		auto cols(void) const { return M; }
		auto size(void) const { return m_data.size(); }

		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::iterator> begin(void) { return { m_data.begin(), this->cols() }; }
		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::const_iterator> begin(void) const { return { m_data.begin(), this->cols() }; }
		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::const_iterator> cbegin(void) { return { m_data.begin(), this->cols() }; }

		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::iterator> end(void) { return { m_data.end(), this->cols() }; }
		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::const_iterator> end(void) const { return { m_data.end(), this->cols() }; }
		IteratorToVectorDataBeginEndIterator<Field, typename std::vector<Field>::const_iterator> cend(void) { return { m_data.end(), this->cols() }; }

		VectorBeginEndIterator<Field, typename std::vector<Field>::iterator> operator[](size_t index)
		{
			if ((index + 1) * M > m_data.size())
				return { m_data.begin() + index * M, m_data.end() };
			return { m_data.begin() + index * M, (m_data.begin()) + (index + 1) * M };
		}
		VectorBeginEndIterator<Field, typename std::vector<Field>::const_iterator> operator[](size_t index) const
		{
			if ((index + 1) * M > m_data.size())
				return { m_data.begin() + index * M, m_data.end() };
			return { m_data.begin() + index * M, (m_data.begin()) + (index + 1) * M };
		}

		Field& at(size_t i, size_t j) { return m_data[i * M + j]; }
		const Field& at(size_t i, size_t j) const { return m_data[i * M + j]; }

		FINAL_CLASS<Field> getCopy(void) const
		{
			return (const FINAL_CLASS<Field>&)*this;
		}

		FINAL_CLASS<Field> getEmptyCopy(void) const
		{
			return FINAL_CLASS<Field>(N, M);
		}

		FINAL_CLASS<Field> getIdCopy(void) const
		{
			return FINAL_CLASS<Field>(N, M, Field(1));
		}

		constexpr FINAL_CLASS<Field>& swapRows(size_t i, size_t j)
		{
			auto row1 = (*this)[i];
			auto row2 = (*this)[j];
			auto it1 = row1.begin();
			auto it2 = row2.begin();
			for (; it1 != row1.end() && it2 != row2.end(); it1++, it2++)
				std::swap(*it1, *it2);
			return *(FINAL_CLASS<Field>*)this;
		}

		constexpr FINAL_CLASS<Field>& swapColumns(size_t i, size_t j)
		{
			for (size_t t = 0; t < this->rows(); t++)
				std::swap(this->at(t, i), this->at(t, j));
			return *(FINAL_CLASS<Field>*)this;
		}

		constexpr FINAL_CLASS<Field> transposed(void) const
		{
			THIS_CLASS result(this->cols(), this->rows());
			for (size_t i = 0; i < this->rows(); i++)
				for (size_t j = 0; j < this->cols(); j++)
					result.at(j, i) = this->at(i, j);
			return (FINAL_CLASS<Field>&)result;
		}

		constexpr FINAL_CLASS<Field>& transpose(void)
		{
			*this = this->transposed();
			return *(FINAL_CLASS<Field>*)this;
		}

		template <class Field2, class T2>
		constexpr DenseVector<decltype(Field()* Field2())> operator*(const DenseVector<Field2, T2>& right) const
		{
			if (this->cols() != right.size())
				throw std::runtime_error((std::string)"In Mat-Vec multiplication this->cols() must be equal to the vector size but they were " + std::to_string(this->cols()) + ", " + std::to_string(right.size()));

			using R = decltype(Field()* Field2());

			DenseVector<R> result(this->rows());
			for (size_t i = 0; i < this->rows(); i++)
				for (size_t j = 0; j < this->cols(); j++)
				result[i] += this->operator[](i)[j] * deRef<Field2>(right[j]);
			return result;
		}

		constexpr FINAL_CLASS<Field> operator*(const DenseMatrixData<Field>& right) const
		{
			if (this->cols() != right.rows())
				throw std::runtime_error((std::string)"In Mat-Mat multiplication this->cols() must be equal to the vector size but they were " + std::to_string(this->cols()) + ", " + std::to_string(right.rows()));

			DenseMatrixData<Field> result(this->rows(), right.cols());
			for (size_t i = 0; i < this->rows(); i++)
				for (size_t j = 0; j < right.cols(); j++)
					for (size_t k = 0; k < this->cols(); k++)
						//result[i][j] += deRef<Field>((*this)[i][k]) * deRef<Field2>(right[k][j]);
						//result[i][j] += (*this)[i][k] * right[k][j];
						result.at(i, j) += (*this).at(i, k) * right.at(k, j);
			return (FINAL_CLASS<Field>&)(result);
		}

		constexpr FINAL_CLASS<Field> operator+(THIS_CLASS right) const
		{
			if (this->rows() != right.rows() || this->cols() != right.cols())
				throw std::runtime_error("In Mat-Mat addidtion the two matrix must have the same size");

			for (size_t i = 0; i < this->rows(); i++)
				for (size_t j = 0; j < this->cols(); j++)
					right[i][j] += (*this)[i][j];
			return (FINAL_CLASS<Field>&)right;
		}

		constexpr FINAL_CLASS<Field> operator-(THIS_CLASS right) const
		{
			if (this->rows() != right.rows() || this->cols() != right.cols())
				throw std::runtime_error("In Mat-Mat subtraction the two matrix must have the same size");

			for (size_t i = 0; i < this->rows(); i++)
				for (size_t j = 0; j < this->cols(); j++)
					right[i][j] -= (*this)[i][j];
			return (FINAL_CLASS<Field>&)right;
		}

		constexpr FINAL_CLASS<Field>& operator*=(const THIS_CLASS& right)
		{
			return (*this) = (*this) * right;
		}

		constexpr FINAL_CLASS<Field>& operator+=(const THIS_CLASS& right)
		{
			return (*this) = (*this) + right;
		}

		constexpr FINAL_CLASS<Field>& operator-=(const THIS_CLASS& right)
		{
			return (*this) = (*this) - right;
		}

		/*auto tmp_col(void)
		{
			using IT = DynamicStrideIterator<std::vector<Field>::iterator>;
			using V = lc::VectorImpl<Field, VectorBeginEndIteratorData<IT>>;

			auto end = m_data.begin() + this->cols() * (this->rows() - 1) + 1;
			IT b = IT(m_data.begin(), end, this->cols());

			//b - 1;

			return V(b, b + this->rows());
		}*/

	private:
	public:// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		size_t N, M;
		std::vector<Field> m_data;

#if __has_include(<cereal/cereal.hpp>)
		// !!!
		template <class Archive, class Field>
		friend void serialize(Archive& archive, DenseMatrixData<Field>& M, const Uint32 version);
#endif
	};


	/*
	// ================================================================
	//                       SUB MATRIX REFERENCE
	// ================================================================

	template <class Field>
	class SubMatrixData
	{
	public:
		std::vector<size_t> IndexVector;
		std::vector<bool> ValidIndexVector;
	public:

		// ================================
		//         CONSTRUCTORS
		// ================================

		// default copy constructor
		SubMatrixData(const SubMatrixData&) = default;

		// default move constructor
		SubMatrixData(SubMatrixData&&) = default;

		template <class Matrix>
		SubMatrixData(const IndexVector& i_indexes, const IndexVector& j_indexes)

	private:
		
	};
	*/

#if __has_include(<cereal/cereal.hpp>)
	template <class Archive, class Field>
	void serialize(Archive& archive, DenseMatrixData<Field>& M, const uint32_t version)
	{
		if (version == 0)
		{
			archive(cereal::make_nvp("N", M.N));
			archive(cereal::make_nvp("M", M.M));

			// TODO remove size in future versions
			// m_data saves its size but it is redundant since we know it is N*M
			archive(cereal::make_nvp("data", M.m_data));

			return;
		}

		// TODO a decent exception, not an assert
		assert(0);
	}
#endif
}

#if __has_include(<cereal/cereal.hpp>)
// see https ://github.com/USCiLab/cereal/issues/319
// CEREAL_CLASS_VERSION(Vector, 0);
namespace cereal {
	namespace detail {
		template <class Field>
		struct Version<::lc::priv::DenseMatrixData<Field>>
		{
			static const std::uint32_t version;
			static std::uint32_t registerVersion()
			{
				constexpr std::uint32_t v = 0; // VERSION!
				::cereal::detail::StaticObject<Versions>::getInstance().mapping.emplace(
					std::type_index(typeid(::lc::priv::DenseMatrixData<Field>)).hash_code(), v);
				return v;
			}
			static void unused() { (void)version; }
		}; /* end Version */
		template <class Field>
		const std::uint32_t Version<::lc::priv::DenseMatrixData<Field>>::version =
			Version<::lc::priv::DenseMatrixData<Field>>::registerVersion();
	}
} // end namespaces
#endif
