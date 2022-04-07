#include "Transform.h"

namespace ArEngine2D {
	Transform::Transform() noexcept 
	{ 
		Reset(); 
	}
	Transform::Transform(matto const& matrix) noexcept
		: mat_{matrix}
	{}
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
	Transform::self Transform::Inverted() const noexcept
	{
		auto t{*this};
		t.Invert();
		return t;
	}
	Transform::self& Transform::Set(matto const& matrix) noexcept
	{
		mat_ = matrix;
		return *this;
	}
	Transform::self& Transform::Append(matto const& matrix) noexcept
	{
		mat_ = mat_ * matrix;
		return *this;
	}
	Transform::self& Transform::Append(self const& that) noexcept
	{
		return Append(that.Matrix());
	}
	Transform::self& Transform::Prepend(matto const& matrix) noexcept
	{
		mat_ = matrix * mat_;
		return *this;
	}
	Transform::self& Transform::Prepend(self const& that) noexcept
	{
		return Prepend(that.mat_);
	}
	Transform::self Transform::operator>>(self const& rhs) const noexcept
	{
		return Transform{mat_ * rhs.mat_};
		return *this;
	}
	Vec2 Transform::operator()(Vec2 const& what) const noexcept
	{
		return mat_.TransformPoint(what.ToD2DPoint());
	}
	bool Transform::operator==(self const& rhs) const noexcept
	{
		auto const& omat{rhs.mat_};
		return {
			mat_._11 == omat._11 and mat_._12 == omat._12 and
			mat_._21 == omat._21 and mat_._22 == omat._22 and
			mat_._31 == omat._31 and mat_._32 == omat._32
		};
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