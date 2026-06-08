#include "GuiButton.h"
#include "Camera.h"

namespace ArGui {
	GuiButton::GuiButton(Vec2 const& loc, std::string_view text, float width, float height) noexcept 
		: rect_{loc, ClampSize(width), ClampSize(height)}, text_{text} 
	{
		rect_.RelCenter(rect_.GetTopLeft());
		UpdateTextSize();
	}

	void GuiButton::SetLoc(Vec2 newLoc) noexcept
	{ 
		rect_.RelCenter(newLoc); 
	}

	GuiStatus GuiButton::SetText(std::string_view newText)
	{
		text_ = newText;
		UpdateTextSize();
		return currStatus_;
	}

	GuiStatus GuiButton::SetWidth(float newWidth) noexcept
	{
		UpdateStatus(newWidth == 0, GuiStatus::SizeTooSmall);
		rect_.SetWidth(ClampSize(newWidth));
		UpdateTextSize();
		return currStatus_;
	}

	GuiStatus GuiButton::SetHeight(float newHeight) noexcept
	{
		UpdateStatus(newHeight == 0, GuiStatus::SizeTooSmall);
		rect_.SetHeight(ClampSize(newHeight));
		UpdateTextSize();
		return currStatus_;
	}

	void GuiButton::Draw(Grafix& gfx, Camera const& cam,
		ColorF const& bgColor, ColorF const& lineColor, ColorF const& textColor) noexcept
	{ 
		if (!Visible)
		{
			return;
		}

		rect_.Fill(gfx, cam, bgColor);
		rect_.Draw(gfx, cam, lineColor, 2.f);

		// TODO: handle text here.
		if (false && !text_.empty())
		{
			auto center{rect_.GetCenter()};
			// offset = scalar * len
			// scalar = (offset / len) = (char offset)
			auto const scalar{Util::AsciiToRenderedTextRatio() * textSize_};
			center.x -= (text_.size() * 0.5f) * scalar;
			center.y -= scalar;
			gfx.DrawString(center, text_, textColor, textSize_);
		}
	}

	void GuiButton::Update(Mouse const& mouse, Camera const& cam) noexcept
	{
		if (!Enabled)
		{
			return;
		}

		auto const camMouseLoc{cam[mouse.loc]};
		auto const bContainedLastFrame{rect_.Contains(mouseLocLastFrame_)};
		auto const bContainedCurrFrame{rect_.Contains(camMouseLoc)};
		auto const bMouseDown{mouse.left.IsDown()};
		if (bContainedCurrFrame)
		{
			if (bMouseClicked_ = mouse.left.IsPressed();
				bMouseClicked_) // Toggle hold when ONLY the press happens within range
			{
				bMouseHeld_ = true;
			}

			if (bMouseReleased_ = mouse.left.IsJustReleased();
				bMouseReleased_)
			{
				bMouseHeld_ = false;
			}

			bMouseEntered_ = !bContainedLastFrame;
			bMouseLeft_ = false;
			bMouseHovered_ = true;
		}
		else
		{
			// bMouseReleased_ must be updated before bMouseHeld_

			// all clicks out of range don't count
			bMouseClicked_ = false;

			// Can't use bMouseLastFrame_
			bMouseReleased_ = bMouseHeld_ && !bMouseDown;
			bMouseHeld_ = bMouseHeld_ && bMouseDown;

			bMouseEntered_ = false;
			bMouseLeft_ = bContainedLastFrame;
			bMouseHovered_ = false;
		}

		mouseLocLastFrame_ = camMouseLoc;
	}

	GuiElementType GuiButton::GetType() const noexcept
	{ return GuiElementType::Button; }

	Vec2 GuiButton::GetLoc() const noexcept
	{ return rect_.GetCenter(); }

	std::string_view GuiButton::GetText() const noexcept
	{ return text_; }

	float GuiButton::GetWidth() const noexcept
	{ return rect_.GetWidth(); }

	float GuiButton::GetHeight() const noexcept
	{ return rect_.GetHeight(); }

	GuiStatus GuiButton::GetStatus() const noexcept
	{ return currStatus_; }

	GuiRectF GuiButton::GetRectF(Camera const& cam) const noexcept
	{
		auto r{rect_};
		r.ScaleTopLeft(cam.Scale());
		r.RelTopLeft(cam(rect_.GetTopLeft()));
		return r;
	}

	bool GuiButton::IsMouseClicked() const noexcept
	{
		return bMouseClicked_;
	}
	bool GuiButton::IsMouseHeld() const noexcept
	{
		return bMouseHeld_;
	}
	bool GuiButton::IsMouseJustReleased() const noexcept
	{
		return bMouseReleased_;
	}
	bool GuiButton::IsMouseEntered() const noexcept
	{
		return bMouseEntered_;
	}
	bool GuiButton::IsMouseHovered() const noexcept
	{
		return bMouseHovered_;
	}
	bool GuiButton::IsMouseLeft() const noexcept
	{
		return bMouseLeft_;
	}


	void GuiButton::UpdateStatus(bool condition, GuiStatus ifTrueOtherwiseFine) noexcept
	{ currStatus_ = !condition ? ifTrueOtherwiseFine : GuiStatus::Fine; }

	void GuiButton::ResetStatus() noexcept
	{ currStatus_ = GuiStatus::Fine; }

	void GuiButton::UpdateTextSize() noexcept
	{
		// size = w * scalar / (len * 0.1)
		// scalar = (len * size) / w => (ratio_of(string, rect_width))
		textSize_ = rect_.GetWidth() * Util::RenderedTextToPixelRatio() / (text_.size() * 0.1f);
	}

	float GuiButton::ClampSize(float size) const noexcept
	{ return std::clamp(size, sc_MinSize, sc_MaxSize); }
}

