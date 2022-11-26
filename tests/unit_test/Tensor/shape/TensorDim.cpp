
#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <LC/math/linalg/Tensor/stuff/TensorIndexShape.hpp>

TEST_CASE("Testing TensorDim", "[TensorDim][Tensor][shape]") {
	
	using lc::TensorDim;

	SECTION("Constructors") {
		
		SECTION("Default constructor") {
			TensorDim dim;
			
			REQUIRE(dim.is(TensorDim::Type::Variable));
			REQUIRE(dim.value() == 0);
		}

		SECTION("Default constructor (completeness)") {
			TensorDim dim(42);

			dim.~TensorDim();
			new(&dim)TensorDim;
			REQUIRE(dim.is(TensorDim::Type::Variable));
			REQUIRE(dim.value() == 0);
		}

		SECTION("value constructor") {
			TensorDim dim((size_t)42);

			REQUIRE(dim.is(TensorDim::Type::Number));
			REQUIRE(dim.value() == 42);
		}

		SECTION("value constructor (int)") {
			TensorDim dim((int)42);

			REQUIRE(dim.is(TensorDim::Type::Number));
			REQUIRE(dim.value() == 42);
		}

		SECTION("variable size") {
			TensorDim dim(lc::variable_size);

			REQUIRE(dim.is(TensorDim::Type::Variable));
			REQUIRE(dim.value() == 0);
		}

		SECTION("variable size (nullptr)") {
			TensorDim dim(nullptr);

			REQUIRE(dim.is(TensorDim::Type::Variable));
			REQUIRE(dim.value() == 0);
		}

		auto provider_number = []() -> TensorDim {
			TensorDim dim(42);
			return dim;
		};

		auto provider_variable = []() -> TensorDim {
			TensorDim dim(nullptr);
			return dim;
		};

		SECTION("equality") {
			REQUIRE(provider_number() == provider_number());
			REQUIRE(provider_variable() == provider_variable());
			REQUIRE(provider_number() != provider_variable());
			REQUIRE(TensorDim(2) != TensorDim(3));
		}

		SECTION("move constructor") {
			REQUIRE(TensorDim(std::move(provider_number())) == provider_number());
			REQUIRE(TensorDim(std::move(provider_variable())) == provider_variable());
		}

		SECTION("copy constructor") {
			auto n = provider_number();
			auto v = provider_variable();
			REQUIRE(TensorDim(std::move(n)) == provider_number());
			REQUIRE(TensorDim(std::move(v)) == provider_variable());
		}

		SECTION("move assignment") {
			TensorDim dim;
			dim = provider_number();
			REQUIRE(dim == provider_number());
		}

		SECTION("copy assignment") {
			TensorDim dim;
			TensorDim dim2 = provider_number();
			dim = dim2;
			REQUIRE(dim == dim2);
		}
	}
	
	SECTION("ostream printing") {
		std::ostringstream os;
		os << TensorDim();
		REQUIRE(os.str() == "~");
		os.str("");
		os << TensorDim(42);
		REQUIRE(os.str() == "42");
	}
}