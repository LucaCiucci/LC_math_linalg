#pragma once

#include "containers/containers.hpp"
//#include "MatImplementation.hpp"

namespace lc
{
	using std::size_t;// TODO_IMPORTANT_sposta (config?)
}

namespace lc
{

	/*template <int N, int M, class Field>
	struct FixedMatrixDataHelper
	{
		template <class CP>
		using aaa = typename lc::priv::FixedMatrixData<N, M, Field, Field, CP>;
	};

	template <class Field>
	struct DenseMatrixDataHelper
	{
		template <class CP>
		using aaa = lc::priv::DenseMatrixData<Field, CP>;
	};

	template <int N, int M, class Field = Np>
	struct MatrixHelper
	{
		using Matrix = typename lc::MatrixImpl<Field, typename FixedMatrixDataHelper<N, M, double>::aaa>;
	};*/

	/*template <int N, int M, class Field = Np>
	//using Matrix = lc::MatrixImpl<Field, FixedMatrixDataHelper<N, 3, double>::aaa>;
	using Matrix = typename MatrixHelper<N, M, Field>::Matrix;

	template <class Field = Np>
	using DenseMatrix = lc::MatrixImpl<Field, DenseMatrixDataHelper<double>::aaa>;*/

	template <int N, int M, class Field = double>
	using Matrix = lc::MatrixImpl<Field, lc::priv::FixedMatrixData<N, M, Field, Field>>;

	template <class Field = double>
	using DenseMatrix = lc::MatrixImpl<Field, lc::priv::DenseMatrixData<Field>>;

	template <int N, int M, class Field = double>
	using Mat = Matrix<N, M, Field>;
}

namespace lc
{
	namespace math
	{
		namespace geometry
		{
			using lc::Mat;
			using lc::Matrix;
		}
		using lc::Mat;
		using lc::Matrix;
	}
}
