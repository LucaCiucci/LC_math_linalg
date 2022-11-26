
#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <LC/math/linalg/Tensor/stuff/TensorIndexShape.hpp>

TEST_CASE("Testing TensorPartialIndexValue", "[TensorPartialIndexValue][Tensor][shape]") {

	using lc::TensorPartialIndexValue;

	SECTION("Constructors") {

		SECTION("Default constructor") {
			TensorPartialIndexValue val;

			REQUIRE(val.is(TensorPartialIndexValue::Type::Number));
			REQUIRE(val.value() == 0);
		}

		SECTION("Default constructor (completeness)") {
			TensorPartialIndexValue val(lc::undef_index);

			val.~TensorPartialIndexValue();
			new(&val)TensorPartialIndexValue;
			REQUIRE(val.is(TensorPartialIndexValue::Type::Number));
			REQUIRE(val.value() == 0);
		}

		SECTION("value constructor") {
			TensorPartialIndexValue val((size_t)42);

			REQUIRE(val.is(TensorPartialIndexValue::Type::Number));
			REQUIRE(val.value() == 42);
		}

		SECTION("value constructor (int)") {
			TensorPartialIndexValue val((int)42);

			REQUIRE(val.is(TensorPartialIndexValue::Type::Number));
			REQUIRE(val.value() == 42);
		}

		SECTION("undef index") {
			TensorPartialIndexValue val(lc::undef_index);

			REQUIRE(val.is(TensorPartialIndexValue::Type::Undefined));
			REQUIRE(val.value() == 0);
		}

		SECTION("undef index (nullptr)") {
			TensorPartialIndexValue val(nullptr);

			REQUIRE(val.is(TensorPartialIndexValue::Type::Undefined));
			REQUIRE(val.value() == 0);
		}

		auto provider_number = []() -> TensorPartialIndexValue {
			TensorPartialIndexValue val(42);
			return val;
		};

		auto provider_variable = []() -> TensorPartialIndexValue {
			TensorPartialIndexValue val(nullptr);
			return val;
		};

		SECTION("equality") {
			REQUIRE(provider_number() == provider_number());
			REQUIRE(provider_variable() == provider_variable());
			REQUIRE(provider_number() != provider_variable());
			REQUIRE(TensorPartialIndexValue(2) != TensorPartialIndexValue(3));
		}

		SECTION("move constructor") {
			REQUIRE(TensorPartialIndexValue(std::move(provider_number())) == provider_number());
			REQUIRE(TensorPartialIndexValue(std::move(provider_variable())) == provider_variable());
		}

		SECTION("copy constructor") {
			auto n = provider_number();
			auto v = provider_variable();
			REQUIRE(TensorPartialIndexValue(std::move(n)) == provider_number());
			REQUIRE(TensorPartialIndexValue(std::move(v)) == provider_variable());
		}

		SECTION("move assignment") {
			TensorPartialIndexValue val;
			val = provider_number();
			REQUIRE(val == provider_number());
		}

		SECTION("copy assignment") {
			TensorPartialIndexValue val;
			TensorPartialIndexValue val2 = provider_number();
			val = val2;
			REQUIRE(val == val2);
		}
	}

	SECTION("ostream printing") {
		std::ostringstream os;
		os << TensorPartialIndexValue(nullptr);
		REQUIRE(os.str() == ".");
		os.str("");
		os << TensorPartialIndexValue(42);
		REQUIRE(os.str() == "42");
	}
}