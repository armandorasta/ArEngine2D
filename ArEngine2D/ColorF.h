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

		constexpr auto operator+() const -> cref 
		{ return *this; }
		constexpr auto operator-() const -> self 
		{ return {-r, -g, -b, a}; }
		constexpr auto operator+(cref rhs) const -> self 
		{ return {r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a}; }
		constexpr auto operator-(cref rhs) const -> self 
		{ return operator+(-rhs); }
		constexpr auto operator*(cref rhs) const -> self 
		{ return {r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a}; }
		constexpr auto operator*(float rhs) const -> self 
		{ return {r * rhs, g * rhs, b * rhs, a * rhs}; }
		constexpr friend auto operator*(float lhs, self rhs) -> self 
		{ return rhs * lhs; }
		constexpr auto operator/(float rhs) const -> self 
		{ return operator*(1.f / rhs); }

		constexpr self& operator+=(cref rhs)  
		{ return operator=(operator+(rhs)); }
		constexpr self& operator-=(cref rhs)  
		{ return operator=(operator-(rhs)); }
		constexpr self& operator*=(cref rhs)  
		{ return operator=(operator*(rhs)); }
		constexpr self& operator/=(float rhs) 
		{ return operator=(operator/(rhs)); }
	};

	/**
	 * @brief stolen directly from ::D2D1::ColorF::Enum, thank you.
	*/
	namespace Colors {
		constexpr ColorF
		AliceBlue = 0xF0F8FF,
		AntiqueWhite = 0xFAEBD7,
		Aqua = 0x00FFFF,
		Aquamarine = 0x7FFFD4,
		Azure = 0xF0FFFF,
		Beige = 0xF5F5DC,
		Bisque = 0xFFE4C4,
		Black = 0x000000,
		BlanchedAlmond = 0xFFEBCD,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		BurlyWood = 0xDEB887,
		CadetBlue = 0x5F9EA0,
		Chartreuse = 0x7FFF00,
		Chocolate = 0xD2691E,
		Coral = 0xFF7F50,
		CornflowerBlue = 0x6495ED,
		Cornsilk = 0xFFF8DC,
		Crimson = 0xDC143C,
		Cyan = 0x00FFFF,
		DarkBlue = 0x00008B,
		DarkCyan = 0x008B8B,
		DarkGoldenrod = 0xB8860B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkKhaki = 0xBDB76B,
		DarkMagenta = 0x8B008B,
		DarkOliveGreen = 0x556B2F,
		DarkOrange = 0xFF8C00,
		DarkOrchid = 0x9932CC,
		DarkRed = 0x8B0000,
		DarkSalmon = 0xE9967A,
		DarkSeaGreen = 0x8FBC8F,
		DarkSlateBlue = 0x483D8B,
		DarkSlateGray = 0x2F4F4F,
		DarkTurquoise = 0x00CED1,
		DarkViolet = 0x9400D3,
		DeepPink = 0xFF1493,
		DeepSkyBlue = 0x00BFFF,
		DimGray = 0x696969,
		DodgerBlue = 0x1E90FF,
		Firebrick = 0xB22222,
		FloralWhite = 0xFFFAF0,
		ForestGreen = 0x228B22,
		Fuchsia = 0xFF00FF,
		Gainsboro = 0xDCDCDC,
		GhostWhite = 0xF8F8FF,
		Gold = 0xFFD700,
		Goldenrod = 0xDAA520,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		Honeydew = 0xF0FFF0,
		HotPink = 0xFF69B4,
		IndianRed = 0xCD5C5C,
		Indigo = 0x4B0082,
		Ivory = 0xFFFFF0,
		Khaki = 0xF0E68C,
		Lavender = 0xE6E6FA,
		LavenderBlush = 0xFFF0F5,
		LawnGreen = 0x7CFC00,
		LemonChiffon = 0xFFFACD,
		LightBlue = 0xADD8E6,
		LightCoral = 0xF08080,
		LightCyan = 0xE0FFFF,
		LightGoldenrodYellow = 0xFAFAD2,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSalmon = 0xFFA07A,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightSlateGray = 0x778899,
		LightSteelBlue = 0xB0C4DE,
		LightYellow = 0xFFFFE0,
		Lime = 0x00FF00,
		LimeGreen = 0x32CD32,
		Linen = 0xFAF0E6,
		Magenta = 0xFF00FF,
		Maroon = 0x800000,
		MediumAquamarine = 0x66CDAA,
		MediumBlue = 0x0000CD,
		MediumOrchid = 0xBA55D3,
		MediumPurple = 0x9370DB,
		MediumSeaGreen = 0x3CB371,
		MediumSlateBlue = 0x7B68EE,
		MediumSpringGreen = 0x00FA9A,
		MediumTurquoise = 0x48D1CC,
		MediumVioletRed = 0xC71585,
		MidnightBlue = 0x191970,
		MintCream = 0xF5FFFA,
		MistyRose = 0xFFE4E1,
		Moccasin = 0xFFE4B5,
		NavajoWhite = 0xFFDEAD,
		Navy = 0x000080,
		OldLace = 0xFDF5E6,
		Olive = 0x808000,
		OliveDrab = 0x6B8E23,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Orchid = 0xDA70D6,
		PaleGoldenrod = 0xEEE8AA,
		PaleGreen = 0x98FB98,
		PaleTurquoise = 0xAFEEEE,
		PaleVioletRed = 0xDB7093,
		PapayaWhip = 0xFFEFD5,
		PeachPuff = 0xFFDAB9,
		Peru = 0xCD853F,
		Pink = 0xFFC0CB,
		Plum = 0xDDA0DD,
		PowderBlue = 0xB0E0E6,
		Purple = 0x800080,
		Red = 0xFF0000,
		RosyBrown = 0xBC8F8F,
		RoyalBlue = 0x4169E1,
		SaddleBrown = 0x8B4513,
		Salmon = 0xFA8072,
		SandyBrown = 0xF4A460,
		SeaGreen = 0x2E8B57,
		SeaShell = 0xFFF5EE,
		Sienna = 0xA0522D,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		SlateBlue = 0x6A5ACD,
		SlateGray = 0x708090,
		Snow = 0xFFFAFA,
		SpringGreen = 0x00FF7F,
		SteelBlue = 0x4682B4,
		Tan = 0xD2B48C,
		Teal = 0x008080,
		Thistle = 0xD8BFD8,
		Tomato = 0xFF6347,
		Turquoise = 0x40E0D0,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Yellow = 0xFFFF00,
		YellowGreen = 0x9ACD32;
	}
}