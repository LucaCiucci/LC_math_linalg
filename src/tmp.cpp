/*LC_NOTICE_BEGIN
LC_NOTICE_END*/

#if false

#include <vector>

//#include 
#include <LC/tmp.hpp>

#include <ranges>

namespace lc
{
	std::vector<std::string> split_lines(const std::string& str)
	{
		// TODO more efficient without stringstream

		// https://stackoverflow.com/questions/13172158/c-split-string-by-line
		std::vector<std::string> result;
		std::istringstream ss(str);
		for (std::string line; std::getline(ss, line, '\n');)
			result.push_back(line);
		return result;
	}

	std::string filled_string(const std::string& c, int N)
	{
		std::string result;
		for (int i = 0; i < N; ++i)
			result += c;
		return result;
	}

	std::string put_beside(const std::initializer_list<std::string>& pieces, TerminalHAlign align)
	{
		return put_beside(std::vector<std::string>{pieces}, align);
	}

	std::string put_beside(const std::vector<std::string>& pieces, TerminalHAlign align)
	{
		//using std::list;
		using std::string;
		using std::vector;
		using std::views::transform;
		using namespace std::string_literals;

		vector<vector<string>> piecesLines; piecesLines.reserve(pieces.size());
		auto linesCount = [](const vector<string>& rows) -> size_t { return rows.size(); };
		for (const auto& piece : pieces)
			piecesLines.push_back(split_lines(piece));

		vector<size_t> lenghts(pieces.size(), 0);
		for (size_t i = 0; i < pieces.size(); ++i)
			for (const auto& line : piecesLines[i])
				lenghts[i] = std::max(lenghts[i], line.length());

		string result;
		const auto nLines = max(piecesLines | transform(linesCount));
		for (size_t n = 0; n < nLines; ++n)
		{
			for (size_t i = 0; i < piecesLines.size(); ++i)
			{
				const auto& lines = piecesLines[i];

				// TODO unicode?
				size_t printedChars = n < lines.size() ? lines[n].length() : 0;

				if (align == TerminalHAlign::Right)
					result += filled_string(" ", lenghts[i] - printedChars);
				result += (n < lines.size() ? lines[n] : ""s);
				if (align == TerminalHAlign::Left)
					result += filled_string(" ", lenghts[i] - printedChars);
			}

			if (n + 1 < nLines)
				result += "\n";
		}

		return result;
	}
}

#endif