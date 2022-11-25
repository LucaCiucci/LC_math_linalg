


I found different problems across varius compilers.  

MSCV didn't like:
```cpp
struct FixedTensorContainer :
	FixedTensorContainerProvider<contains_variable_size(_shape)>::Container<_TImpl, Ty, _rank, _shape>
```
and tells there is a syntax error, but compiles replacing `contains_variable_size(_shape)` with `true/flase`, so we can't use the provider tecnique.

I tried template specializationlike
```cpp
template <template <class T> class _TImpl, class Ty, size_t _rank, TensorShape<_rank> _shape>
struct FixedTensorContainerData;

template <template <class T> class _TImpl, class Ty, size_t _rank, TensorShape<_rank> _shape>
	requires (contains_variable_size(_shape))
struct FixedTensorContainerData<_TImpl, Ty, _rank, _shape> {};

template <template <class T> class _TImpl, class Ty, size_t _rank, TensorShape<_rank> _shape>
	requires (!contains_variable_size(_shape))
struct FixedTensorContainerData<_TImpl, Ty, _rank, _shape> {};
```
But intellisense doesn't like template specialization when the `TensorShape<_rank>` is present among the template parameters (replacing `requires` with other tecniques such as a `bool` template parameter or `enable_if` dosn't change this fact).

I finally came up with the idea (after A LOT OF PAIN) of splitting the two `FixedTensorContainerData` definitions and using a `std::conditional` to choose between the two classes.

This problems and a lots of minor similar ones, took me a long time to fix... F**K MSVC/INTELLISENSE they always have some great surprise bugs!

CLang always works :wink: