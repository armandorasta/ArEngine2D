#pragma once

#include "GuiCore.h"

ARGUI_BEGIN_NAMESPACE

namespace Con {
	template <class T>
	concept Number = std::integral<T> || std::floating_point<T>;
}

class Util
{
private:
	constexpr static auto sc_TextCharPerPixel{0.15f};
	constexpr static auto sc_AsciiCharPerTextChar{0.5f};

public:
	[[nodiscard]] 
	constexpr static float RenderedTextToPixelRatio() noexcept
	{ return sc_TextCharPerPixel; }

	[[nodiscard]]
	constexpr static float AsciiToRenderedTextRatio() noexcept
	{ return sc_AsciiCharPerTextChar; }

	[[nodiscard]]
	constexpr static bool AABB(Vec2 const& tl0, Vec2 const& br0, Vec2 const& tl1, Vec2 const& br1) noexcept
	{
		return {
			tl0.x <= br1.x && tl0.x >= br1.x &&
			tl0.y <= br1.y && tl0.y >= br1.y
		};
	}

	[[nodiscard]]
	constexpr static bool IsPointInRect(Vec2 const& p, Vec2 const& rtl, Vec2 const& rbr)
	{
		return (p.x > rtl.x && p.x < rbr.x) && (p.y > rtl.y && p.y < rbr.y);
	}
};

ARGUI_END_NAMESPACE