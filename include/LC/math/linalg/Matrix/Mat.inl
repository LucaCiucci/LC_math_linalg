
// #include "Mat"
#include <exception>

#include <LC/utils/strings/strings.hpp>
//#include "Mat"
#include <iostream>

namespace lc::math::geometry
{

#define THIS_CLASS Mat<N, M, T>
	// ================================================================
	//                             MAT
	// ================================================================

	// ================================
	//         CONSTRUCTORS
	// ================================

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>::Mat(ConstructType type) :
		Mat()
	{
		ConstructType t;
		switch (t)
		{
		case ConstructType::Empty:
			break;
		case ConstructType::Id:
			for (unsigned i = 0; i < N && i < M; i++)
				(*this)[i][i] = T(1);
			break;
		default:
			break;
		}
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>::Mat(T x) :
		Mat()
	{
		for (unsigned i = 0; i < N && i < M; i++)
			(*this)[i][i] = x;
	}

#ifdef LC_ENABLE_CONCEPTS
	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	template <class T2, std::enable_if_t<std::convertible_to<T2, T>, bool>>
	inline constexpr Mat<N, M, T>::Mat(const T2* data)
	{
		for (auto it = this->begin(); it != this->end(); it++)
			for (auto it2 = it->begin(); it2 != it->end(); it2++, data++)
				*it2 = *data;
	}
#endif

#ifdef LC_ENABLE_CONCEPTS
	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>::Mat(const T* data)
	{
		for (auto it = this->begin(); it != this->end(); it++)
			for (auto it2 = it->begin(); it2 != it->end(); it2++, data++)
				*it2 = *data;
	}
#endif

#ifdef LC_ENABLE_CONCEPTS
	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
#ifdef LC_ENABLE_CONCEPTS
	template <class T2, int N2, int M2, std::enable_if_t<std::convertible_to<T2, T>, bool>>
#else
	template <class T2, int N2, int M2>
#endif
	inline constexpr Mat<N, M, T>::Mat(const T2(&mat)[N2][M2]) :
		Mat()
	{
		std::cout << N2 << "e" << M2 << std::endl;
		for (int i = 0; i < MIN(N, N2); i++)
			for (int j = 0; j < MIN(M, M2); j++)
				(*this)[i][j] = mat[i][j];
	}
#endif


	// ================================
	//			UTILITY
	// ================================

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::swapRows(size_t i, size_t j)
	{
		std::swap((*this)[i], (*this)[j]);
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::swapColumns(size_t i, size_t j)
	{
		for (auto& r : *this)
			std::swap(r[i], r[j]);
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::setAllElementsTo(const T& c)
	{
		for (const auto& r : *this)
			r = c;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::toRowEcheleonForm(void)
	{
		return this->toRowEcheleonForm_impl<false, false>();
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	template <class Indexable>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::toRowEcheleonForm(Indexable& right)
	{
		return this->toRowEcheleonForm_impl<false, true>(&right);
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::toReducedRowEcheleonForm(void)
	{
		return this->toRowEcheleonForm_impl<true, false>();
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	template <class Indexable>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::toReducedRowEcheleonForm(Indexable& right)
	{
		return this->toRowEcheleonForm_impl<true, true>(&right);
	}


	// ================================
	//			MATH OPERATORS
	// ================================

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<M, N, T> Mat<N, M, T>::transposed(void) const
	{
		Mat<M, N, T> tmp;
		for (int i = 0; i < this->rows(); i++)
			for (int j = 0; j < this->cols(); j++)
				tmp[j][i] = (*this)[i][j];
		return tmp;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline constexpr Mat<N, M, T> Mat<N, M, T>::operator*(T right) const
	{
		Mat<N, M, T> result;
		for (int i = 0; i < N; i++)
			result[i] = (*this)[i] * right;
		return result;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T> Mat<N, M, T>::operator+(const Mat<N, M, T>& right) const
	{
		auto result = *this;
		(BASE&)result += (BASE&)right;
		return result;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T> Mat<N, M, T>::operator-(const Mat<N, M, T>& right) const
	{
		auto result = *this;
		(BASE&)result -= (BASE&)right;
		return result;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline constexpr Vector<N, T> Mat<N, M, T>::operator*(const Vector<N, T>& right) const
	{
		Vector<N, T> result;
		for (int i = 0; i < this->rows(); i++)
			result[i] = this->operator[](i) * right;
		return result;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	template<int M2>
	inline constexpr Mat<N, M2, T> Mat<N, M, T>::operator*(const Mat<M, M2, T>& right) const
	{
		Mat<N, M2, T> result;
		for (int i = 0; i < this->rows(); i++)
			for (int j2 = 0; j2 < right.cols(); j2++)
				for (int i2 = 0; i2 < right.rows(); i2++)
					result[i][j2] += (*this)[i][i2] * right[i2][j2];
		return result;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T> Mat<N, M, T>::operator+=(const Mat<N, M, T>& right)
	{
		*((BASE*)this) += right;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template<int N, int M, class T>
	inline constexpr Mat<N, M, T> Mat<N, M, T>::operator-=(const Mat<N, M, T>& right)
	{
		*((BASE*)this) -= right;
		return *this;
	}


	// ================================
	//      ROWS/COLS OPERATIONS
	// ================================

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::sumRows(size_t first, size_t second)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > N || second > N)
			throw std::runtime_error("Mat::sumRows() 'first' or 'second' out of range");
#endif
		(*this)[first] += (*this)[second];
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::sumRows(size_t first, size_t second, T coeff)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > N || second > N)
			throw std::runtime_error("Mat::sumRows() 'first' or 'second' out of range");
#endif
		(*this)[first] += (*this)[second] * coeff;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::subtractRows(size_t first, size_t second)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > N || second > N)
			throw std::runtime_error("Mat::subtractRows() 'first' or 'second' out of range");
#endif
		(*this)[first] -= (*this)[second];
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::subtractRows(size_t first, size_t second, T coeff)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > N || second > N)
			throw std::runtime_error("Mat::subtractRows() 'first' or 'second' out of range");
#endif
		(*this)[first] -= (*this)[second] * coeff;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::sumColumns(size_t first, size_t second)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > M || second > M)
			throw std::runtime_error("Mat::sumColumns() 'first' or 'second' out of range");
#endif
		for (std::size_t i = 0; i < N; i++)
			(*this)[i][first] += (*this)[i][second];
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::sumColumns(size_t first, size_t second, T coeff)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > M || second > M)
			throw std::runtime_error("Mat::sumColumns() 'first' or 'second' out of range");
#endif
		for (std::size_t i = 0; i < N; i++)
			(*this)[i][first] += (*this)[i][second] * coeff;
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::subtractColumns(size_t first, size_t second)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > M || second > M)
			throw std::runtime_error("Mat::subtractColumns() 'first' or 'second' out of range");
#endif
		for (std::size_t i = 0; i < N; i++)
			(*this)[i][first] -= (*this)[i][second];
		return *this;
	}

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	inline Mat<N, M, T>& Mat<N, M, T>::subtractColumns(size_t first, size_t second, T coeff)
	{
#ifdef LC_VECTOR_CHECKS
		if (first > M || second > M)
			throw std::runtime_error("Mat::subtractColumns() 'first' or 'second' out of range");
#endif
		for (std::size_t i = 0; i < N; i++)
			(*this)[i][first] -= (*this)[i][second] * coeff;
		return *this;
	}


	// ================================
	//            PRIVATE
	// ================================

	////////////////////////////////////////////////////////////////
	template <int N, int M, class T>
	template <bool ReducedForm, bool RightVector, class Indexable>
	inline constexpr Mat<N, M, T>& Mat<N, M, T>::toRowEcheleonForm_impl(Indexable* right)
	{
		// ■ □
		/* we want to use only row moves to reduce to echleon form:
		/ x x x x x x x x x x \        / ■ x x x x x x x x x \
		| x x x x x x x x x x |        |   ■ x x x x x x x x |
		| x x x x x x x x x x |   ->   |         ■ x x x x x |
		| x x x x x x x x x x |        |           ■ x x x x |
		\ x x x x x x x x x x /        \               ■ x x /
		*/

		// we will find the pivot one row at a time: 'pivot_row' will keep track of witch row we are searching in:
		/*
		             / ■ x x x x x x x x x \
		             |   ■ x x x x x x x x |
		pivot_row -> |         x x x x x x | <- we want to place a pivot in this row
		             |         x x x x x x |
		             \         x x x x x x /
		*/
		std::size_t pivot_row = 0;


		for (std::size_t col_index = 0; col_index < this->cols() && pivot_row < this->rows(); col_index++)
		{
			/*
			we are runningh trough every column and we try to find a possible pivot.
			If we find a pivot, then we place it into position and we go to the next row and column
			          col_index
			             v
			   / ■ x x x x x x x x x \
			   |   ■ x x x x x x x x |
			-> |         x x x x x x | <- we want to place a pivot in this row
			   |         x x x x x x |
			   \         x x x x x x /
			*/

			// we choose to use the biggest value as ppivot, so we need to find the max value
			auto max_v = std::abs((*this)[pivot_row][col_index]);
			std::size_t max_i = pivot_row;
			for (std::size_t i = pivot_row; i < this->rows(); i++) // <- canditate pivots can only be searched under 'pivot_row'
			{
				auto tmp = std::abs((*this)[i][col_index]);
				if (tmp > max_v)
				{
					max_v = tmp;
					max_i = i;
				}
			}

			/*           v
			   / □ x x x x x x x x x \
			   |   □ x x x x x x x x |
			-> |         x x x x x x | <- we want to place a pivot in this row
			   |         ■ x x x x x | <- we found a candidate pivot at 'max_i'
			   \         x x x x x x /
			*/

			// if the column is empty, nothing to do
			if (max_v == 0)
				continue;

			/* 
			now we need to place the pivot in the right place
			            v                                         v
			  / □ x x x x x x x x x \                   / □ x x x x x x x x x \
			  |   □ x x x x x x x x |           ⥮       |   □ x x x x x x x x |
			> |         x x x x x x | <-|     ---->    >|         ■ x x x x x |
			  |         ■ x x x x x | <-|               |         x x x x x x |
			  \         x x x x x x /                   \         x x x x x x /
			*/
			if (max_i != pivot_row)
			{
				this->swapRows(max_i, pivot_row);

				if constexpr (RightVector)
					std::swap((*right)[max_i], (*right)[pivot_row]);
			}
			//std::cout << *this << std::endl;

			/*
			now we need eliminate the underlying values
			             v                             v
			   / □ x x x x x x x x x \       / □ x x x x x x x x x \
			   |   □ x x x x x x x x |       |   □ x x x x x x x x |
			-> |         ■ x x x x x |  ->  >|         ■ x x x x x |
			   |         x x x x x x |       |           x x x x x |
			   \         x x x x x x /       \           x x x x x /
			*/
			T pivot = (*this)[pivot_row][col_index];
			for (std::size_t row_index = ReducedForm ? 0 : (pivot_row + 1); row_index < this->rows(); row_index++)
			{
				if constexpr (ReducedForm)
					if (row_index == pivot_row)
						continue;
				T factor = this->operator[](row_index)[col_index] / pivot;
				this->subtractRows(row_index, pivot_row, factor);
				this->operator[](row_index)[col_index] = T();// force the underlying values to be filled with 0s

				if constexpr (RightVector)
					(*right)[row_index] -= (*right)[pivot_row] * factor;
			}
			//std::cout << *this << std::endl;

			// if we want the reduced form, we need to force the current pivot to 1
			if constexpr (ReducedForm)
			{
				this->operator[](pivot_row) /= pivot;
				this->operator[](pivot_row)[col_index] = 1;

				if constexpr (RightVector)
					(*right)[pivot_row] /= pivot;
			}
			//std::cout << *this << std::endl;

			// pass to the next row
			pivot_row++;
		}

		return *this;
	}
}

////////////////////////////////////////////////////////////////
template <class Mat, std::enable_if_t<lc::math::geometry::__details__::MyMatrixType<Mat>, bool>>
inline std::ostream& operator<<(std::ostream& stream, const Mat& mat)
{
	namespace table = lc::utils::strings::unicode::UnicodeTable;

	constexpr auto N = mat.get_N();
	constexpr auto M = mat.get_M();

	std::vector<std::string> numbers;
	for (std::size_t i = 0; i < N; i++)
		//numbers.push_back(std::to_string(mat[i][j]));
		for (std::size_t j = 0; j < M; j++)
		{
			std::stringstream ss;
			ss << mat[i][j];
			numbers.push_back(ss.str());
		}

	std::size_t max_l = 0;
	for (const auto& s : numbers)
		max_l = std::max(max_l, s.length());

	for (auto& s : numbers)
	{
		auto sLen = s.size();

		std::string blank;
		for (std::size_t i = 0; i < (max_l - sLen); i++)
			blank += ' ';

		s = blank + s;
	}

	auto it = numbers.begin();

	// TODO cambia con un qualcosa unicode
	if (N > 1)
		//stream << "/ ";
		stream << table::lines::upperLeft << " ";
	for (int i = 0; i < N; i++)
	{
		if (i > 0 && i < N - 1)
			//stream << "| ";
			stream << table::lines::vertical << " ";
		if (N == 1)
			stream << "(";
		if (i == N - 1 && N > 1)
			//stream << "\\ ";
			stream << table::lines::lowerLeft << " ";
		for (int j = 0; j < M; j++)
		{
			//stream << mat[i][j];
			stream << *it++;
			if (j < M - 1)
				stream << ", ";
		}
		if (i == 0 && N > 1)
			//stream << " \\";
			stream << " " << table::lines::upperRight;
		if (i > 0 && i < N - 1)
			//stream << " |";
			stream << " " << table::lines::vertical;
		if (N == 1)
			stream << ")";
		if (i == N - 1 && N > 1)
			//stream << " /";
			stream << " " << table::lines::lowerRight;
		if (i < N - 1)
			stream << std::endl;
	}
	return stream;
}

#undef THIS_CLASS
