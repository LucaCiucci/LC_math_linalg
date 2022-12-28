#pragma once

#include <optional>
#include <array>
#include <span>

#include "TensorIndexShape.hpp"

namespace lc
{
	// indellisense has some problem with constexpr if, so we use this pattern instead

	template <size_t _rank2>
	struct TensorDataReshaper
	{
		template <class Ty>
		static constexpr void reshape_data(const std::span<size_t, _rank2>& shapeIn, const std::span<size_t, _rank2>& shapeOut, Ty* dataIn, Ty* dataOut) {
			const size_t nBlocksIn = shapeIn[0];
			const size_t nBlocksOut = shapeOut[0];
			const size_t nBlocks = std::min(nBlocksIn, nBlocksOut);

			const std::span<size_t, _rank2 - 1> subShapeIn(std::next(shapeIn.begin()), _rank2 - 1);
			const std::span<size_t, _rank2 - 1> subShapeOut(std::next(shapeOut.begin()), _rank2 - 1);

			const size_t inBlockSize = elemCount(subShapeIn);
			const size_t outBlockSize = elemCount(subShapeOut);

			if (outBlockSize == inBlockSize)
			{
				// --------------------------------
				//             SHIFT
				// --------------------------------

				// TODO this case might me included in one of the other cases
				// and might be unnecessary

				// examples:
				// 
				// CASE dataIn > dataOut (-> use LR)
				//   |dddd|dddd|dddd|dddd|
				// v   /    /    /    /
				// |xxxx|xxxx|xxxx|xxxx|----|----|----|
				// 
				// CASE dataIn < dataOut (-> use RL)
				// |dddd|dddd|dddd|dddd|
				//   v \    \    \    \
					//   |xxxx|xxxx|xxxx|xxxx|----|----|----|
					//
					// CASE dataIn = dataOut (-> use LR (or RL, it is the same))
					// |dddd|dddd|dddd|dddd|
					// v  |    |    |    |
					// |xxxx|xxxx|xxxx|xxxx|----|----|----|

				if (dataIn >= dataOut)
				{
					// ----------------
					//       LR
					// ----------------

					// Now we can move each block of the data
					//   |dddd|dddd|dddd|dddd|
					// v  1/   2/   3/   4/
					// |xxxx|xxxx|xxxx|xxxx|----|----|----|
					for (size_t blockIndex = 0; blockIndex < nBlocks; ++blockIndex)
					{
						Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
					}

					// now we fill the remaining blocks with the default value
					// |dddd|dddd|dddd|dddd|0000|0000|0000|
					for (size_t blockIndex = nBlocks; blockIndex < nBlocksOut; ++blockIndex)
					{
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						std::/*ranges::*/fill(outBlockData, outBlockData + outBlockSize, Ty()); // ! <- Ty might be non-copyable or copy might be sub-obtimal, use something better like STL would do
					}
				}
				else
				{
					// ----------------
					//       RL
					// ----------------

					// Now we can move each block of the data
					// |dddd|dddd|dddd|dddd|
					//   v1\   2\   3\   4\
						//   |xxxx|xxxx|xxxx|xxxx|----|----|----|
					for (size_t _blockIndex = 0; _blockIndex < nBlocks; ++_blockIndex)
					{
						const auto blockIndex = nBlocks - _blockIndex - 1;
						Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
					}

					// now we fill the remaining blocks with the default value
					// |dddd|dddd|dddd|dddd|0000|0000|0000|
					for (size_t blockIndex = nBlocks; blockIndex < nBlocksOut; ++blockIndex)
					{
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						std::/*ranges::*/fill(outBlockData, outBlockData + outBlockSize, Ty()); // ! <- Ty might be non-copyable or copy might be sub-obtimal, use something better like STL would do
					}
				}

				// end shift
				return;
			}

			if (outBlockSize <= inBlockSize)
			{
				// --------------------------------
				//           REDUCTION
				// --------------------------------

				// examples:
				// 
				// CASE dataIn > dataOut (-> LR)
				//   |ddddd|ddddd|ddddd|ddddd|
				// v    /    /    /    /
				// |xxxx|xxxx|xxxx|xxxx|----|----|----|
				// 
				// CASE dataIn_end < dataOut_end (-> RL)
				// |ddddd|ddddd|ddddd|ddddd|
				//      \     \    \     \  v
				//      |xxxx|xxxx|xxxx|xxxx|----|----|----|
				//
				// CASE dataIn < dataOut & dataIn_end > dataOut_end (-> (special case) + LR + RL)
				// |ddddd|ddddd|ddddd|ddddd|
				//   v \    \     |    / v
				//   |xxxx|xxxx|xxxx|xxxx|----|----|----|

				// all the cases can be threated in the same way by iterating
				// LR in the right part so that we do not overwrite the next block
				// and then iterate RL in the left part. Before doing that, we have
				// to take care of the special case that is the block that is contained in its source
				// (if any)

				// special case
				// |ddddd|ddddd|ddddd|ddddd|
				//     \    \     |    /
				//   |xxxx|xxxx|xxxx|xxxx|----|----|----|
				//                ^ special case
				std::optional<size_t> specialIndex;
				for (size_t blockIndex = 0; blockIndex < nBlocks; ++blockIndex)
				{
					Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
					Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
					Ty* const inBlockData_end = dataIn + (blockIndex + 1) * inBlockSize;
					Ty* const outBlockData_end = dataOut + (blockIndex + 1) * outBlockSize;

					if (outBlockData >= inBlockData && outBlockData_end <= inBlockData_end)
						TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData), specialIndex = blockIndex;
				}

				// LR
				// |ddddd|ddddd|ddddd|ddddd|
				//     \    \     |    /1
				//   |xxxx|xxxx|xxxx|xxxx|----|----|----|
				//                     ^-------->
				if (specialIndex || dataIn >= dataOut) {
					const size_t first_index = specialIndex ? (specialIndex.value() + 1) : 0;
					for (size_t blockIndex = first_index; blockIndex < nBlocks; ++blockIndex)
					{
						Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
					}
				}

				// RL
				// |ddddd|ddddd|ddddd|ddddd|
				//    2\   1\     |    /
				//   |xxxx|xxxx|xxxx|xxxx|----|----|----|
				//    <------^
				if (specialIndex || (dataOut + nBlocks * outBlockSize) >= (dataIn + nBlocks * inBlockSize))
				{
					const size_t _blockIndex_start = specialIndex ? specialIndex.value() : nBlocks;
					for (size_t _blockIndex = _blockIndex_start; _blockIndex > 0; --_blockIndex)
					{
						const size_t blockIndex = _blockIndex - 1;
						Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
						Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
						TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
					}
				}

				// now we fill the remaining blocks with the default value
				// |dddd|dddd|dddd|dddd|0000|0000|0000|
				for (size_t blockIndex = nBlocks; blockIndex < nBlocksOut; ++blockIndex)
				{
					Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
					std::/*ranges::*/fill(outBlockData, outBlockData + outBlockSize, Ty()); // ! <- Ty might be non-copyable or copy might be sub-obtimal, use something better like STL would do
				}

				// end reduction
				return;
			}

			if (outBlockSize >= inBlockSize)
			{
				// --------------------------------
				//           EXPANSION
				// --------------------------------

				// examples:
				// 
				// CASE ....
				//   |dddd|dddd|dddd|dddd|
				//     /    /    |     \
					// |xxxxx|xxxxx|xxxxx|xxxxx|----|----|----|

					// LR
				for (size_t blockIndex = 0; blockIndex < nBlocks; ++blockIndex)
				{
					Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
					Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
					Ty* const inBlockData_end = dataIn + (blockIndex + 1) * inBlockSize;
					Ty* const outBlockData_end = dataOut + (blockIndex + 1) * outBlockSize;

					if (outBlockData_end > inBlockData_end)
						break;

					TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
				}

				// RL
				for (size_t _blockIndex = nBlocks; _blockIndex > 0; --_blockIndex)
				{
					const size_t blockIndex = _blockIndex - 1;
					Ty* const inBlockData = dataIn + blockIndex * inBlockSize;
					Ty* const outBlockData = dataOut + blockIndex * outBlockSize;

					if (outBlockData < inBlockData)
						break;

					TensorDataReshaper<_rank2 - 1>::template reshape_data<Ty>(subShapeIn, subShapeOut, inBlockData, outBlockData);
				}

				// now we fill the remaining blocks with the default value
				// |dddd|dddd|dddd|dddd|0000|0000|0000|
				for (size_t blockIndex = nBlocks; blockIndex < nBlocksOut; ++blockIndex)
				{
					Ty* const outBlockData = dataOut + blockIndex * outBlockSize;
					std::/*ranges::*/fill(outBlockData, outBlockData + outBlockSize, Ty()); // ! <- Ty might be non-copyable or copy might be sub-obtimal, use something better like STL would do
				}

				// end expansion
				return;
			}

			// end big else
		}
	};

	template <>
	struct TensorDataReshaper<0>
	{
		template <class Ty>
		static constexpr void reshape_data(const std::span<size_t, 0>& shapeIn, const std::span<size_t, 0>& shapeOut, Ty* dataIn, Ty* dataOut) {
			// single elements

			std::swap(*dataIn, *dataOut);
		}
	};
}