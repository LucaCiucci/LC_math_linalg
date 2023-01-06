#pragma once

namespace lc
{
	template <class T> auto sqr(T&& t) { return t * t; }
	template <class T> auto sqr(const T& t) { return t * t; }

}

