#pragma once

#include "Vec2.h"
#include "Transform.h"
#include "Mouse.h"

namespace ArEngine2D {
	class Camera
	{
	private:

		struct CamExtents
		{
			Vec2 topLeft;
			Vec2 botRight;
		};

	public:

		inline static float const* s_pWindowWidth{};
		inline static float const* s_pWindowHeight{};

	public:

		Camera();
		Camera(Vec2 const& loc, float scale = 1.f, float dir = 0.f);

		// users may not call this function
		static void InternalInitialization(float* pWindowWidth, float* pWindowHeight);

	public:

		void MoveTo(Vec2 const& where) noexcept;
		void Move(Vec2 const& delta) noexcept;
		void SetZoom(float toWhat) noexcept;
		void Zoom(float percent) noexcept;
		void SetDir(float toWhat) noexcept;
		void Rotate(float delta) noexcept;

		void Tran(Vec2& point) const noexcept;
		void RevTran(Vec2& point) const noexcept;
		Vec2 operator()(Vec2 const& point) const noexcept;
		Vec2 operator[](Vec2 const& point) const noexcept;

	private:

		void LazyEval() const noexcept;
		static float WindowWidth() noexcept;
		static float WindowHeight() noexcept;

	public:

		Vec2 const& Loc() const noexcept;
		float Dir() const noexcept;
		float Scale() const noexcept;
		Transform const& CurrTransform() const noexcept;
		Transform const& CurrTransformRev() const noexcept;
		CamExtents CalcExtents() const noexcept;

	private:
		Vec2 loc_;
		float dir_;
		float scale_;

		// lazy eval
		mutable bool bUpToDate_{};
		mutable Transform transform_{};
		mutable Transform revTransform_{};
	};

	class DraggableCamera : public Camera
	{
	public:

		DraggableCamera(Mouse& mouse, Vec2 const& loc = {}, float scale = 1.f, float dir = 0.f);

	public:

		void UpdateDrag(Key const& dragKey);
		void UpdateZoomUsingScrollWheel();

	public:

		void SetZoomSpeed(float newSpeed) noexcept;
		float ZoomSpeed() const noexcept;

	private:
		float zoomSpeed_{1.1f};
		Mouse& mouse_;

		bool bLanded_{};
		Vec2 mouseLandLoc_;
		Vec2 camLocCache_;
	};
}