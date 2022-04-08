#pragma once

#include "ImplUtil.h"
#include "Util.h"
#include "Transform.h"

namespace ArEngine2D {
	class Surface
	{ 
	private:
		using self = Surface;

	public:

		Surface(self const&)		 = default;
		Surface(self&&)				 = default;
		self& operator=(self const&) = default;
		self& operator=(self&&)		 = default;

		Surface(float width, float height, Transform const& transform) noexcept;

	public:

		void SetTransform(Transform const& toWhat) noexcept;
		Transform const& CurrTransform() const noexcept;

		float Width() const noexcept;
		float Height() const noexcept;

	private:
		float width_;
		float height_;
		Transform tran_;
	};
}

