#pragma once

#include <LC/math/linalg/Vector/Vector.hpp>
#include <LC/math/concepts/concepts.hpp>

#include "../MatImplementation.hpp"

namespace lc::priv
{
	template <int N, int M, class Field, class T>
	class FixedMatrixData :
		public Vector<N, Vector<M, Field, T>>
	{
		using THIS_CLASS = FixedMatrixData<N, M, Field, T>;
		template <int N2, int M2, class Field2, class T2>
		using FINAL_CLASS = MatrixImpl<Field2, FixedMatrixData<N2, M2, Field2, T2>>;
	public:

		using BASE = Vector<N, Vector<M, Field, T>>;
		//using ConcretePointer = std::conditional_t<std::is_same_v<_ConcretePointer, Itself>, THIS_CLASS>;

		// ================================
		//             INFO
		// ================================

		static constexpr int get_N(void) { return N; }
		static constexpr int get_M(void) { return M; }

		// inherit constructors
		using BASE::BASE;

		// Homogeneus matrix
		constexpr FixedMatrixData(Field x) :
			FixedMatrixData()
		{
			for (unsigned i = 0; i < MIN(N, M); i++)
				deRef<Field>((*this)[i][i]) = x;
		}

		// From data pointer
		constexpr FixedMatrixData(const T* data)
		{
			for (auto it = this->begin(); it != this->end(); it++)
				for (auto it2 = it->begin(); it2 != it->end(); it2++, data++)
					*it2 = *data;
		};

		// From matrix
		template <class T2, int N2, int M2, std::enable_if_t<std::convertible_to<T2, T>, bool> = true>
		constexpr FixedMatrixData(const T2(&mat)[N2][M2])
		{
			for (int i = 0; i < MIN(N, N2); i++)
				for (int j = 0; j < MIN(M, M2); j++)
					(*this)[i][j] = mat[i][j];
		}

/*#ifdef LC_ENABLE_CONCEPTS
		template <std::convertible_to<Field> T2>
#else
		template <class T2>
#endif
		constexpr FixedMatrixData(std::initializer_list<std::initializer_list<T2>> list)
		{
			//for ()
		}*/

		// ================================
		//            ACCESS
		// ================================

		// Number of rows
		static constexpr int rows(void) { return N; };

		// Number of columns
		static constexpr int cols(void) { return M; };

		// Number of elements
		static constexpr std::size_t size(void) { return THIS_CLASS::rows() * THIS_CLASS::cols(); }

		T& at(size_t i, size_t j) { return (*this)[i][j]; }
		const T& at(size_t i, size_t j) const { return (*this)[i][j]; }

		// ================================
		//			UTILITY
		// ================================

		auto getCopy(void) const
		{
			return (const FINAL_CLASS<N, M, Field, T>&)*this;
		}

		auto getEmptyCopy(void) const
		{
			return FINAL_CLASS<N, M, Field, T>();
		}

		auto getIdCopy(void) const
		{
			return FINAL_CLASS<N, M, Field, T>(Field(1));
		}

		// swap two rows
		constexpr FINAL_CLASS<N, M, Field, T>& swapRows(size_t i, size_t j)
		{
			std::swap((*this)[i], (*this)[j]);
			return *(FINAL_CLASS<N, M, Field, T>*)this;
		}

		// swap two columns
		constexpr FINAL_CLASS<N, M, Field, T>& swapColumns(size_t i, size_t j)
		{
			for (auto& r : *this)
				std::swap(r[i], r[j]);
			return *this;
		}

		// ================================
		//			MATH OPERATORS
		// ================================

		// return the transposed marix
		constexpr FINAL_CLASS<M, N, Field, T> transposed(void) const
		{
			FixedMatrixData<M, N, Field, Field> tmp;
			for (int i = 0; i < this->rows(); i++)
				for (int j = 0; j < this->cols(); j++)
					deRef<Field>(tmp[j][i]) = deRef<Field>((*this)[i][j]);
			return (FINAL_CLASS<M, N, Field, T>&)tmp;
		}

		// Transpose
		constexpr FINAL_CLASS<N, M, Field, T>& transpose(void) requires (N == M)
		{
			for (std::size_t i = 1; i < this->rows(); i++)
				for (std::size_t j = 0; j < i; j++)
					std::swap(deRef<Field>((*this)[i][j]), deRef<Field>((*this)[j][i]));
			return *(FINAL_CLASS<M, N, Field, T>*)this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Vector multiplication
		////////////////////////////////////////////////////////////////
		template <class Field2, class T2>
		constexpr Vector<N, decltype(Field()* Field2())> operator*(const Vector<M, Field2, T2>& right) const
		{
			using R = decltype(Field()* Field2());

			Vector<N, R> result;
			for (int i = 0; i < this->rows(); i++)
				result[i] = this->operator[](i) * right;
			return result;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix multiplication
		////////////////////////////////////////////////////////////////
		template <int M2, class Field2, class T2>
		constexpr FINAL_CLASS<N, M2, decltype(Field()* Field2()), decltype(Field()* Field2())> operator*(const FINAL_CLASS<M, M2, Field2, T2>& right) const
		{
			using R = decltype(Field() * Field2());
			FixedMatrixData<N, M2, R, R> result;
			for (int i = 0; i < this->rows(); i++)
				for (int j2 = 0; j2 < right.cols(); j2++)
					for (int i2 = 0; i2 < right.rows(); i2++)
						result[i][j2] += deRef<Field>((*this)[i][i2]) * deRef<Field2>(right[i2][j2]);
			return (const FINAL_CLASS<N, M2, decltype(Field()* Field2()), decltype(Field()* Field2())>&)result;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix Addition
		////////////////////////////////////////////////////////////////
		// TODO requires
		constexpr FINAL_CLASS<N, M, Field, T> operator+(THIS_CLASS right) const
		{
			for (int i = 0; i < N; i++)
				right[i] += (*this)[i];
			return (FINAL_CLASS<N, M, Field, T>&)right;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix subtraction
		////////////////////////////////////////////////////////////////
		// TODO requires
		constexpr FINAL_CLASS<N, M, Field, T> operator-(THIS_CLASS right) const
		{
			for (int i = 0; i < N; i++)
				right[i] -= (*this)[i];
			return (FINAL_CLASS<N, M, Field, T>&)right;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix multiplication assignment
		////////////////////////////////////////////////////////////////
		constexpr FINAL_CLASS<N, M, Field, T>& operator*=(const THIS_CLASS& right) requires (N == M)
		{
			return (*this) = (*this) * right;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix Addition
		////////////////////////////////////////////////////////////////
		// TODO requires
		constexpr FINAL_CLASS<N, M, Field, T>& operator+=(const THIS_CLASS& right)
		{
			return (*this) = (*this) + right;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Matrix-Matrix Addition
		////////////////////////////////////////////////////////////////
		// TODO requires
		constexpr FINAL_CLASS<N, M, Field, T>& operator-=(const THIS_CLASS& right)
		{
			return (*this) = (*this) - right;
		}
	};
}



