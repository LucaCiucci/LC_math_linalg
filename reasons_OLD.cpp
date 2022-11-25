
#include <iostream>

#include <LC/math/linalg/Tensor.hpp>
#include <LC/tmp.hpp>

#include <functional>

#include <numbers>
#include <random>
#include <chrono>

int main() {
	std::cout << "Hello There!" << std::endl;

	using Ten = lc::Tensor<double, 1, 2, 3, 4, 5, 6, 7>;

	std::cout << sizeof(Ten) << std::endl;
	Ten T;
	std::cout << T.shape() << std::endl;
	T.at(0).at(0).at(0).at(0).at(0).at(2).at(1) = 2;
	std::cout << T.at(0).at(0).at(0).at(0).at(0).at(0).at(0) << std::endl;
	std::cout << T.at(0).at(0).at(0).at(0).at(0).at(0) << std::endl;
	std::cout << T.at(0).at(0).at(0).at(0).at(0) << std::endl;
	std::cout << T.at(0).at(0).at(0).at(0) << std::endl;

	lc::Tensor<double, 2, 3> T2;
	auto a = T2.get({ 1, 1 });
	//std::initializer_list<int>;

	auto provider = [&T2](size_t i, size_t j) -> auto& {
		return T2[j][i];
	};
	
	lc::TensorImpl<lc::FixedLambdaTensorContainer<lc::TensorImpl,  decltype(provider), 3, 2>> fltc(provider);
	//fmtc.a;
	//constexpr auto a3 = fmtc.mapper(3);
	std::cout << fltc.shape() << std::endl;
	
	fltc.at(0).at(1) = 5;
	const auto r = fltc.at(0);
	//r.at(1) = 1;

	std::cout << T2 << std::endl;
	//////////////////////std::cout << fltc << std::endl;
	(*fltc.begin()).begin();

	constexpr auto jsf = lc::priv::BidimensionalTensor<decltype(fltc)>;
	constexpr auto jsf2 = lc::priv::NdTensor<decltype(fltc), 1>;

	{
		// TODO questa cosa è strana, l'iteratore operator* restituisce un rvalue, come è possibile che ci fa fare...
		auto bb = fltc.begin();
		const auto& a = *bb; // questo?!?!??!?
		// forse queste cose sono legali ma devo studiare meglio cosa dice lo standard
		// auto& a2 = *bb; // ovviamente questo non si può fare e quindi da' errore

		for (const auto& r : fltc)
		{
			std::cout << "---" << std::endl;
			for (auto&& e : r)
				std::cout << e << std::endl;
		}
	}

	for (auto r : fltc)
		for (auto&& e : r)
			e = 1;
	///////////////////////////std::cout << fltc << std::endl;
	std::cout << T2 << std::endl;
	//T2.transposed().at(0) = 0;
	//auto& d = T2.transposed().at(0).at(0);
	//d = 1;
	T2.transposed().at(2).at(1) = 0;
	std::cout << T2 << std::endl;


	std::array<int, 3> arr_aefgr;

	lc::Tensor<double, 2, 2, 3> T3;
	auto provider2 = [&T3](size_t i, size_t j) -> double& {
		return T3[0][j][1];
	};
	lc::TensorImpl<lc::FixedLambdaTensorContainer<lc::TensorImpl, decltype(provider2), 3, 2>> fltc2(provider2);
	fltc2.at(0).at(0) = 1;
	///////////////std::cout << fltc2 << std::endl;
	/////////////for (const auto& M : T3)
		////////////////std::cout << M << std::endl << M.transposed() << std::endl;

	lc::TensorImpl<lc::FixedLambdaTensorContainer<lc::TensorImpl, decltype(provider2), 3, 2>> fltc3(provider2);

	std::function<void()> fefegre;

	int ggg = 0, ggg2 = 0;
	char ggg3 = 0, ggg4 = 1, ggg5 = 2, ggg6 = 3, ggg7 = 4;
	auto ll = [&ggg, &ggg2, ggg3, ggg4, ggg5, ggg6, ggg7]() {};
	using LL = decltype(ll);
	constexpr auto ll_s = sizeof(ll);
	//LL ll2([&ggg, &ggg2, ggg3, ggg4, ggg5, ggg6, ggg7]() {});
	
	std::vector<std::vector<std::vector<int>>> vvv;

	lc::Tensor<double, 2, 2, 2> T222;
	T222[0][0][1] = 1;
	//auto S222 = T222.sub_view(std::array<size_t, 3>{0, 1}, std::array<size_t, 4>{1, 0}, std::array<size_t, 5>{1, 0});
	auto S222 = T222.sub_view<
		std::array<size_t, 3>,
		std::array<size_t, 4>,
		std::array<size_t, 5>
	>(
		std::array<size_t, 3>{0, 1},
		std::array<size_t, 4>{1, 0},
		std::array<size_t, 5>{1, 0}
	);
	{
		auto SSSSSSSSSSSSS = T222.sub_view<
			std::array<size_t, 3>,
			std::array<size_t, 4>,
			std::array<size_t, 5>
		>(
			std::array<size_t, 3>{0, 1},
			std::array<size_t, 4>{1, 0},
			std::array<size_t, 5>{1, 0}
		);
	}
	//S222[0][3][1] = 3;
	auto&& sdfjekn = S222[0].dataProvider();
	auto aa1 = S222[0];
	auto aa2 = S222[0][0]; aa1[0];
	auto provider_ = aa2.dataProvider();
	provider_(0);
	aa2.dataProvider()(0);
	aa1[0].dataProvider()(0);
	//S222[0][0].dataProvider()(0);
	auto sub = S222[0];
	//sub.at(0).at(0) = 1;
	//using eeee = decltype(sub.at(0));
	//using eeee2 = eeee::referenced_at_return_type;
	//double& ddd = sub.at(0).dataProvider()(0);
	//std::cout << "asfee: " << sdfjekn << std::endl;
	const auto& eeee = S222[0];
	//std::cout << "\t" << typeid(S222).name() << std::endl;
	//std::cout << "\t" << typeid(S222[0]).name() << std::endl;
	//std::cout << "\t" << typeid(S222[0][0]).name() << std::endl;
	//std::cout << "\t" << typeid(S222[0][0][0]).name() << std::endl;
	//std::cout << typeid(S222[0][0][0][0]).name() << std::endl;
	std::cout << T222[0] << std::endl;
	S222[0][1][2] = 44;
	S222[0][1][2] = std::numbers::pi;
	std::cout << S222[0] << std::endl;
	std::cout << T222[0] << std::endl;

	std::random_device rdev;
	std::default_random_engine engine(rdev());
	std::uniform_int_distribution d1(0, 1);
	std::uniform_int_distribution d2(0, 4);
	std::uniform_real_distribution<double> rd(0, 1);
	std::cout << lc::put_beside("orig: ", T222[0], " view: ", S222[0], " view^T: ", S222[0].transposed()) << std::endl;
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
	for (int i = 0; i < 10000000; ++i)
	{
		//T222[0][d1(engine)][d1(engine)] = rd(engine);
		d1(engine);
		d1(engine);
		rd(engine);
	}
	auto end = high_resolution_clock::now();
	std::cout << "took " << duration_cast<milliseconds>(end - start) << std::endl;
	start = high_resolution_clock::now();
	for (int i = 0; i < 10000000; ++i)
	{
		S222[0][d2(engine)][d2(engine)] = rd(engine);
	}
	end = high_resolution_clock::now();
	std::cout << "took " << duration_cast<milliseconds>(end - start) << std::endl;

	lc::put_beside({ "a", "b\ncc" });

	std::cout << lc::put_beside("orig: ", T222[0], " view: ", S222[0], " view^T: ", S222[0].transposed()) << std::endl;

	lc::TensorTraits<decltype(T222)>::TImpl<double>;
	decltype(T222)::TImpl<int>;
	//lc::TensorTraits<decltype(T222)>::TImpl
	std::iterator_traits<std::array<double, 3>::iterator>::difference_type;

	//__clang_version__;
	//std::cout << __clang_version__ << std::endl;

	return 0;
}