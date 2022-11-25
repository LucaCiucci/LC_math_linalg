#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <concepts>
#include <vector>
#include <algorithm>

namespace lc
{
	// TODO sposta
	// TODO meglio i vincoli
	template <std::ranges::range T>
	auto max(const T& range)
	{
		using R = std::remove_cvref_t<decltype(*range.begin())>;
		//std::max

		// TODO or size() ??? maybe conditional with concept? or empty??
		if (std::distance(range.begin(), range.end()) == 0)
		{
			// TODO or error? or infinity? vedi cosa fa std::max, che dereferenzia il risultato di std::max_element ma è end se è vuoto!
			return R();
		}

		return *std::max_element(range.begin(), range.end());
	}

	// TODO sposta!!!
	template <class T>
	concept OStreamable = requires (std::ostream os, T t) { { os << t } -> std::convertible_to<std::ostream&>; };
	
	// TODO move on anothe library
	// TODO sposta!!!
	template <OStreamable T>
	std::string stream_to_string(const T& streamable)
	{
		std::ostringstream ss;
		ss << streamable;
		return ss.str();
	}

	std::string filled_string(const std::string& c, int N);

	// TODO sposta!!!
	// TODO or list???
	std::vector<std::string> split_lines(const std::string& str);

	enum class TerminalHAlign
	{
		Left,
		Right
	};

	// place side by side
	std::string put_beside(const std::initializer_list<std::string>& pieces, TerminalHAlign align = TerminalHAlign::Left);

	// place side by side
	std::string put_beside(const std::vector<std::string>& pieces, TerminalHAlign align = TerminalHAlign::Left);

	template <std::ranges::range T>
	std::string put_beside(const T& pieces, TerminalHAlign align = TerminalHAlign::Left)
	{
		std::vector<std::string> v;
		for (const auto& p : pieces)
			v.push_back(p);
		return put_beside(v, align);
	}

	template <OStreamable... Ts>
	std::string put_beside(const Ts&... streamables)
	{
		return put_beside({ stream_to_string(streamables)... });
	}

	template <OStreamable... Ts>
	std::string put_beside(TerminalHAlign align = TerminalHAlign::Left, const Ts&... streamables)
	{
		return put_beside({ stream_to_string(streamables)... }, align);
	}
}