#pragma once

//#include <LC/config/config.h>

#include "./VectorData.hpp"
#include "VectorImplementation.hpp"

// if can include <cereal/cereal.hpp>
#if __has_include(<cereal/cereal.hpp>)
	#include <cereal/cereal.hpp>
#endif

namespace lc
{
	template <int N, class Field, class T = Field>
	using Vector = VectorImpl<Field, lc::priv::VectorDataFixed<N, Field, T>, true, N>;

	template <class Field, class T = Field>
	using DenseVector = VectorImpl<Field, std::vector<T>, false>;

	template <class Field, class T = Field> using Vector2 = Vector<2, Field, T>;
	template <class Field, class T = Field> using Vector3 = Vector<3, Field, T>;
	template <class Field, class T = Field> using Vector4 = Vector<4, Field, T>;

	// some common vectors
	using Vector2b = Vector2<uint8_t>;
	using Vector2u = Vector2<unsigned>;
	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;

	using Vector3b = Vector3<uint8_t>;
	using Vector3u = Vector3<unsigned>;
	using Vector3i = Vector3<int>;
	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;

	using Vector4b = Vector4<uint8_t>;
	using Vector4u = Vector4<unsigned>;
	using Vector4i = Vector4<int>;
	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;

#if __has_include(<cereal/cereal.hpp>)
	template <class Archive, int N, class Field>
	void serialize(Archive& archive, Vector<N, Field, Field>& v)
	{
		// in text archives, treat the object as an array
		if constexpr (cereal::traits::is_text_archive<Archive>::value)
			archive(cereal::make_size_tag<cereal::size_type>(v.size()));

		// write the data
		for (auto& x : v)
			archive(x);
	}
#endif
}

// see https://github.com/USCiLab/cereal/issues/319
// CEREAL_CLASS_VERSION(Vector, 0);
// namespace cereal {
// 	namespace detail {
// 		template <int N, class Field>
// 		struct Version<::lc::Vector<N, Field, Field>>
// 		{
// 			static const std::uint32_t version;
// 			static std::uint32_t registerVersion()
// 			{
// 				constexpr std::uint32_t v = 0; // VERSION!
// 				::cereal::detail::StaticObject<Versions>::getInstance().mapping.emplace(
// 					std::type_index(typeid(::lc::Vector<N, Field, Field>)).hash_code(), v);
// 				return v;
// 			}
// 			static void unused() { (void)version; }
// 		}; /* end Version */
// 		template <int N, class Field>
// 		const std::uint32_t Version<::lc::Vector<N, Field, Field>>::version =
// 			Version<::lc::Vector<N, Field, Field>>::registerVersion();
// 	}
// } // end namespaces

#define _LC_TMP_VECTORINSTANCES \
	using lc::Vector;           \
	using lc::Vector2;          \
	using lc::Vector3;           \
	using lc::Vector4;           \
	using lc::Vector2b;         \
	using lc::Vector2u;         \
	using lc::Vector2i;         \
	using lc::Vector2f;         \
	using lc::Vector2d;         \
	using lc::Vector3b;         \
	using lc::Vector3u;         \
	using lc::Vector3i;         \
	using lc::Vector3f;         \
	using lc::Vector3d;         \
	using lc::Vector4b;         \
	using lc::Vector4u;         \
	using lc::Vector4i;         \
	using lc::Vector4f;         \
	using lc::Vector4d;         \
	using lc::DenseVector;

namespace lc
{
	namespace math
	{
		namespace geometry
		{
			_LC_TMP_VECTORINSTANCES
		}
		_LC_TMP_VECTORINSTANCES
	}
}
#undef _LC_TMP_VECTORINSTANCES
