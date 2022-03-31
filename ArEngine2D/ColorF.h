#pragma once

#include "Util.h"

#include <DirectXMath.h>

#include <compare>
#include <cstdint>
#include <algorithm>
#include <d2d1.h>

namespace ArEngine2D
{
	class ColorF
	{
	public:

		using self = ColorF;
		using cref = ColorF const&;

	public:

		float r;
		float g;
		float b;
		float a;

		/**
		 * @brief defaulted (does not initialize anything)
		*/
		constexpr ColorF() : r{}, g{}, b{}, a{} { }
		constexpr ColorF(ColorF const&)				= default;
		constexpr ColorF(ColorF&&)					= default;
		constexpr ColorF& operator=(ColorF const&)	= default;
		constexpr ColorF& operator=(ColorF&&)		= default;

		constexpr ColorF(float r, float g, float b, float a = 1.f) :
			r{r}, g{g}, b{b}, a{a}
		{ }

		/**
		 * @brief useful for converting hex to ColorF
		 * @param rbg => the hex value that contains rbg ex. 0xFFFF00.
		 * @param a => the alpha (defaults to 1)
		*/
		constexpr ColorF(std::uint32_t rbg, float a = 1.f) : 
			r{ByteToFloat((rbg >> 16) & 0xFF)},
			g{ByteToFloat((rbg >>  8) & 0xFF)},
			b{ByteToFloat((rbg >>  0) & 0xFF)},
			a{a}
		{ }

		constexpr ColorF(D2D1_COLOR_F d2dColor) : 
			r{d2dColor.r}, g{d2dColor.g},
			b{d2dColor.b}, a{d2dColor.a}
		{}

		constexpr operator D2D1_COLOR_F() const 
		{ return {r, g, b, a}; }

	public:

		/**
		 * @brief does not clamp anything.
		 * @return a scaled version of the current color with the alpha channel untouched. (non-mutating)
		*/
		constexpr self MulNoAlpha(float rhs) const
		{ return {r * rhs, g * rhs, b * rhs, a}; }

		/**
		 * @brief does a better job of checking for equality than operator==.
		 * @param rhs => the right hand side of the expression.
		 * @return true if the two colors are close enough to be considered equal, false otherwise.
		*/
		constexpr auto Equals(cref rhs) const -> bool
		{
			return {
				Util::FloatEq(r, rhs.r) and
				Util::FloatEq(g, rhs.g) and
				Util::FloatEq(b, rhs.b) and 
				Util::FloatEq(a, rhs.a)
			};
		}

		/**
		 * @brief strongly ordered (unlike operator <=>).
		 * @param rhs => the right hand side of the expression.
		 * @return => the order of the two colors.
		*/
		constexpr auto Compare(cref rhs) const -> std::strong_ordering
		{
			std::strong_ordering compRes{};

			if (compRes = Util::FloatCmp(r, rhs.r);
				compRes != std::strong_ordering::equal)
			{
				return compRes;
			}
			if (compRes = Util::FloatCmp(g, rhs.g);
				compRes != std::strong_ordering::equal)
			{
				return compRes;
			}
			if (compRes = Util::FloatCmp(b, rhs.b);
				compRes != std::strong_ordering::equal)
			{
				return compRes;
			}
			else return Util::FloatCmp(a, rhs.a);
		}

		/**
		 * @brief clamps all fields to [0, 1] (non-mutating)
		 * @return a new vector with all of its fields clamped to [0, 1].
		*/
		constexpr auto Saturated() const -> self
		{
			return {
				std::clamp(r, 0.f, 1.f),
				std::clamp(g, 0.f, 1.f),
				std::clamp(b, 0.f, 1.f),
				std::clamp(a, 0.f, 1.f),
			};
		}

		/**
		 * @brief clamps all fields to [0, 1] (mutating)
		*/
		constexpr void Staturate()
		{ operator=(Saturated()); }

		/**
		 * @brief inverts the color but does nothing to the alpha (non-mutating).
		 * @return an inverted version of the current color.
		*/
		constexpr auto Inverted() const -> self
		{ return {1.f - r, 1.f - g, 1.f - b, a}; }

		/**
		 * @brief inverts the color but does nothing to the alpha (mutating).
		*/
		constexpr void Invert()
		{ operator=(Inverted()); }

		/**
		 * @brief a way to convert ColorF to hex.
		 * @return a binary word with each color channel taking to bits.
		*/
		constexpr auto ToDword() const -> std::uint32_t
		{
			return {
				(FloatToByte(r) << 16) |
				(FloatToByte(g) <<  8) |
				(FloatToByte(b) <<  0)
			};
		}

		/**
		 * @brief convertion to DirectX::XMFLOAT4
		 * @return DirectX::XMFLOAT4{r, g, b, a}
		*/
		constexpr auto ToD2DColor() const -> D2D1_COLOR_F
		{ return {r, g, b, a}; }

	public:

		/**
		 * @brief maps [0.0f, 1.0f] to [0UI32, 255UI32].
		 * @param fNum => the number to be converted.
		 * @return => the byte equivalent of the number.
		*/
		constexpr static auto FloatToByte(float fNum) -> std::uint32_t
		{ return static_cast<std::uint32_t>(fNum * 255); }

		/**
		 * @brief maps [0UI32, 255UI32] to [0.0f, 1.0f].
		 * @param bNum => the number to be converted.
		 * @return => the floating point equivalent of the number.
		*/
		constexpr static auto ByteToFloat(std::uint32_t bNum) -> float
		{ return bNum / 255.f; }

	public: // operators

		constexpr auto operator<=>(cref rhs) const->std::partial_ordering = default;

		constexpr auto operator+() const -> cref {
			return *this;
		}
		constexpr auto operator-() const -> self {
			return {-r, -g, -b, a};
		}
		constexpr auto operator+(cref rhs) const -> self {
			return {r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a};
		}
		constexpr auto operator-(cref rhs) const -> self {
			return operator+(-rhs);
		}
		constexpr auto operator*(cref rhs) const -> self {
			return {r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a};
		}
		constexpr auto operator*(float rhs) const -> self {
			return {r * rhs, g * rhs, b * rhs, a * rhs};
		}
		constexpr friend auto operator*(float lhs, self rhs) -> self {
			return rhs * lhs;
		}
		constexpr auto operator/(float rhs) const -> self {
			return operator*(1.f / rhs);
		}

		constexpr self& operator+=(cref rhs)  { return operator=(operator+(rhs)); }
		constexpr self& operator-=(cref rhs)  { return operator=(operator-(rhs)); }
		constexpr self& operator*=(cref rhs)  { return operator=(operator*(rhs)); }
		constexpr self& operator/=(float rhs) { return operator=(operator/(rhs)); }
	};

	namespace Colors {
		constexpr ColorF BLACK{0x000000};
		constexpr ColorF WHITE{0xFFFFFF};
		constexpr ColorF RED{0xFF0000};
		constexpr ColorF GREEN{0x00FF00};
		constexpr ColorF BLUE{0x0000FF};
		constexpr ColorF YELLOW{0xFFFF00};
		constexpr ColorF MAGENTA{0xFF00FF};
		constexpr ColorF CYAN{0x00FFFF};
		constexpr ColorF ORANGE{0xFFA500};
		constexpr ColorF GRAY{WHITE.MulNoAlpha(.75f)};
		constexpr ColorF DARK_GRAY{GRAY.MulNoAlpha(.5f)};
		constexpr ColorF DARK_RED{RED.MulNoAlpha(.5f)};
		constexpr ColorF DARK_GREEN{GREEN.MulNoAlpha(.5f)};
		constexpr ColorF DARK_BLUE{BLUE.MulNoAlpha(.5f)};
		constexpr ColorF DARK_YELLOW{YELLOW.MulNoAlpha(.5f)};
		constexpr ColorF DARK_MAGENTA{MAGENTA.MulNoAlpha(.5f)};
		constexpr ColorF DARK_CYAN{CYAN.MulNoAlpha(.5f)};
	}
}