#pragma once

#include "ColorF.h"

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <algorithm>
#include <source_location>

namespace ArFac {
	using namespace ArEngine2D;

	class Block
	{
	public:
		using ID = std::int32_t;

		enum class Type : ID
		{
			Floor = 0,
			Belt = 1,
			Welder = 2,
			Pusher = 3,
			Sensor = 4,
			Wire = 5,
			Producer = 6,
		};

	private:
		struct BlockInfo
		{
			Type type;
			std::string str;
			char ch;
			ID id;
		};

	private:
		// not inline so, I don't have to recompile every time I add a new type
		static std::vector<BlockInfo> const sc_Blocks;

	public:

		// convertion functions
		static ID CharToID(char ch);
		static char IDToChar(ID id);
		static ID TypeToID(Type type);
		static Type IDToType(ID id);
		static std::string_view TypeToString(Type type);
		static Type StringToType(std::string_view str);

		// encapsulated constants
		static ID MinBlockID() noexcept;
		static ID MaxBlockID() noexcept;

	private:
		template <class TVal, std::invocable<BlockInfo const&> Callable>
		static BlockInfo const& FindHelper(TVal&& value, Callable&& proj,
			std::source_location debugInfo = std::source_location::current())
		{
			// I have no idea why std::find is not noexcept?
			if (auto const it{std::ranges::find(sc_Blocks, value, proj)};
				it != sc_Blocks.end())
			{
				return *it;
			}
			throw std::exception{debugInfo.function_name()};
		}
	};
}