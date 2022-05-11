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

#define ARGUI_					::ArGui::
#define ARGUI_BEGIN_NAMESPACE	namespace ArGui {
#define ARGUI_END_NAMESPACE		}
#define ARGUI_ASSERT(_condition, _message) assert((_condition) && _message)


ARGUI_BEGIN_NAMESPACE
using namespace ArEngine2D;

using Uint = std::uint32_t;
using Sint = std::int32_t;

constexpr auto UintMax{std::numeric_limits<Uint>::max()};
constexpr auto UintMin{std::numeric_limits<Uint>::min()};
ARGUI_END_NAMESPACE


