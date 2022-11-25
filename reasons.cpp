
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

	Tensor<double, 3, 3> t;

	std::cout << t << std::endl;

	return 0;
}