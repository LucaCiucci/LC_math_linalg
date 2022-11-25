#pragma once

#include <ostream>
#include <ranges>
#include <string>

namespace lc
{
	template <std::ranges::range R>
	void stream_vector_like(std::ostream& os, const R& r, const std::string& opener = "(", const std::string& closer = ")", const std::string& separator = ", ")
	{
		// TODO optional constant width

		os << opener;
		auto it = r.begin();
		if (it != r.end())
		{
			os << *it;
			++it;
		}
		for (; it != r.end(); ++it)
			os << separator << *it;
		os << closer;
	}}