
#include <iostream>

#include <LC/math/linalg/Vector/Vector.hpp>
#include <LC/math/linalg/Matrix/Matrix.hpp>

using namespace lc;

int main() {
	std::cout << "Hello There!" << std::endl;

	Vector3d v1(1, 2, 3);
	Mat<3, 3, double> m1({
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	});

	std::cout << v1 << std::endl;
	std::cout << m1 << std::endl;
	std::cout << m1 * m1 << std::endl;
	std::cout << m1 * v1 << std::endl;

	return 0;
}
