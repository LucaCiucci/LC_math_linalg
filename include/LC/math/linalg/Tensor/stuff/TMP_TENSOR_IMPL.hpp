#pragma once

// TODO a better check for other analyzers
// TODO remove always true, just for development purposes
// TODO make a development macro?
#if defined(__INTELLISENSE__) || true

namespace lc
{
	// A fake tensor implementation for intellisense template instantiation models
	template <class T> class TMP_TENSOR_IMPL : public T {};
}

#endif