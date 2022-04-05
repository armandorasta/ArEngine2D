#include "Transform.h"

namespace ArEngine2D {
	Transform::Transform() noexcept 
	{ 
		Reset(); 
	}
	Transform::self& Transform::Translate(Vec2 const& myVec) noexcept
	{
		mat_ = mat_ * matto::Translation(myVec.x, myVec.y);
		return *this;
	}
	Transform::self& Transform::Scale(Vec2 const& myVec, Vec2 const& myCenter) noexcept
	{
		mat_ = mat_ * matto::Scale(myVec.x, myVec.y, myCenter.ToD2DPoint());
		return *this;
	}
	Transform::self& Transform::Scale(float scalar, Vec2 const& myCenter) noexcept
	{
		return Scale({scalar, scalar}, myCenter);
	}
	Transform::self& Transform::Rotate(float angle, Vec2 const& myCenter) noexcept
	{
		mat_ = mat_ * matto::Rotation(angle * 180.f / 3.1415926f, myCenter.ToD2DPoint());
		return *this;
	}
	Transform::self& Transform::Reset() noexcept
	{ 
		mat_ = matto::Identity();
		return *this;
	}
	Transform::self& Transform::Invert() noexcept
	{
		assert(IsInvertible() && "Tried inverting an uninvertible Transform.");
		mat_.Invert();
		return *this;
	}
	bool Transform::IsInvertible() const noexcept
	{ 
		return mat_.IsInvertible(); 
	}
	D2D1::Matrix3x2F const& Transform::Matrix() const noexcept
	{ 
		return mat_; 
	}
}