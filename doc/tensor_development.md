# `Tensor` development

## Rationale

- **easy to use/import**
- **fast**: even if the implementation is complex, compiles must be able to easily optimize it
- **pay for what you use**: avoid unnecessary resource usage, take advantage of inlining and compile time evaluations to reduce data carried around
- **modularized**: You may not want to include all the features, split files
- **support for C++20 modules**
- **use modern features** to make code simple

---

$$
\text{\verb!T[i][j][k]!} \equiv T_{i,j,k}
$$

$$
\text{\verb!T[i][j][k]!} = \text{\verb!(T[i])[j][k]!} \equiv T_{i,j,k} = (T_{i,:,:})_{j,k} \; \Rightarrow \; \text{\verb!T[i]!} \equiv T_{i,:,:}
$$

## Target Usage

Here, we present some ideas on how we want tensors to behave.

### Static shape
Declaration for fixed size tensor (statically allocated):
```cpp
// declare tensor with shape (3, 3) i.e. a 3x3 matrix
Tensor<double, 3, 3> M;

// arbitrary shapes are possibles
Tensor<double, 2, 3, 4> T;
```
Some aliases:
```cpp
Vector<3> a;        // same as Tensor<double, 3>
Vector<3, float> a; // same as Tensor<float, 3>
Vector3d a;         // same as Tensor<double, 3>
// ...

Matrix<3, 3> M;     // same as Tensor<double, 3, 3>
Mat<3, 3> M;        // same as Tensor<double, 3, 3>
Mat3x3 M;           // same as Tensor<double, 3, 3>
Mat3x3<float> M;    // same as Tensor<float, 3, 3>
// ...
```

Partial dynamic size
```cpp
// Tensor 2x3x4x5, with index 1 and 3 having dynamic sizes
Tensor<double, 2, dyn_size, 4, dyn_size> T(3, 5);
// note that in this way we will not store nor carry around
// 4 index sizes but only 2 and the others are kept track of at
// compile time!

// change the size of the index 1
T.reshape<1>(5);
// now the tensor is 2x5x4x5

T.reshape(8, 9);
// now the tensor is 2x8x4x9

// or maybe call ir resize?
T.resize(4); // Error: only one index given but there are two free indexes
```

> ::warning::  
> use negative size to mark upper/lower index?  
> might be a useless effort

Some aliases
```cpp
DenseVector; // Vector<*, dyn_size>
DenseMatrix; // Mat<dyn_size, dyn_size, *>
```

```cpp
// static nDim implicit (double) tensor declaration
// (might not be very useful apart from some quick tests)
DenseTensor T({
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
});
// this will be archived using template deduction guidelines
```

### Dynamic shape
```cpp
DynamicTensor T(2, 3, 4); // tensor with shape (2,3,4)
T.reshape(2, 2); // Now a 2x2 matrix
T.reshape({2, 3}); // Now a 2x3 matrix
T.reshape(some_iterable); // something more dynamic
```

### Sparse

> TODO

### Access

Access elements
```cpp
// access elements with checks
double& element = M.at(1, 1);
double& element = M(1, 1);

// access elements without checks
// but the size of "{...}" have to be checkable at
// compile time using constexpr (and inline ?)
// if not checkable at compile time, it will not be done!
// for performance reasons
double& element = M[{1, 1}];
```

Access sub-tensor
```cpp
auto row = M.at(1);
auto row = M(1);
auto row = M[1];

// the type of `row` is unspecified, it might be a reference or an object
// that behaves like a reference, in this case it will not be copyable:
auto row2 = row;      // ERROR
Tensor<2> row2 = row; // OK
// the scope of row must be smaller than the original tensor, this is
// guaranteed if the tensor is a local variable because of non-copyability/movability
// of `row`, but in other cases is up to the user not to do bad things
// this is ok, it's just like iterators

// row can be used as a tensor "of references", you can do
// usual tensor operations with it

// direct operations on sub/rows:
// assign the values of row #2 to the ones of row #1
M[0] = M(1); // OK

// in the same way:
auto subMatrix = M.sub({ i_min, i_max }, { j_min, j_max });
auto subMatrix = M.sub({ i_min, i_max }/*, j implicitly "all"*/);

// if you want to pick specific lines/rows and/or have them reordered:
auto subMatrix = M.sub_view(
	{ 0, 2 },   // skip row with index 1
	{ 1, 2, 0 } // sort columns in a different manner
);
/*
Example:
/ 1 2 3 \      M.sub_view({0,2},{1,2,0})       / 2 3 1 \
| 4 5 6 |  ->          same as             ->  \ 8 9 0 /
\ 7 8 9 /     M.sub_view(lc::all,{1,2,0})
                  .sub_view({0,2})
*/

// see stateless lambdas
// https://stackoverflow.com/questions/5849059/lambda-expressions-as-class-template-parameters
// for dynamic dense lambda will be a parameter
auto t = M.sub_view<decltype([](size_t i, size_t j) {
		return { j, i };
	})>(3, 3); // <- 3, 3 is the final size
// equivalent to (but might not be as efficient as):
auto t = M.transposed();
```

Additional features for vectors
```cpp
// Note: Vector<2, double> is the same as Tensor<double, 2>
Vector<3> v;

// get a sub-vector of (x, z), it will behave like sub-tensors
// so that it can be used just like glsl sub-vectors
v.xz() // behaves like a "vector of references" with size = 2

double d = v.dot(v);           // OK
double d = v * v;              // ERROR: this is not the dot product!
double d = v.transposed() * v; // OK actually it could return something as Tensor<double> or Tensor<double, 1>, but can decay to double if conversion is required
```

```cpp
DenseVector<T> v;
// same as
Vector<lc::dyn_size> v;
```

```cpp
// Note: will not be very efficient
GenericTensor<double> T(2, 3, 4); // <- 2x3x4 number of index can change!

// change it into a 2x2 matrix:
T.reshape(2, 2);

// access operations will return a "generic sub-tensor reference" object
// that can be converted to the elements type but runtime checks will be performed
// it might use runtime polymorphism
```


for very specific use cases, vector of references are allowed
```cpp
// this will behave pretty much like `Tensor<double, ...>`
Tensor<lc::RefTo<double>, ...> T;
```

concepts
```cpp
template <class T>
concept TensorType; // use sfinae with a private signature type inside the implementation

template <TensorType T> concept FixedSizeTensor;
template <TensorType T> concept ContigousDataTensor; // has to provide strides
// ...
```


## Implementation

The general idea is that the *implementation* of *mathematical methods* is **separate** from the implementation of the *data storage and access*, this to avoid rewriting math. methods multiple times for each kind of tensor, for example:
```cpp
template <
	// the implementation of the data holder and other specific stuff
	template <class CT> Container,
	// The type of the elements
	class T,
	// the final type, if given, this will be used for some return types
	// such as operator+=
	// users will use this parameter if they want to inherit from
	// tensor to add their own stuff
	class ConcreteType = Itself
	>
requires TensorContainer<Container> && std::is_base_of<TensorImpl, ConcreteType>
class TensorImpl : public Container<GetConcreteType<TensorImpl, ConcreteType>>
{
public:

	// we want to ensure that this class is only an implementation
	// and no additional data is required
	~TensorImpl() {
		// we place this check inside the destructor since
		// placing it in the class definition scope will not allow us
		// to access the complete type "TensorImpl<Container>"
		static_assert(sizeof(TensorImpl) == sizeof(Container));
		// TODO: maybe better remove this and enforce it in an explicit test?
	}

	// inherit constructors, effectively making this class
	// the same of base
	using Container::Container;

	// any function that doesn't require to access the 
	constexpr Tensor& fill(const T& value);

	// we use C++20 features such as concepts to enable/disable
	// specific functionalities
	constexpr auto xy() requires (FixedSizeTensor<TensorImpl> ? StaticShape<TensorImpl> == { 1 });
}
```
Notice that the function `xy()` cna be implemented in base classes, but we use C++20's concepts to place all these functions in a unique class end enabling them when necessary. Without concepts it would be more esoteric, for example:
```cpp
template <...>
class TensorBasicFunctions : Container<...> { ... };

template <...>
class TensorAdditionalFunctions : TensorBasicFunctions<...> { ... };
// vectors
template <...>
class TensorAdditionalFunctions<..., N> : /*other fancy inheritance levels to define vector properties depending on their size, using this presented pattern*/ { ... };
// matrices
template <...>
class TensorAdditionalFunctions<..., N> : /*other fancy inheritance levels to define matrix properties depending on their size, using this presented pattern*/ { ... };

template <...>
class TensorImpl : TensorAdditionalFunctions<...> { ... };
```

Unfortunately, this will lead to a very complex design using [this kind of pettern](https://stackoverflow.com/questions/66668579/is-it-possible-to-access-child-types-in-c-using-crtp), also not very stable.  
Another option is to use this different pattern:
```cpp

template <
	// the concrete implementation
	template <class Container>
	class TImpl
	... // other params
	>
struct SomeTensorContainer
{
	// Now we can return arbitrary containers with the required implementation!
	TImpl<SomeOtherContainer<TImpl, ...>> foo();
	// ...
};

template <class Container, class Ty>
struct TensorImpl : Container
{
	// ...
};
```

> :warning:  
> At an high level, tensors are not defined recursively, for example:
> ```cpp
> template <..., size_t N, size_t... NN>
> class TensorImpl : std::array<TensorImpl<..., NN>, N> { ... };
> ```
> or similar because this limits the internal operation a lot: for example it could become kinda like `vector<vector<vector<...>>>` and it will loose both performance and solidity (every row sub tensor might become of different size unexpectedly if used incorrectly). Instead, containers implementations might be recursively defined, for example `FixedTensorContainer` (or better call `StaticSizeTensorContainer`???)


## TODOs

- [ ] why not proposing a standard:
  - too much arbitrary choices in the library
  - very big
  - programming is about math, but since from a tensor follows a lots of math functions (either internal or external to the tensor class), with a lot of possible implementations, it is better to leave it as a library to allow it to grow: "*what's better than perfect? Standard!*" but for a library that can be extended, enhanced and customized, a library i frefearable rather than a static bunch of code.
- [ ] separate the pure tensor implementation and basic functions from the bigger ones, users might <ins>just want to use a vector or a matrix</ins> and avoid including a large series of large files!
- [ ] dire che per ora solo c++20 per i requires, altrimenti diventa complicatissimo, poi in futuro forse supporto per versioni precedenti.
- [ ] potrebbe essere una brutta idea per il debug, ma forse alcune classi intermedie per adesemptio (oppositable, addable, etc.), ma forse meglio lasciarle come requires e trova un modo di determinare quando abilitarle o forzare la disabilitazione.

```cpp
// this is ok, why?
double foo() { return 42; }
const double& ff = foo();

// also this is ok, even if the iterator returns a value
// and not a reference
for (const auto& r : fltc)
	for (auto& e : r)
		e = 1;

// in fact, this is also ok in the dame way
auto bb = fltc.begin();
const auto& a = *bb; // questo?!?!??!?
```

$T_{i, free, k, free}$
```cpp
T[i][free][j][free]; //molto difficile da fare, cosa sarebbe T[i][free]?
T(i, free, j, free); // più facile da implementare
```

TODO non copyable


Here is a simple footnote[^clang-bug]. With some additional text after it.

[^clang-bug]: See
My reference.
ciso a tuttu

