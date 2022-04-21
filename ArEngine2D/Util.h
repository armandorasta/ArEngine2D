#pragma once

#include "FixedWindowsHeader.h"

#include <format>
#include <compare>
#include <concepts>
#include <string_view>
#include <numbers>


namespace ArEngine2D::Details {
	template <class TNum>
	concept ComparableToZero = requires (TNum lhs)
	{
		{ lhs < 0 } -> std::convertible_to<bool>;
		{ lhs > 0 } -> std::convertible_to<bool>;
		{ lhs == 0 } -> std::convertible_to<bool>;
	};
}

namespace ArEngine2D {
	/**
	 * @brief mostly contains constexpr versions of functions
	 * already in the standard library.
	*/
	class Util
	{
	public:

		Util() = delete;

	public:

		constexpr static auto Pi{std::numbers::pi_v<float>};
		constexpr static auto TwoPi{2.f * Pi};
		constexpr static auto HalfPi{Pi * 0.5f};

	public:

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
		template <Details::ComparableToZero TNum>
		constexpr static auto Abs(TNum num) noexcept
		{ return num > static_cast<TNum>(0) ? num : -num; }

		template <Details::ComparableToZero TNum>
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
		template <std::floating_point TNum>
		constexpr static auto FloatEq(TNum lhs, TNum rhs) noexcept -> bool
		{
			auto const unsignedDelta{Abs(lhs - rhs)};
			return unsignedDelta <= std::numeric_limits<TNum>::epsilon();
		}

		/**
		 * @brief a stable way for comparing two floating point numbers (strongly ordered).
		 * @param lhs => left hand side of the expression.
		 * @param rhs => right hand side of the expression.
		 * @return the order of the two numbers.
		*/
		template <std::floating_point TNum>
		constexpr static auto FloatCmp(TNum lhs, TNum rhs) noexcept -> std::strong_ordering
		{
			auto const signedDelta{lhs - rhs};
			auto const unsignedDelta{Abs(signedDelta)};

			if (unsignedDelta <= std::numeric_limits<TNum>::epsilon())
			{
				return std::strong_ordering::equal;
			}
			else return {
				signedDelta > static_cast<TNum>(0) ?
				std::strong_ordering::greater :
				std::strong_ordering::less
			};
		}

		/**
		 * @brief takes an angle in radians.
		 * @return the equivalent angle measured in degrees.
		*/
		template <std::floating_point TNum>
		constexpr static auto RadianToDegree(TNum angle)
		{
			return static_cast<TNum>(180) * angle / std::numbers::pi_v<TNum>;
		}

		/**
		 * @brief takes an angle in degrees.
		 * @return the equivalent angle measured in radians.
		*/
		template <std::floating_point TNum>
		constexpr static auto DegreeToRadian(TNum angle)
		{
			return std::numbers::pi_v<TNum> * angle / static_cast<TNum>(180);
		}

		/**
		 * @brief wraps an angle measured in radians to an unspecified range;
		 *        could be [0, 2pi) or [-pi, +pi).
		 * @return a wrapped version of the angle.
		*/
		template <std::floating_point TNum>
		constexpr auto WrapAngle(TNum angle)
		{
			// n which is (a * 2 * pi + b)
			// mul => a + (b / 2pi)
			auto const mul{(angle / std::numbers::pi_v<TNum>) * static_cast<TNum>(0.5)};
			// floorMul => a
			auto const floorMul{static_cast<std::int32_t>(mul)};
			// floorRemainder => b / 2pi
			auto const floorRemainder{mul - static_cast<TNum>(floorMul)};
			// return value => b * n #
			return floorRemainder * angle;
		}
	};
}