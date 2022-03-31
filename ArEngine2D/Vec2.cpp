#include "Vec2.h"

#include <cmath>
#include <format>

namespace ArEngine2D {
	auto Vec2::Mag() const noexcept -> float
	{ 
		return std::sqrt(Mag2()); 
	}
	auto Vec2::Normalized() const noexcept -> self
	{ 
		return operator/(Mag()); 
	}
	void Vec2::Normalize() noexcept
	{ 
		operator=(Normalized()); 
	}
	auto Vec2::Rotated(float angle) const noexcept -> self
	{
		float fCos{ }, fSin{ };
		DirectX::XMScalarSinCos(&fSin, &fCos, angle);
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