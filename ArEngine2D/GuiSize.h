#pragma once

#include "GuiCore.h"

namespace ArGui {
	class GuiSize
	{
	public:
		constexpr GuiSize(Uint x, Uint y) noexcept : x{x}, y{y}
		{
		}

	public:

		constexpr Uint Mag() const noexcept
		{ return x * y; }

		constexpr void Flip() noexcept
		{ std::swap(x, y); }

		constexpr Vec2 ToVec2() const noexcept
		{ return {static_cast<float>(x), static_cast<float>(y)}; }

	public:
		constexpr GuiSize const& operator+() const noexcept
		{ return *this; }
		constexpr GuiSize operator+(GuiSize const& rhs) const noexcept
		{
			ARGUI_ASSERT((UintMax - x > rhs.x) && (UintMax - y > rhs.y), "Overflow on operator+");
			return {x + rhs.x, y + rhs.y};
		}
		constexpr GuiSize operator-(GuiSize const& rhs) const noexcept
		{
			ARGUI_ASSERT(rhs.x > x || rhs.y > y, "Underflow");
			return {x - rhs.x, y - rhs.y};
		}
		constexpr GuiSize operator*(float rhs) const noexcept
		{
			ARGUI_ASSERT(x > (UintMax / rhs) && y > (UintMax / rhs), "Overflow on operator*");
			return {static_cast<Uint>(x * rhs), static_cast<Uint>(y * rhs)};
		}
		constexpr friend GuiSize operator*(float lhs, GuiSize const& rhs) noexcept
		{ return rhs * lhs; }
		constexpr GuiSize operator/(float rhs) const noexcept
		{ return operator*(1.f / rhs); }

		constexpr GuiSize& operator+=(GuiSize const& rhs) noexcept
		{ return operator=(operator+(rhs)); }
		constexpr GuiSize& operator-=(GuiSize const& rhs) noexcept
		{ return operator=(operator-(rhs)); }
		constexpr GuiSize& operator*=(float rhs) noexcept
		{ return operator=(operator*(rhs)); }
		constexpr GuiSize& operator/=(float rhs) noexcept
		{ return operator=(operator/(rhs)); }

	public:
		Uint x;
		Uint y;
	};
}