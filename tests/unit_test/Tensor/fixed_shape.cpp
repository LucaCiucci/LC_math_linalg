
#include <catch2/catch_test_macros.hpp>

#include <LC/testing/MoveGuard.hpp>

#include <LC/math/linalg/Tensor.hpp>

TEST_CASE("constructors for fixed shape tensors", "[Tensor][fixed_shape][constructors]")
{
	using lc::Tensor;
	using lc::TensorShape;
	using lc::TShape;

	SECTION("static shape") {

		REQUIRE(Tensor<int, 2, 3, 4>::rank() == 3);
		REQUIRE(Tensor<int, 2, 3, 4>::variableSizeCount == 0);
		REQUIRE(Tensor<int, 2, 3, 4>::staticShape == TensorShape<3>({ 2, 3, 4 }));
		REQUIRE(Tensor<int, 2, 3, 4>::shape() == Tensor<int, 2, 3, 4>::staticShape);
		REQUIRE(Tensor<int, 2, 3, 4>::elementsCount() == (2 * 3 * 4));

		SECTION("Default constructor") {
			constexpr bool ok = []() constexpr -> bool {
				using T = Tensor<int, 2, 3, 4>;
				T t;

				// check initied to zeros
				for (size_t i = 0; i < t.shape()[0].value(); ++i)
					for (size_t j = 0; j < t.shape()[1].value(); ++j)
						for (size_t k = 0; k < t.shape()[2].value(); ++k)
							if (t[{i, j, k}] != 0)
								return false;

				return true;
			}();
			REQUIRE(ok);
		}

		SECTION("default construtor correctness") {

			REQUIRE(([]() -> bool {
				using T = Tensor<int, 2, 3, 4>;
				T t;

				// check initied to zeros
				for (size_t i = 0; i < t.shape()[0].value(); ++i)
					for (size_t j = 0; j < t.shape()[1].value(); ++j)
						for (size_t k = 0; k < t.shape()[2].value(); ++k)
							if (t[{i, j, k}] != 0)
								return false;

				// fill with numbers
				{
					int counter = 0;
					for (size_t i = 0; i < t.shape()[0].value(); ++i)
						for (size_t j = 0; j < t.shape()[1].value(); ++j)
							for (size_t k = 0; k < t.shape()[2].value(); ++k)
								t[{i, j, k}] = counter++;
				}

				// destroy and reinit
				t.~Tensor<int, 2, 3, 4>();
				new (&t)T;

				// check initied to zeros
				for (size_t i = 0; i < t.shape()[0].value(); ++i)
					for (size_t j = 0; j < t.shape()[1].value(); ++j)
						for (size_t k = 0; k < t.shape()[2].value(); ++k)
							if (t[{i, j, k}] != 0)
								return false;
				return true;
			}()));
		}

#define ARR_R3_INIT {      \
	{                      \
		{ 1,  2,  3,  4 }, \
		{ 5,  6,  7,  8 }, \
		{ 9, 10, 11, 12 }  \
	},                     \
	{                      \
		{ 13, 14, 15, 16 },\
		{ 17, 18, 19, 20 },\
		{ 21, 22, 23, 24 } \
	}                      \
}

		SECTION("construct from array") {
			const int arr[2][3][4] = ARR_R3_INIT;
			const Tensor<int, 2, 3, 4> t = arr;

			bool ok = true;
			for (size_t i = 0; i < t.shape()[0].value(); ++i)
				for (size_t j = 0; j < t.shape()[1].value(); ++j)
					for (size_t k = 0; k < t.shape()[2].value(); ++k)
						if (t[{i, j, k}] != arr[i][j][k])
							ok = false;
			REQUIRE(ok);
		}

		SECTION("construct from array (move)") {

			// TOOD use some move checker class like in TensorIndex
			size_t counter = lc::MoveGuard<int>::counterValue();
			const Tensor<lc::MoveGuard<int>, 2, 3, 4> t(ARR_R3_INIT);
			const lc::MoveGuard<int> arr[2][3][4] = ARR_R3_INIT;

			bool ok = true;
			for (size_t i = 0; i < t.shape()[0].value(); ++i)
				for (size_t j = 0; j < t.shape()[1].value(); ++j)
					for (size_t k = 0; k < t.shape()[2].value(); ++k)
						if (t[{i, j, k}].id() != counter++ || t[{i, j, k}].value() != arr[i][j][k].value())
							ok = false;
			REQUIRE(ok);
		}

		SECTION("copy constructor") {
			const Tensor<int, 2, 3, 4> t(ARR_R3_INIT);
			const Tensor<int, 2, 3, 4> t2 = t;
			bool ok = true;
			for (size_t i = 0; i < t.shape()[0].value(); ++i)
				for (size_t j = 0; j < t.shape()[1].value(); ++j)
					for (size_t k = 0; k < t.shape()[2].value(); ++k)
						if (t2[{i, j, k}] != t[{i, j, k}])
							ok = false;
			REQUIRE(ok);
		}

		SECTION("move constructor") {
			size_t counter = lc::MoveGuard<int>::counterValue();
			Tensor<lc::MoveGuard<int>, 2, 3, 4> t(ARR_R3_INIT);
			const Tensor<lc::MoveGuard<int>, 2, 3, 4> t2(std::move(t));
			const lc::MoveGuard<int> arr[2][3][4] = ARR_R3_INIT;
			bool ok = true;
			for (size_t i = 0; i < t.shape()[0].value(); ++i)
				for (size_t j = 0; j < t.shape()[1].value(); ++j)
					for (size_t k = 0; k < t.shape()[2].value(); ++k)
						if (t2[{i, j, k}].id() != counter++ || t2[{i, j, k}].value() != arr[i][j][k].value())
							ok = false;
			REQUIRE(ok);
		}
	}
}