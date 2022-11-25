
#pragma once


#include <LC/math/linalg/IndexShape.hpp>

namespace lc
{
	template <class LambdaDataProvider> // TODO sizes unnecessary?
	struct FixedLambdaTensorContainerBase;

	// TODO this specialization is likely to be unnecessary, it is very unlikely
	// that a default constructible lambda can provide data
	// or... maybe if it references some global/static variables
	template <class LambdaDataProvider>
		requires (std::is_default_constructible_v<LambdaDataProvider>)
	struct FixedLambdaTensorContainerBase<LambdaDataProvider>
	{
	public:

	protected:

		LambdaDataProvider dataProvider() const { return {}; }
	};

	template <class LambdaDataProvider>
		requires (!std::is_default_constructible_v<LambdaDataProvider>)
	struct FixedLambdaTensorContainerBase<LambdaDataProvider>
	{
	public:

		FixedLambdaTensorContainerBase(const LambdaDataProvider& dataProvider) :
			m_dataProvider(dataProvider)
		{
		}

	protected: public:

		LambdaDataProvider& dataProvider() { return m_dataProvider; }
		const LambdaDataProvider& dataProvider() const { return m_dataProvider; }

	private:
		LambdaDataProvider m_dataProvider;
	};

	/*template <class Container, size_t S>
	inline constexpr auto&& unpackIndexing(Container& c, size_t i)
	{
		return c[i];
	}

	template <class Container, size_t S, size_t... Sizes>
	inline constexpr auto&& unpackIndexing(Container& c, size_t i, AlwaysT<Sizes, size_t>... ii)
	{
		return unpackIndexing<decltype(c[i]), Sizes...>(c[i], ii...);
	}*/

	template <template <class T> class _TImpl, size_t N, TensorShape<N> _shape>
	class FixedSlice
	{
	public:

		static constexpr inline TensorShape<N> staticShape = _shape;

		

	private:
		
	};
}