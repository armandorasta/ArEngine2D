#include "Camera.h"

namespace ArEngine2D {
	Camera Camera::s_CameraAtDefaultPosition = {};

	Camera::Camera() : Camera{{0.f, 0.f}, 1.f, 0.f} { }
	Camera::Camera(Vec2 const& loc, float scale, float dir)
		: loc_{loc}, scale_{scale}, dir_{dir} 
	{ }
	void Camera::InternalInitialization(float* pWindowWidth, float* pWindowHeight)
	{
		assert(s_pWindowWidth == nullptr and s_pWindowHeight == nullptr && "User tried to call Camera::InternalInitialization");
		s_pWindowWidth = pWindowWidth;
		s_pWindowHeight = pWindowHeight;
	}
	Camera const& Camera::CameraAtDefaultPosition() noexcept
	{
		return s_CameraAtDefaultPosition;
	}
	void Camera::MoveTo(Vec2 const& where) noexcept
	{
		bUpToDate_ = false;
		loc_ = where;
	}
	void Camera::Move(Vec2 const& delta) noexcept
	{
		//MoveTo(loc_ + delta.Rotated(dir_) / scale_);

		auto myMat{transform_.Matrix()};
		// remove any translation.
		myMat._31 = {};
		myMat._32 = {};
		Vec2 const actDel{myMat.TransformPoint(delta.ToD2DPoint())};
		MoveTo(loc_ + actDel);
	}
	void Camera::SetZoom(float toWhat) noexcept
	{
		bUpToDate_ = false;
		scale_ = toWhat;
	}
	void Camera::Zoom(float percent) noexcept
	{
		SetZoom(scale_ * percent);
	}
	void Camera::SetDir(float toWhat) noexcept
	{
		bUpToDate_ = false;
		dir_ = toWhat;
	}
	void Camera::Rotate(float delta) noexcept
	{
		SetDir(Util::WrapAngle(dir_ + delta));
	}
	void Camera::Tran(Vec2& point) const noexcept
	{
		point = operator()(point);
	}
	void Camera::RevTran(Vec2& point) const noexcept
	{
		point = operator[](point);
	}
	Vec2 Camera::operator()(Vec2 const& point) const noexcept
	{
		LazyEval();
		return transform_(point);
	}
	Vec2 Camera::operator[](Vec2 const& point) const noexcept
	{
		LazyEval();
		return revTransform_(point);
	}
	void Camera::LazyEval() const noexcept
	{
		if (bUpToDate_)
		{
			return;
		}

		auto const centerVec = Vec2{WindowWidth(), WindowHeight()} * 0.5f;
		transform_.Reset().Translate(-loc_).Scale(scale_).Rotate(-dir_).Translate(centerVec);
		revTransform_ = transform_.Inverted();
		bUpToDate_ = true;
	}
	float Camera::WindowWidth() noexcept
	{
		return *s_pWindowWidth;
	}
	float Camera::WindowHeight() noexcept
	{
		return *s_pWindowHeight;
	}
	Vec2 const& Camera::Loc() const noexcept
	{
		return loc_;
	}
	float Camera::Dir() const noexcept
	{
		return dir_;
	}
	float Camera::Scale() const noexcept
	{
		return scale_;
	}
	Transform const& Camera::CurrTransform() const noexcept
	{
		LazyEval();
		return transform_;
	}
	Transform const& Camera::CurrTransformRev() const noexcept
	{
		LazyEval();
		return revTransform_;
	}
	Camera::CamExtents Camera::CalcExtents() const noexcept
	{
		auto const vec = Vec2{WindowWidth(), WindowHeight()} * (0.5f / scale_);
		return {loc_ - vec, loc_ + vec};
	}
	DraggableCamera::DraggableCamera(Mouse& mouse, Vec2 const& loc, float scale, float dir)
		: mouse_{mouse}, Camera { loc, scale, dir }
	{ }
	void DraggableCamera::UpdateDrag(Key const& dragKey)
	{
		if (bLanded_)
		{
			if (dragKey.IsUp())
				// stopped dragging the camera
			{
				bLanded_ = false;
			}
			else // still dragging
			{
				auto myMat{CurrTransformRev().Matrix()};
				// remove the translation
				myMat._31 = { };
				myMat._32 = { };
				Vec2 const actDel{myMat.TransformPoint((mouseLandLoc_ - mouse_.loc).ToD2DPoint())};
				MoveTo(camLocCache_ + actDel);
			}
		}
		else
		{
			if (dragKey.IsPressed())
				// started dragging the camera
			{
				bLanded_ = true;
				mouseLandLoc_ = (mouse_.loc);
				camLocCache_ = Loc();
			}
		}
	}
	void DraggableCamera::UpdateZoomUsingScrollWheel()
	{
		mouse_.ForEachWheel([&](auto e) {
			if (e.IsUp())	Zoom(zoomSpeed_);
			else			Zoom(1.f / zoomSpeed_);
		});
	}
	void DraggableCamera::UpdateZoomUsing(Key const& zoomInKey, Key const& zoomOutKey)
	{ 
		if (zoomInKey.IsDown())
		{
			Zoom(zoomSpeed_);
		}
		else if (zoomOutKey.IsDown())
		{
			Zoom(1.f / zoomSpeed_);
		}
	}
	void DraggableCamera::SetZoomSpeed(float newSpeed) noexcept
	{ 
		zoomSpeed_ = newSpeed; 
	}
	float DraggableCamera::ZoomSpeed() const noexcept
	{ 
		return zoomSpeed_; 
	}
}