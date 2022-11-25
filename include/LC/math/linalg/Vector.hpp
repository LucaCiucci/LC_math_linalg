/*LC_NOTICE_BEGIN
LC_NOTICE_END*/

#include "Tensor.hpp"

namespace lc
{
	namespace priv
	{
		// tells if a type can be dereferenced
		template <class T>
		concept CanBeDereferencedOnce = requires (T v) { { *v }; };

		template <class T, class T2>
		concept CanBeDereferencedInto = requires (T v) { { *v -> T2 }; };
	}

	// TODO using Np = double

	template <size_t N, class T = double>
	using Vector = Tensor<T, N>;

#define _LC_TMP_DECLARE_VectorN(Ne) template <class T = double> using Vector##Ne = Vector<Ne, T>;

	_LC_TMP_DECLARE_VectorN(2);
	_LC_TMP_DECLARE_VectorN(3);
	_LC_TMP_DECLARE_VectorN(4);

#define _LC_TMP_DECLARE_VectorNT(N, T, Type) using Vector##N##T = Vector##N<Type>;

	_LC_TMP_DECLARE_VectorNT(2, d, double);

	using Vector2d = Vector2<double>;
	using Vector3d = Vector3<double>;
	using Vector4d = Vector4<double>;

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
	using Vector4f = Vector4<float>;

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
	using Vector4f = Vector4<float>;
}

#undef _LC_TMP_DECLARE_VectorN
#undef _LC_TMP_DECLARE_VectorNT