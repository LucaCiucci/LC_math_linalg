
#include <catch2/catch_test_macros.hpp>

#include <LC/math/linalg/Tensor/stuff/TensorIndex.hpp>

namespace {
	static size_t counter = 0;

	struct MoveChecker {

		MoveChecker() = default;
		MoveChecker(const MoveChecker&) = delete;
		MoveChecker(MoveChecker&&) = default;

		MoveChecker& operator=(const MoveChecker&) = delete;
		MoveChecker& operator=(MoveChecker&&) = default;

		bool operator==(const MoveChecker&) const = default;

		size_t m_id = counter++;
	};

	std::ostream& operator<<(std::ostream& os, const MoveChecker&) {
		return os;
	}
}

TEST_CASE("Testing TensorIndex", "[TensorIndex][Tensor][index]") {
	
	using lc::TensorIndex;

	auto allZeros = [](const auto& c) { return std::all_of(c.begin(), c.end(), [](auto i) { return i == 0; }); };

	SECTION("TensorIndex") {
		
		SECTION("default constructor") {
			TensorIndex<3> index;
			REQUIRE(allZeros(index));
			index[2] = 42;
			index.~TensorIndex<3>();
			new (&index)TensorIndex<3>;
			REQUIRE(allZeros(index));
		}

		SECTION("init list constructor") {
			std::initializer_list<size_t> l = { 1, 2, 3 };
			TensorIndex<3> index = l;
			REQUIRE((index[0] == 1 && index[1] == 2 && index[2] == 3));
		}

		SECTION("init list move constructor") {
			TensorIndex<3> index = { 1, 2, 3 };
			REQUIRE((index[0] == 1 && index[1] == 2 && index[2] == 3));
		}

		SECTION("copy constructor") {
			TensorIndex<3> index = { 1, 2, 3 };
			REQUIRE(TensorIndex<3>(index) == index);
		}

		SECTION("move constructor") {
			size_t counter = ::counter;
			lc::TensorIndexImpl<3, MoveChecker> index;
			lc::TensorIndexImpl<3, MoveChecker> index2(std::move(index));
			REQUIRE((index2[0].m_id == counter++ && index2[1].m_id == counter++ && index2[2].m_id == counter++));
		}

		SECTION("span constructor") {
			std::array<size_t, 3> arr = { 1, 2, 3 };
			TensorIndex<3> index = std::span<const size_t, 3>(arr);
			REQUIRE((index[0] == 1 && index[1] == 2 && index[2] == 3));
		}

		// TODO assignments

		// TODO constexpr

		SECTION("tail") {
			constexpr bool ok = []() constexpr -> bool{
				const TensorIndex<3> index = { 1, 2, 3 };
				const auto tail = index.tail();
				return tail[0] == 2 && tail[1] == 3;
			};
			REQUIRE(ok);
		}

		SECTION("tailSpan") {
			constexpr bool ok = []() constexpr -> bool {
				TensorIndex<3> index = { 1, 2, 3 };
				const std::span<size_t, 2> tail = index.tailSpan();
				return tail[0] == 2 && tail[1] == 3;
			};
			REQUIRE(ok);
		}

		SECTION("tailSpan const") {
			constexpr bool ok = []() constexpr -> bool {
				const TensorIndex<3> index = { 1, 2, 3 };
				const auto tail = index.tailSpan();
				return tail[0] == 2 && tail[1] == 3;
			};
			REQUIRE(ok);
		}

		SECTION("ostream printing") {
			lc::PartialTensorIndex<3> index = { 1, 2, nullptr };
			std::ostringstream os;
			os << index;
			REQUIRE(os.str() == "(1, 2, .)");
		}
	}
}