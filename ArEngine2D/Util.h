#pragma once

#include "FixedWindowsHeader.h"

#include <format>
#include <compare>
#include <concepts>
#include <string_view>
#include <numbers>



namespace ArEngine2D {
	namespace Details {
		template <class TNum>
		concept Number = std::is_arithmetic_v<TNum>;
	}

	/**
	 * @brief mostly contains constexpr versions of functions
	 * already in the standard library.
	*/
	class Util
	{
	private:
		constexpr static auto sc_TextCharPerPixel{0.15f};
		constexpr static auto sc_AsciiCharPerTextChar{0.5f};

	public:

		Util() = delete;

	public:
		// no need for std::numbers; your boy got it in his head.
		constexpr static auto Pi{3.1415926535897932384626433832795f};
		constexpr static auto TwoPi{2.f * Pi};
		constexpr static auto HalfPi{Pi * 0.5f};

	public:

		[[nodiscard]]
		constexpr static float RenderedTextToPixelRatio() noexcept
		{ return sc_TextCharPerPixel; }

		[[nodiscard]]
		constexpr static float AsciiToRenderedTextRatio() noexcept
		{ return sc_AsciiCharPerTextChar; }

		/**
		 * @brief outputs a formatted string to the debug console.
		 * @param fmt => the format.
		 * @param ...args => the formating arguments.
		*/
		template <class... TArgs>
		static void WriteDebug(std::string_view fmt, TArgs&&... args)
		{ OutputDebugStringA(std::format(fmt, std::forward<TArgs>(args)...).data()); }

		/**
		 * @brief removes the sign of anything comparable to zero.
		 * @param num => the signed (or unsigned) number.
		 * @return => a version always compares greater than or equal to zero.
		*/
		template <Details::Number TNum>
		constexpr static auto Abs(TNum num)
		{ return num > static_cast<TNum>(0) ? num : -num; }

		/**
		 * @return 1 if the number is positive, 0 if it's zero, -1 if it's negative.
		*/
		template <Details::Number TNum>
		constexpr static auto Sign(TNum num) noexcept
		{
			constexpr auto Zero{static_cast<TNum>(0)};
			constexpr auto One{static_cast<TNum>(1)};
			constexpr auto NegativeOne{static_cast<TNum>(-1)};
			return (num == Zero) ? Zero : (num > Zero ? One : NegativeOne);
		}

		/**
		 * @brief a stable way for checking for floating point equality.
		 * @param lhs => left hand side of the expression.
		 * @param rhs => right hand side of the expression.
		 * @return true if the two numbers are close enough to be considered equal, false otherwise.
		*/
		constexpr static auto FloatEq(float lhs, float rhs) -> bool
		{
			auto const unsignedDelta{Abs(lhs - rhs)};
			return unsignedDelta <= std::numeric_limits<float>::epsilon();
		}

		/**
		 * @brief a stable way for comparing two floating point numbers (strongly ordered).
		 * @param lhs => left hand side of the expression.
		 * @param rhs => right hand side of the expression.
		 * @return the order of the two numbers.
		*/
		constexpr static auto FloatCmp(float lhs, float rhs) -> std::strong_ordering
		{
			auto const signedDelta{lhs - rhs};
			auto const unsignedDelta{Abs(signedDelta)};

			if (unsignedDelta <= std::numeric_limits<float>::epsilon())
			{
				return std::strong_ordering::equal;
			}
			else if (signedDelta > static_cast<float>(0))
			{
				return std::strong_ordering::greater;
			}
			else return std::strong_ordering::less;
		}

		/**
		 * @brief takes an angle in radians.
		 * @return the equivalent angle measured in degrees.
		*/
		constexpr static float RadianToDegree(float angle)
		{ return static_cast<float>(180) * angle / Pi; }

		/**
		 * @brief takes an angle in degrees.
		 * @return the equivalent angle measured in radians.
		*/
		constexpr static float DegreeToRadian(float angle)
		{ return Pi * angle / 180.f; }

		/**
		 * @brief wraps an angle measured in radians to an unspecified range;
		 *        could be [0, 2pi) or [-pi, +pi).
		 * @return a wrapped version of the angle.
		*/
		static float WrapAngle(float angle)
		{ return std::fmod(angle, Util::TwoPi); }
	};
}