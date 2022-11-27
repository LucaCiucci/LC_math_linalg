
#include <catch2/catch_test_macros.hpp>

#include <LC/math/linalg/Tensor/stuff/PartialFixedTensorShapeHolder.hpp>

namespace {
	template <lc::TShapeType _Shape>
	class Exposer : public lc::PartialFixedTensorShapeHolder<_Shape>
	{
	public:
		// TODO subclasses should set this functions as private
		using lc::PartialFixedTensorShapeHolder<_Shape>::freeSizeInternalIndex;
		using lc::PartialFixedTensorShapeHolder<_Shape>::freeSizeInternalIndex2index;
		using lc::PartialFixedTensorShapeHolder<_Shape>::freeSizeAt;
		using lc::PartialFixedTensorShapeHolder<_Shape>::is_dim_assignable;
		using lc::PartialFixedTensorShapeHolder<_Shape>::is_dim_ok_for_reshape;
		using lc::PartialFixedTensorShapeHolder<_Shape>::mem_shape;
	};

	using lc::PartialFixedTensorShapeHolder;
	using lc::TShape;
	constexpr auto _ = nullptr;

	template <class T>
	using PFTSH = PartialFixedTensorShapeHolder<T>;
}


TEST_CASE("PartialFixedTensorShapeHolder", "") {

	REQUIRE(PartialFixedTensorShapeHolder<TShape<2, _, 4>>::rank() == 3);
	REQUIRE(PartialFixedTensorShapeHolder<TShape<2, _, 4>>::staticShape == TShape<2, _, 4>::shape);
	REQUIRE(PartialFixedTensorShapeHolder<TShape<2, _, 4>>::variableSizeCount == 1);
	REQUIRE(PartialFixedTensorShapeHolder<TShape<_, _, 4>>::variableSizeCount == 2);
	REQUIRE(PartialFixedTensorShapeHolder<TShape<_, _, _>>::variableSizeCount == 3);
	REQUIRE(PartialFixedTensorShapeHolder<TShape<_, _, _, _>>::variableSizeCount == 4);
	
	SECTION("constructors") {
		SECTION("default") {
			PartialFixedTensorShapeHolder<TShape<2, _, 4>> t;
			REQUIRE(t.shape() == TShape<2, 0, 4>::shape);

			// TODO destroy and recreate in place
		}
		
		SECTION("copy") {
			Exposer<TShape<2, _, 4>> t1;
			t1.mem_shape({2, 3, 4});
			PartialFixedTensorShapeHolder<TShape<2, _, 4>> t2(t1);
			REQUIRE(t2.shape() == TShape<2, 3, 4>::shape);
		}

		SECTION("move") {
			Exposer<TShape<2, _, 4>> t1;
			t1.mem_shape({ 2, 3, 4 });
			PartialFixedTensorShapeHolder<TShape<2, _, 4>> t2(std::move(t1));
			REQUIRE(t2.shape() == TShape<2, 3, 4>::shape);
		}
	}

	SECTION("assignment") {
		SECTION("copy") {
			Exposer<TShape<2, _, 4>> t1;
			t1.mem_shape({ 2, 3, 4 });
			PartialFixedTensorShapeHolder<TShape<2, _, 4>> t2;
			t2 = t1;
			REQUIRE(t2.shape() == TShape<2, 3, 4>::shape);
		}

		SECTION("move") {
			Exposer<TShape<2, _, 4>> t1;
			t1.mem_shape({ 2, 3,  4 });
			PartialFixedTensorShapeHolder<TShape<2, _, 4>> t2;
			t2 = std::move(t1);
			REQUIRE(t2.shape() == TShape<2, 3, 4>::shape);
		}
	}

	SECTION("protected functions") {
		
		SECTION("freeSizeInternalIndex") {
			REQUIRE(Exposer<TShape<_>>::freeSizeInternalIndex(0) == 0);
			REQUIRE(Exposer<TShape<1, _>>::freeSizeInternalIndex(1) == 0);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::freeSizeInternalIndex(1) == 0);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::freeSizeInternalIndex(4) == 1);
		}

		SECTION("freeSizeInternalIndex2index") {
			REQUIRE(Exposer<TShape<_>>::freeSizeInternalIndex2index(0) == 0);
			REQUIRE(Exposer<TShape<1, _>>::freeSizeInternalIndex2index(0) == 1);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::freeSizeInternalIndex2index(0) == 1);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::freeSizeInternalIndex2index(1) == 4);
		}

		SECTION("freeSizeAt") {
			Exposer<TShape<2, _, 4, 5, _>> t;
			t.mem_shape({ 2, 3, 4, 5, 6});
			REQUIRE(t.freeSizeAt(1) == 3);
			REQUIRE(t.freeSizeAt(4) == 6);
		}

		SECTION("freeSizeAt (const)") {
			constexpr auto t = []() constexpr {
				Exposer<TShape<2, _, 4, 5, _>> t;
				t.mem_shape({ 2, 3, 4, 5, 6 });
				return t;
			}();
			REQUIRE(t.freeSizeAt(1) == 3);
			REQUIRE(t.freeSizeAt(4) == 6);
		}

		SECTION("is_dim_assignable") {
			REQUIRE(Exposer<TShape<_>>::is_dim_assignable(0) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_assignable(0) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_assignable(1) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_assignable(2) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_assignable(3) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_assignable(4) == true);
		}

		SECTION("is_dim_ok_for_reshape") {
			REQUIRE(Exposer<TShape<_>>::is_dim_ok_for_reshape({ 42 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 4, 5, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ _, 3, 4, 5, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, _, 5, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ _, 3, _, _, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 4, _, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 4, 5, _ }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, _, 4, 5, _ }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, _, 4, 5, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 12, 3, 4, 5, 6 }) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 13, 4, 5, 6 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 14, 5, 6 }) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 4, 15, 6 }) == false);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 3, 4, 5, 16 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 2, 13, 4, 5, 16 }) == true);
			REQUIRE(Exposer<TShape<2, _, 4, 5, _>>::is_dim_ok_for_reshape({ 12, 3, 14, 15, 6 }) == false);
		}

		SECTION("mem_shape") {
			Exposer<TShape<2, _, 4, 5, _>> t;
			t.mem_shape({ 2, 3, 4, 5, 6 });
			REQUIRE(t.shape() == TShape<2, 3, 4, 5, 6>::shape);
			t.mem_shape({ 2, 13, 4, _, 16 });
			REQUIRE(t.shape() == TShape<2, 13, 4, 5, 16>::shape);
			t.mem_shape({ _, 23, _, _, 26 });
			REQUIRE(t.shape() == TShape<2, 23, 4, 5, 26>::shape);
		}
	}
}