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

	/**
	 * @brief stolen directly from ::D2D1::ColorF::Enum, thank you.
	*/
	namespace Colors {
		constexpr ::ArEngine2D::ColorF
		ALICE_BLUE = 0xF0F8FF,
		ANTIQUE_WHITE = 0xFAEBD7,
		AQUA = 0x00FFFF,
		AQUAMARINE = 0x7FFFD4,
		AZURE = 0xF0FFFF,
		BEIGE = 0xF5F5DC,
		BISQUE = 0xFFE4C4,
		BLACK = 0x000000,
		BLANCHED_ALMOND = 0xFFEBCD,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		BURLY_WOOD = 0xDEB887,
		CADET_BLUE = 0x5F9EA0,
		CHARTREUSE = 0x7FFF00,
		CHOCOLATE = 0xD2691E,
		CORAL = 0xFF7F50,
		CORNFLOWER_BLUE = 0x6495ED,
		CORNSILK = 0xFFF8DC,
		CRIMSON = 0xDC143C,
		CYAN = 0x00FFFF,
		DARK_BLUE = 0x00008B,
		DARK_CYAN = 0x008B8B,
		DARK_GOLDENROD = 0xB8860B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_KHAKI = 0xBDB76B,
		DARK_MAGENTA = 0x8B008B,
		DARK_OLIVEGREEN = 0x556B2F,
		DARK_ORANGE = 0xFF8C00,
		DARK_ORCHID = 0x9932CC,
		DARK_RED = 0x8B0000,
		DARK_SALMON = 0xE9967A,
		DARK_SEAGREEN = 0x8FBC8F,
		DARK_SLATEBLUE = 0x483D8B,
		DARK_SLATEGRAY = 0x2F4F4F,
		DARK_TURQUOISE = 0x00CED1,
		DARK_VIOLET = 0x9400D3,
		DEEP_PINK = 0xFF1493,
		DEEP_SKY_BLUE = 0x00BFFF,
		DIM_GRAY = 0x696969,
		DODGER_BLUE = 0x1E90FF,
		FIREBRICK = 0xB22222,
		FLORAL_WHITE = 0xFFFAF0,
		FOREST_GREEN = 0x228B22,
		FUCHSIA = 0xFF00FF,
		GAINSBORO = 0xDCDCDC,
		GHOST_WHITE = 0xF8F8FF,
		GOLD = 0xFFD700,
		GOLDENROD = 0xDAA520,
		GRAY = 0x808080,
		GREEN = 0x008000,
		GREEN_YELLOW = 0xADFF2F,
		HONEYDEW = 0xF0FFF0,
		HOT_PINK = 0xFF69B4,
		INDIANRED = 0xCD5C5C,
		INDIGO = 0x4B0082,
		IVORY = 0xFFFFF0,
		KHAKI = 0xF0E68C,
		LAVENDER = 0xE6E6FA,
		LAVENDER_BLUSH = 0xFFF0F5,
		LAWN_GREEN = 0x7CFC00,
		LEMON_CHIFFON = 0xFFFACD,
		LIGHT_BLUE = 0xADD8E6,
		LIGHT_CORAL = 0xF08080,
		LIGHT_CYAN = 0xE0FFFF,
		LIGHT_GOLDENRODYELLOW = 0xFAFAD2,
		LIGHT_GREEN = 0x90EE90,
		LIGHT_GRAY = 0xD3D3D3,
		LIGHT_PINK = 0xFFB6C1,
		LIGHT_SALMON = 0xFFA07A,
		LIGHT_SEAGREEN = 0x20B2AA,
		LIGHT_SKYBLUE = 0x87CEFA,
		LIGHT_SLATEGRAY = 0x778899,
		LIGHT_STEELBLUE = 0xB0C4DE,
		LIGHT_YELLOW = 0xFFFFE0,
		LIME = 0x00FF00,
		LIME_GREEN = 0x32CD32,
		LINEN = 0xFAF0E6,
		MAGENTA = 0xFF00FF,
		MAROON = 0x800000,
		MEDIUM_AQUAMARINE = 0x66CDAA,
		MEDIUM_BLUE = 0x0000CD,
		MEDIUM_ORCHID = 0xBA55D3,
		MEDIUM_PURPLE = 0x9370DB,
		MEDIUM_SEAGREEN = 0x3CB371,
		MEDIUM_SLATEBLUE = 0x7B68EE,
		MEDIUM_SPRINGGREEN = 0x00FA9A,
		MEDIUM_TURQUOISE = 0x48D1CC,
		MEDIUM_VIOLETRED = 0xC71585,
		MIDNIGHT_BLUE = 0x191970,
		MINT_CREAM = 0xF5FFFA,
		MISTY_ROSE = 0xFFE4E1,
		MOCCASIN = 0xFFE4B5,
		NAVAJO_WHITE = 0xFFDEAD,
		NAVY = 0x000080,
		OLD_LACE = 0xFDF5E6,
		OLIVE = 0x808000,
		OLIVE_DRAB = 0x6B8E23,
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		ORCHID = 0xDA70D6,
		PALE_GOLDEN_ROD = 0xEEE8AA,
		PALE_GREEN = 0x98FB98,
		PALE_TURQUOISE = 0xAFEEEE,
		PALE_VIOLETRED = 0xDB7093,
		PAPAYA_WHIP = 0xFFEFD5,
		PEACH_PUFF = 0xFFDAB9,
		PERU = 0xCD853F,
		PINK = 0xFFC0CB,
		PLUM = 0xDDA0DD,
		POWDER_BLUE = 0xB0E0E6,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		ROSY_BROWN = 0xBC8F8F,
		ROYAL_BLUE = 0x4169E1,
		SADDLE_BROWN = 0x8B4513,
		SALMON = 0xFA8072,
		SANDY_BROWN = 0xF4A460,
		SEA_GREEN = 0x2E8B57,
		SEA_SHELL = 0xFFF5EE,
		SIENNA = 0xA0522D,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SLATE_BLUE = 0x6A5ACD,
		SLATE_GRAY = 0x708090,
		SNOW = 0xFFFAFA,
		SPRING_GREEN = 0x00FF7F,
		STEEL_BLUE = 0x4682B4,
		TAN = 0xD2B48C,
		TEAL = 0x008080,
		THISTLE = 0xD8BFD8,
		TOMATO = 0xFF6347,
		TURQUOISE = 0x40E0D0,
		VIOLET = 0xEE82EE,
		WHEAT = 0xF5DEB3,
		WHITE = 0xFFFFFF,
		WHITE_SMOKE = 0xF5F5F5,
		YELLOW = 0xFFFF00,
		YELLOW_GREEN = 0x9ACD32;
	}
}