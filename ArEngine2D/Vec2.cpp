#include "Vec2.h"

#include <cmath>
#include <format>

namespace ArEngine2D {
	float Vec2::Dist(cref lhs, cref rhs)
	{
		return std::sqrt(Dist2(lhs, rhs));
	}
	auto Vec2::Mag() const noexcept -> float
	{ 
		return std::sqrt(Mag2()); 
	}
	auto Vec2::Normalized() const noexcept -> self
	{ 
		auto const mag{Mag()};
		return Util::FloatEq(mag, 0.f) ? Vec2{} : operator/(mag);
	}
	void Vec2::Normalize() noexcept
	{ 
		operator=(Normalized()); 
	}
	auto Vec2::Rotated(float angle) const noexcept -> self
	{
		// did not use SinCos because I am planning to switch to 
		// std::sin and std::cos in c++23
		float fCos{DirectX::XMScalarCos(angle)};
		float fSin{DirectX::XMScalarSin(angle)};
		return {
			x * fCos - y * fSin,
			x * fSin + y * fCos,
		};
	}
	void Vec2::Rotate(float angle) noexcept
	{
		operator=(Rotated(angle));
	}
	auto Vec2::ToString() const -> std::string
	{ 
		return std::format("({}, {})", x, y); 
	}
}