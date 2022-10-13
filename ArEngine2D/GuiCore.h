#pragma once
#include "Engine.h"
#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <cassert>

// text size 1 is 6 by 8 pixels

#define ARGUI_ASSERT(_condition, _message) assert((_condition) && _message)

#ifdef NDEBG // release mode
#define ARGUI_NOEXCEPT_NDEBUG	noexcept
#define ARGUI_CONSTEXPR_NDEBUG	constexpr
#else // ^^^ NDEBUG vvv !NDEBUG
#define ARGUI_NOEXCEPT_NDEBUG
#define ARGUI_CONSTEXPR_NDEBUG	inline
#endif // ^^^ NDEBUG


namespace ArGui {
	using namespace ArEngine2D;

	using Uint = std::uint32_t;
	using Int = std::int32_t;

	inline constexpr auto UintMax{std::numeric_limits<Uint>::max()};
	inline constexpr auto UintMin{std::numeric_limits<Uint>::min()};
}

