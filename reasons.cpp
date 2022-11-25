
#include <iostream>

//#include <LC/math/linalg/Tensor.hpp>
//#include <LC/tmp.hpp>
#include <LC/math/linalg/Tensor/Tensor.hpp>

#include <functional>

#include <numbers>
#include <random>
#include <chrono>

struct tmp_tilde {};
inline void operator~(const tmp_tilde&) {}

struct F {
	constexpr F() : v{ 1,2,3 } {
	}

	constexpr ~F() {

	}
	std::vector<int> v = { 1,2,3 };
};

constexpr int example_fcn()
{
	std::array<int, 1> arr = { 4 };
	std::span<int, 1> arrs(arr.begin(), arr.end());

	for (const int& a : arrs)
		return a;

	return lc::elemCount(arrs);

	return 0;
}

constexpr double fooooo() {
	using A = lc::FixedTensorContainer<lc::TMP_TENSOR_IMPL, double, lc::TShape<nullptr, nullptr>>;
	A aaaa({
		{ 10, 2, 3 },
		{ 3, 4 },
		{ 0, 0, 5, 0, 0, 1}
		});

	//std::array<double, 10> dd = {};
	//std::span<double, 5> ds(dd.data(), 5);
	
	return aaaa[{0, 1}];
	return 0;
}

int main() {
	std::cout << "Hello There!" << std::endl;

	//using namespace lc;

	//Tensor<double, 3, 3> M;
	//for (int i = 0; i < 3; i++)
	//	for (int j = 0; j < 3; j++)
	//		M[i][j] = i * 3 + j + 1;
	//auto M2 = M.plus(M);

	//std::cout << put_beside("M: ", M, " M2: ", M2) << std::endl;

	//tmp::print_sequence(std::make_integer_sequence<size_t, 3>());

	lc::TensorShape<3> aa({ (size_t)1, (size_t)2, {} });
	//aa.size();
	for (const auto& b : aa)
		std::cout << b << std::endl;
	std::cout << aa << std::endl;

	lc::VariableLenghtTensorShape s = { 1, 2, 3 };
	std::cout << s << std::endl;
	std::cin >> s;
	std::cout << s << std::endl;

	//using A = lc::FixedTensorContainer_<double, 2, lc::tshape<2>{ 1, 2 }, true >::AAA;
	
	lc::FixedTensorContainer<lc::TMP_TENSOR_IMPL, double, lc::TShape<1, 2, 3>> C = {};

	C.shape();
	std::cout << C.shape() << std::endl;
	std::cout << C.data() << std::endl;

	constexpr auto shape = lc::TensorShape<3>{ nullptr, 2, nullptr };
	lc::TensorIndex<3> ti = { 1, 2, 3 };
	using A = lc::FixedTensorContainer<lc::TMP_TENSOR_IMPL, double, lc::TShape<nullptr, nullptr>>;
	A aaaa({
		{ 10, 2, 3 },
		{ 3, 4 },
		{ 0, 0, 5, 0, 0, 1}
	});
	//aaaa[{1, 2}];
	std::cout << "aaaa:\n" << aaaa << std::endl;
	std::cout << lc::hide_zeors(aaaa) << std::endl;
	aaaa.reshape({ nullptr, 4 });
	aaaa.reshape({ 2 });
	std::cout << lc::hide_zeors(aaaa) << std::endl;
	aaaa.reshape({ 10, 10 });
	aaaa[{ 5, 5 }] = 1000;
	std::cout << lc::hide_zeors(aaaa) << std::endl;
	aaaa.reshape({ 1 });
	std::cout << lc::hide_zeors(aaaa) << std::endl;
	aaaa.reshape(lc::TensorShape<2>{ 3, 3 });
	auto d = aaaa.data();
	for (int i = 0; i < 9; ++i)
		d[i] = i + 1;
	aaaa.reshape(lc::TensorShape<2>{ 4, 2 });
	for (int i = 0; i < 8; ++i)
		std::cout << d[i] << std::endl;

	using B = lc::FixedTensorContainer<lc::TMP_TENSOR_IMPL, double, lc::TShape<nullptr>>;
	B bb;

	using CC = lc::FixedTensorContainer<lc::TMP_TENSOR_IMPL, double, lc::TShape<3>>;

	CC c;

	constexpr auto sssss = example_fcn();

	//constexpr A ca;
	constexpr int rrr = fooooo();
	std::cout << rrr << std::endl;


	/*constexpr std::vector<int> v;

	constexpr auto iii = ca[{0, 0}];*/

	sizeof(A);
	sizeof(CC);
	std::cout << sizeof(CC) << std::endl;

	using TT = lc::Tensor<double, 3, nullptr>;
	TT tt({
		{ 1, 2, 3},
		{ 1, 2},
		{ 1, 2}
		});
	//constexpr bool aawf = lc::BidimensionalFixedTensorContainerType<TT>;

	constexpr bool ewgrm = lc::StaticRankTensor<TT>;
	
	std::cout << tt << std::endl;

	return 0;
}