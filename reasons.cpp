
#include <iostream>

//#include <LC/math/linalg/Tensor.hpp>
//#include <LC/tmp.hpp>
#include <LC/math/linalg/Tensor/Tensor.hpp>

#include <functional>

#include <numbers>
#include <random>
#include <chrono>

using namespace lc;
inline constexpr auto _ = nullptr;

int main() {
	std::cout << "Hello There!" << std::endl;

	Tensor<double, 3, _> t({{1, 2}, {0, 1}, {0, 0, 3}});

	std::cout << t << std::endl;

	std::initializer_list<std::initializer_list<double>> l = { {1, 2}, {3, 4}, {} };
	Tensor<double, 3, _> t2(l);
	std::cout << t2 << std::endl;
	
	double dd[2][2] = { {1, 2}, {3, 4} };
	
	Tensor<double, 2, _> t3((const double*)dd, TShape<2, 2>());
	Tensor<double, 2, _> t4((double*)dd, TShape<2, 2>());

	std::cout << t3 << std::endl;
	std::cout << t4 << std::endl;

	Tensor<double, _, _> t5({
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
		});

	std::cout << t5 << std::endl;
	t5.reshape({ 2, 5 });
	std::cout << t5 << std::endl;


	using A = TShape<3, 3>::PlainArr<double>;

	return 0;
}