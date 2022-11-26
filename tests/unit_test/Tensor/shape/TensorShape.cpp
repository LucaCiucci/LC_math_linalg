#include <catch2/catch_test_macros.hpp>

#include <LC/math/linalg/Tensor/stuff/TensorIndexShape.hpp>

TEST_CASE("Testing TensorShape", "[TensorShape][shape]")
{
	using lc::TensorDim;
	using lc::TensorShape;

	SECTION("Constructors") {
		
		SECTION("Default constructor") {
			TensorShape<3> shape;

			for (const auto& dim : shape) {
				REQUIRE(dim == TensorDim());
			}
		}

		SECTION("rank") {
			REQUIRE(TensorShape<3>::rank() == 3);
			REQUIRE(TensorShape<2>::rank() == 2);
			REQUIRE(TensorShape<1>::rank() == 1);
			REQUIRE(TensorShape<0>::rank() == 0);

			REQUIRE(lc::VariableLenghtTensorShape({ 1, 2, 3, 4 }).rank() == 4);
			REQUIRE(lc::VariableLenghtTensorShape().rank() == 0);
		}

		SECTION("shape_tail") {
			TensorShape<3> shape({ 1, 2, 3 });
			
			REQUIRE(lc::shape_tail(shape) == TensorShape<2>({ 2, 3 }));
		}
	}

	SECTION("TShape") {
		
		SECTION("Creation") {
			REQUIRE(lc::TShape<1, 2, 3>::shape == TensorShape<3>({ 1, 2, 3 }));
			REQUIRE(lc::TShape<1, nullptr, 3>::shape == TensorShape<3>({ 1, nullptr, 3 }));
		}
		
		// TODO...
	}
}