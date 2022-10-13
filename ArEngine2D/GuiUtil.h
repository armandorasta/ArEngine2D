#pragma once

#include "GuiCore.h"

namespace ArGui {
	namespace Con {
		template <class T>
		concept Number = std::integral<T> || std::floating_point<T>;
	}

	class Util
	{
	private:
		// multiply by this to convert from rendered characters count to pixel count.
		constexpr static auto sc_RenderedCharPerPixel{0.15f};

		// multiply by this to convert from rendered to ascii.
		constexpr static auto sc_AsciiCharPerRenderedChar{0.5f};

	public:
		[[nodiscard]]
		constexpr static float RenderedTextToPixelRatio() noexcept
		{ return sc_RenderedCharPerPixel; }

		[[nodiscard]]
		constexpr static float AsciiToRenderedTextRatio() noexcept
		{ return sc_AsciiCharPerRenderedChar; }

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
}