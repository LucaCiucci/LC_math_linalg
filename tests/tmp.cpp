

#include <LC/math/linalg/Vector.hpp>

#include <vector>

#include <LC/math/linalg/Tensor.hpp>

#include <ranges>

template <class T, int... Sizes>
concept TensorContainer = requires(T c) {
	{ c.size() } -> std::convertible_to<size_t>;
};

template <template <class T> class Impl>
// TODO require Impl constructoble from C<Impl> and similar
struct C
{
	Impl<C<Impl>> foo() { return {}; };
};

template <class Container>
struct DummyImpl : Container
{

};

struct TImpl : C<DummyImpl>
{
	using Ty = float;
};

using TImpl2 = DummyImpl<C<DummyImpl>>;

void a() {
	using namespace lc;

	//Tensor<int, 3, 3> t;
	
	std::tuple<int, int, int>;

	constexpr auto p = priv::product(1, 2, 3, 4);

	//FixedTensorContainer<uint8_t, 3, 3> d;
	//auto v = d.at(0).at(0);
	//constexpr auto S = sizeof(d);

	//TensorImpl<AT> t;
	//auto r = t.foo();
	//auto r2 = r;

	TImpl2 t;
	auto r = t.foo();
	auto r2 = r.foo();
	r2.foo().foo();

	Tensor<double, 2, 3> T;
	auto a = T.at(0);
	auto a2 = T.at(0).at(0);

	//T.shape() == std::array<size_t, 2>({ 2, 2 });
	//bool aaa = (T.shape() == priv::ShapeArray<2>({ 2, 2 }));
	constexpr auto s = T.shape();
	constexpr auto s0 = s[0];
	constexpr auto s1 = s[1];
	//constexpr auto s2 = s[2];
	constexpr bool eq = s.equals({ 2, 3 });

	constexpr auto i = priv::CountNumbers<size_t, 2, 2>::value;

	//std::tuple
	priv::StaticIndexShape<2, 3, 4, 5, 6> shape;
	//constexpr auto s = sizeof(i2);

	constexpr auto b = (shape.equals({2, 3, 4, 5, 6}));

	// TODO assert sizeof StaticIndexShape = char
}


