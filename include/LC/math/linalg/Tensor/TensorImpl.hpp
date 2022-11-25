#pragma once


namespace lc
{
	
	template <class Base> // TODO concept
	class TensorImpl : public Base
	{
	public:

		using Base::Base;
		using Base::operator=; // ? maybe better reimplement

	private:
		
	};
	
	template <class T> struct is_Tensor : std::false_type {};
	template <class T> struct is_Tensor<TensorImpl<T>> : std::true_type {};
	template <class T> concept TensorType = is_Tensor<T>::value;

	template <class T> concept StaticRankTensor = TensorType<T> && (T::has_fixed_rank());
	template <class T, size_t N> concept RankNTensor = TensorType<T> && (T::rank() == N);
	template <class T, size_t N> concept RankGTNTensor = TensorType<T> && (T::rank() > N);
	template <class T, size_t N> concept RankLTNTensor = TensorType<T> && (T::rank() < N);
	template <class T> concept Rank1Tensor = RankNTensor<T, 1>;
	template <class T> concept Rank2Tensor = RankNTensor<T, 2>;
	template <class T> concept Rank3Tensor = RankNTensor<T, 3>;
}