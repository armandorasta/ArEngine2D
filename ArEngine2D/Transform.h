#pragma once

#include <d2d1.h>
#include "Vec2.h"

namespace ArEngine2D {
	class Transform
	{
	private:
		using self = Transform;
		using matto = D2D1::Matrix3x2F;

	public:

		Transform() noexcept;
		Transform(self const& that)			= default;
		Transform(self&& that)				= default;
		self& operator=(self const& that)	= default;
		self& operator=(self&& that)		= default;

		Transform(matto const& matrix) noexcept;

	public:

		self& Translate(Vec2 const& myVec) noexcept;
		self& Scale(Vec2 const& myVec, Vec2 const& myCenter = { }) noexcept;
		self& Scale(float scalar, Vec2 const& myCenter = { }) noexcept;
		self& Rotate(float angle, Vec2 const& myCenter = { }) noexcept;
		self& Reset() noexcept;
		self& Invert() noexcept;
		self Inverted() const noexcept;
		self& Set(matto const& matrix) noexcept;
		self& Append(matto const& matrix) noexcept;
		self& Append(self const& that) noexcept;
		self& Prepend(matto const& matrix) noexcept;
		self& Prepend(self const& that) noexcept;

	public:

		self operator>>(self const& rhs) const noexcept;
		Vec2 operator()(Vec2 const& what) const noexcept;
		bool operator==(self const& rhs) const noexcept;

	public:

		bool IsInvertible() const noexcept;
		D2D1::Matrix3x2F const& Matrix() const noexcept;

	private:
		D2D1::Matrix3x2F mat_;
	};
}

