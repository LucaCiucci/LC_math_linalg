#pragma once

#include "FixedTensorContainer.hpp"
#include "TensorImpl.hpp"

namespace lc
{
	
	template <class _Ty, TensorDim... _Dims>
	using Tensor = TensorImpl<FixedTensorContainer<TensorImpl, _Ty, TShape<_Dims...>>>;

}
