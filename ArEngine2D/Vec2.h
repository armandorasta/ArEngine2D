#pragma once

#include "Util.h"

#include <DirectXMath.h>
#include <d2d1.h>
#include <compare>

namespace ArEngine2D {
	class Vec3;

	class Vec2
	{
	public:
		using self = Vec2;
		using cref = Vec2 const&;

		float x;
		float y;

		constexpr Vec2(self const&)				= default;
		constexpr Vec2(self&&)					= default;
		constexpr self& operator=(self const&)	= default;
		constexpr self& operator=(self&&)		= default;

		constexpr Vec2() : x{}, y{} { }
		constexpr Vec2(float x, float y) : x{x}, y{y} { }
		constexpr Vec2(D2D1_POINT_2F p) : x{p.x}, y{p.y} { }

		constexpr operator D2D1_POINT_2F() const 
		{ return {x, y}; }

		/** 
		* @brief creats a vector using a direction and a length.
		 * @param dir => the direction of the vector (angle in radians)
		 * @param len => the length of the vector (defaults to 1)
		 * @return <cos(dir), sin(dir)> * len
		 */
		constexpr static self FromDir(float dir, float len = 1.f)
		{
			return self{DirectX::XMScalarCos(dir), DirectX::XMScalarSin(dir)} * len;
		}

	public:

		/**
		 * @return the dot product between the current vector and rhs.
		*/
		constexpr auto Dot(cref rhs) const noexcept -> float
		{ return x * rhs.x + y * rhs.y; }

		/**
		 * @return the length of the resulting vector of the cross product between the current vector and rhs.
		*/
		constexpr auto Cross(cref rhs) const noexcept -> float
		{ return x * rhs.y - rhs.x * y; }

		/**
		 * @return the square of the length of the vector (much faster than Mag()).
		*/
		constexpr auto Mag2() const noexcept -> float
		{ return Dot(*this); }

		/**
		 * @return the length of the vector.
		*/
		auto Mag() const noexcept -> float;
		
		/**
		 * @return a normlized version of the current vector (non-mutating).
		*/
		auto Normalized() const noexcept ->self;
		
		/**
		 * @brief normalizes the current vector (mutating).
		*/
		void Normalize() noexcept;

		/**
		 * @param angle => the angle (in rad) to rotate the vector by.
		 * @return a rotated version of the current vector (non-mutating).
		*/
		auto Rotated(float angle) const noexcept -> self;

		/**
		 * @brief rotates the vector inplace (mutating).
		 * @param angle => the angle (in rad) to rotate the vector by.
		*/
		void Rotate(float angle) noexcept;

		/**
		 * @return "(x, y)"
		*/
		auto ToString() const->std::string;

		/**
		 * @brief does a better job than == at checking this.
		 * @param rhs => the vector to be compared with.
		 * @return true if the two vectors are close enough to be considered equal,
		 * false otherwise.
		*/
		constexpr auto Equals(cref rhs) const noexcept -> bool
		{
			return Util::FloatEq(x, rhs.x) and Util::FloatEq(y, rhs.y);
		}

		/**
		 * @brief this is more raliable at checking equality than <=>.
		 * @param rhs => the vector to be compared with.
		 * @return the order (strong unlike the built in) of two vectors.
		*/
		constexpr auto Compare(cref rhs) const noexcept -> std::strong_ordering
		{
			constexpr auto compareFloats = [](float lhs, float rhs) constexpr {
				if (lhs > rhs) return std::strong_ordering::greater;
				if (lhs < rhs) return std::strong_ordering::less;
				return std::strong_ordering::equal;
			};

			if (Util::FloatEq(x, rhs.x))
			{
				return compareFloats(y, rhs.y);
			}
			else
			{
				return compareFloats(x, rhs.x);
			}
		}

		constexpr D2D1_POINT_2F ToD2DPoint() const 
		{ return operator D2D1_POINT_2F(); }

	public: // operators
		
		constexpr auto operator+() const noexcept -> cref {
			return *this;
		}
		constexpr auto operator-() const noexcept -> self {
			return {-x, -y};
		}
		constexpr auto operator+(cref rhs) const noexcept -> self {
			return {x + rhs.x, y + rhs.y};
		}
		constexpr auto operator-(cref rhs) const noexcept -> self {
			return {x - rhs.x, y - rhs.y};
		}
		constexpr auto operator*(cref rhs) const noexcept -> float {
			return Dot(rhs);
		}
		constexpr auto operator*(float rhs) const noexcept -> self {
			return {x * rhs, y * rhs};
		}
		constexpr friend auto operator*(float lhs, self rhs) noexcept -> self {
			return rhs * lhs;
		}
		constexpr auto operator/(float rhs) const noexcept -> self {
			return operator*(1.f / rhs);
		}

		constexpr self& operator+=(cref rhs) noexcept
		{ return operator=(operator+(rhs)); }
		constexpr self& operator-=(cref rhs) noexcept
		{ return operator=(operator-(rhs)); }
		constexpr self& operator*=(float rhs) noexcept
		{ return operator=(operator*(rhs)); }
		constexpr self& operator/=(float rhs) noexcept
		{ return operator=(operator/(rhs)); }

		constexpr auto operator <=>(cref rhs) const->std::partial_ordering = default;

	public:
		const static self Zero;
		const static self UnitX;
		const static self UnitY;
		const static self One;
	};

	inline const Vec2::self Vec2::Zero{0.f, 0.f};
	inline const Vec2::self Vec2::UnitX{1.f, 0.f};
	inline const Vec2::self Vec2::UnitY{0.f, 1.f};
	inline const Vec2::self Vec2::One{1.f, 1.f};

}

namespace std {
	template <>
	struct formatter<ArEngine2D::Vec2> : public formatter<std::string> 
	{
		auto format(ArEngine2D::Vec2 const& vec, format_context context) 
		{
			auto const str{vec.ToString()};
			auto in_iterator{std::begin(str)};
			auto out_iterator{context.out()};
			for (auto const end{std::end(str)}; in_iterator != end;) 
			{
				*out_iterator++ = *in_iterator++;
			}
			return out_iterator;
		}
	};
}