#pragma once

#include <LC/math/linalg/Vector/Vector.hpp>

//#include "containers/containers.hpp"
//#include <LC/utils/strings/strings.hpp>

namespace lc
{
	template <class Field, class Container>
	class MatrixImpl :
		public Container
	{
		using BASE = Container;
		using THIS_CLASS = MatrixImpl<Field, Container>;

	public:

		////////////////////////////////////////////////////////////////
		// inherit constructors
		////////////////////////////////////////////////////////////////
		using BASE::BASE;
		//using BASE::operator=;

		// ================================
		//			UTILITY
		// ================================

		////////////////////////////////////////////////////////////////
		/// \brief set all elements to c
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS& setAllElementsTo(const Field& c)
		{
			for (auto& r : *this)
				for (auto& x : r)
					deRef<Field>(x) = c;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief To Echeleon form
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS& toRowEcheleonForm(void)
		{
			return this->toRowEcheleonForm_impl<false, false>();
		}

		// TODO_MINOR requires scalar
		////////////////////////////////////////////////////////////////
		/// \brief To Echeleon form
		////////////////////////////////////////////////////////////////
		template <class Indexable>// TODO_MINOR require Indexable indexable and []-> VectorialType
		constexpr THIS_CLASS& toRowEcheleonForm(Indexable& right)
		{
			return this->toRowEcheleonForm_impl<false, true>(&right);
		}

		////////////////////////////////////////////////////////////////
		/// \brief To Echeleon form
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS& toReducedRowEcheleonForm(void)
		{
			return this->toRowEcheleonForm_impl<true, false>();
		}

		////////////////////////////////////////////////////////////////
		/// \brief To Echeleon form
		////////////////////////////////////////////////////////////////
		template <class Indexable>// TODO_MINOR require Indexable indexable and []-> VectorialType
		constexpr THIS_CLASS& toReducedRowEcheleonForm(Indexable& right)
		{
			return this->toRowEcheleonForm_impl<true, true>(&right);
		}

		// ================================
		//			MATH OPERATORS
		// ================================

		using BASE::operator*;
		////////////////////////////////////////////////////////////////
		/// \brief Scaling
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS operator*(Field other) const
		{
			auto tmp = *this;
			/*for (auto& r : tmp)
				for (auto& x : r)
					deRef<Field>(x) *= other;*/
			for (int i = 0; i < this->rows(); i++)
				for (int j = 0; j < this->cols(); j++)
					deRef<Field>(tmp.at(i, j)) *= other;
			return tmp;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Scaling
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS operator/(Field other) const
		{
			auto tmp = *this;
			for (auto& r : tmp)
				for (auto& x : r)
					deRef<Field>(x) /= other;
			return tmp;
		}

		////////////////////////////////////////////////////////////////
		/// \brief 5e
		////////////////////////////////////////////////////////////////
		constexpr THIS_CLASS inverse(int newtonIterations = 0)
		{
			if (this->rows() != this->cols())
				throw std::runtime_error((std::string)"cannot invert a non-square matrix: " + std::to_string(this->rows()) + "x" + std::to_string(this->cols()));

			//THIS_CLASS left = this->getCopy();
			//THIS_CLASS right = this->getIdCopy();
			THIS_CLASS left = this->getCopy();// ???????????????
			THIS_CLASS right = this->getIdCopy();// ???????????????
			left.toReducedRowEcheleonForm(right);
			for (int i = 0; i < newtonIterations; i++)
				right = (right * 2 - right * ((*this) * right));
			return right;// TODO controlli ed eventualmente ritorno condizionale
		}

		// ================================
		//      ROWS/COLS OPERATIONS
		// ================================

		////////////////////////////////////////////////////////////////
		/// \brief Add to one row another one
		////////////////////////////////////////////////////////////////
		THIS_CLASS& sumRows(size_t first, size_t second)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->rows() || second >= this->rows())
				throw std::runtime_error("Mat::sumRows() 'first' or 'second' out of range");
#endif
			for (size_t j = 0; j < this->cols(); j++)
				deRef<Field>((*this)[first][j]) += deRef<Field>((*this)[second][j]);
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Add to one row a scalar multiple of another
		////////////////////////////////////////////////////////////////
		THIS_CLASS& sumRows(size_t first, size_t second, Field coeff)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->rows() || second >= this->rows())
				throw std::runtime_error("Mat::sumRows() 'first' or 'second' out of range");
#endif
			for (size_t j = 0; j < this->cols(); j++)
				deRef<Field>((*this)[first][j]) += deRef<Field>((*this)[second][j]) * coeff;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Subtract to one row another one
		////////////////////////////////////////////////////////////////
		THIS_CLASS& subtractRows(size_t first, size_t second)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->rows() || second >= this->rows())
				throw std::runtime_error("Mat::subtractRows() 'first' or 'second' out of range");
#endif
			for (size_t j = 0; j < this->cols(); j++)
				deRef<Field>((*this)[first][j]) -= deRef<Field>((*this)[second][j]);
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Subtract to one row a scalar multiple of another
		////////////////////////////////////////////////////////////////
		THIS_CLASS& subtractRows(size_t first, size_t second, Field coeff)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->rows() || second >= this->rows())
				throw std::runtime_error("Mat::subtractRows() 'first' or 'second' out of range");
#endif
			for (size_t j = 0; j < this->cols(); j++)
				deRef<Field>((*this)[first][j]) -= deRef<Field>((*this)[second][j]) * coeff;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Add to one column another one
		////////////////////////////////////////////////////////////////
		THIS_CLASS& sumColumns(size_t first, size_t second)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->cols() || second >= this->cols())
				throw std::runtime_error("Mat::sumColumns() 'first' or 'second' out of range");
#endif
			for (std::size_t i = 0; i < this->rows(); i++)
				deRef<Field>((*this)[i][first]) += deRef<Field>((*this)[i][second]);
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Add to one column a scalar multiple of another
		////////////////////////////////////////////////////////////////
		THIS_CLASS& sumColumns(size_t first, size_t second, Field coeff)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->cols() || second >= this->cols())
				throw std::runtime_error("Mat::sumColumns() 'first' or 'second' out of range");
#endif
			for (std::size_t i = 0; i < this->rows(); i++)
				deRef<Field>((*this)[i][first]) += deRef<Field>((*this)[i][second]) * coeff;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Subtract to one column another one
		////////////////////////////////////////////////////////////////
		THIS_CLASS& subtractColumns(size_t first, size_t second)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->cols() || second >= this->cols())
				throw std::runtime_error("Mat::subtractColumns() 'first' or 'second' out of range");
#endif
			for (std::size_t i = 0; i < this->rows(); i++)
				deRef<Field>((*this)[i][first]) -= deRef<Field>((*this)[i][second]);
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Subtract to one column a scalar multiple of another
		////////////////////////////////////////////////////////////////
		THIS_CLASS& subtractColumns(size_t first, size_t second, Field coeff)
		{
#ifdef LC_VECTOR_CHECKS
			if (first >= this->cols() || second >= this->cols())
				throw std::runtime_error("Mat::subtractColumns() 'first' or 'second' out of range");
#endif
			for (std::size_t i = 0; i < this->rows(); i++)
				deRef<Field>((*this)[i][first]) -= deRef<Field>((*this)[i][second]) * coeff;
			return *this;
		}

	private:

		////////////////////////////////////////////////////////////////
		/// \brief echleon form algorithm implementation
		////////////////////////////////////////////////////////////////
		template <bool ReducedForm, bool RightVector, class Indexable = void>
		constexpr THIS_CLASS& toRowEcheleonForm_impl(Indexable* right = static_cast<Indexable*>(nullptr))
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
				//auto max_v = std::abs(deRef<Field>((*this)[pivot_row][col_index]));
				auto max_v = std::abs(deRef<Field>(this->at(pivot_row, pivot_row)));
				std::size_t max_i = pivot_row;
				for (std::size_t i = pivot_row; i < this->rows(); i++) // <- canditate pivots can only be searched under 'pivot_row'
				{
					//auto tmp = std::abs(deRef<Field>((*this)[i][col_index]));
					auto tmp = std::abs(deRef<Field>(this->at(i, col_index)));
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
						//std::swap(deRef<Field>((*right)[max_i]), deRef<Field>((*right)[pivot_row]));
						//std::swap((*right)[max_i], (*right)[pivot_row]);
						right->swapRows(max_i, pivot_row);
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
				Field pivot = deRef<Field>((*this)[pivot_row][col_index]);
				for (std::size_t row_index = ReducedForm ? 0 : (pivot_row + 1); row_index < this->rows(); row_index++)
				{
					if constexpr (ReducedForm)
						if (row_index == pivot_row)
							continue;
					//Field factor = deRef<Field>(this->operator[](row_index)[col_index]) / pivot;
					Field factor = deRef<Field>(this->at(row_index, col_index)) / pivot;
					this->subtractRows(row_index, pivot_row, factor);
					//deRef<Field>(this->operator[](row_index)[col_index]) = Field();// force the underlying values to be filled with 0s
					deRef<Field>(this->at(row_index, col_index)) = Field();// force the underlying values to be filled with 0s

					if constexpr (RightVector)
						//deRef<Field>((*right)[row_index]) -= deRef<Field>((*right)[pivot_row] * factor);
						//(*right)[row_index] -= (*right)[pivot_row] * factor;
						right->subtractRows(row_index, pivot_row, factor);
				}
				//std::cout << *this << std::endl;

				// if we want the reduced form, we need to force the current pivot to 1
				if constexpr (ReducedForm)
				{
					//deRef<Field>(this->operator[](pivot_row)) /= pivot;
					//deRef<Field>(this->operator[](pivot_row)[col_index]) = Field(1);
					
					//this->operator[](pivot_row) /= pivot;
					for (int i = 0; i < this->cols(); i++)
						this->at(pivot_row, i) /= pivot;
					//this->operator[](pivot_row)[col_index] = Field(1);
					this->at(pivot_row, col_index) = Field(1);

					if constexpr (RightVector)
						//deRef<Field>((*right)[pivot_row]) /= pivot;
						//(*right)[pivot_row] /= pivot;
						for (int i = 0; i < right->cols(); i++)
							right->at(pivot_row, i) /= pivot;
				}
				//std::cout << *this << std::endl;

				// pass to the next row
				pivot_row++;
			}

			return *this;
		}
	};
}

////////////////////////////////////////////////////////////////
template <class Field, class Container>
inline std::ostream& operator<<(std::ostream& stream, const lc::MatrixImpl<Field, Container>& mat)
{

// #pragma warning( push )
// #pragma warning( disable : 26498 )// "use constexpr" warning
	const auto N = mat.rows();
	const auto M = mat.cols();
// #pragma warning( pop )

	std::vector<std::string> numbers;
	for (std::size_t i = 0; i < mat.rows(); i++)
		//numbers.push_back(std::to_string(mat[i][j]));
		for (std::size_t j = 0; j < mat.cols(); j++)
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
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			//stream << mat[i][j];
			stream << *it++;
			if (j < M - 1)
				stream << ", ";
		}
		if (i < N - 1)
			stream << std::endl;
	}
	return stream;
}
