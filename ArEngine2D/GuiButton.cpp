#include "GuiButton.h"
#include "Camera.h"

namespace ArGui {
	GuiButton::GuiButton(Vec2 const& loc, std::string_view text, float width, float height,
		ColorF const& activeTextCol, ColorF const& activeBGCol, ColorF const& activeLineCol) noexcept :
		rect_{loc, ClampSize(width), ClampSize(height)},
		text_{text}, textCol_{activeTextCol}, bgCol_{activeBGCol}, lineCol_{activeLineCol}
	{
		rect_.RelCenter(rect_.GetTopLeft());
		UpdateTextSize();
	}

	void GuiButton::SetLoc(Vec2 const& newLoc) noexcept
	{ rect_.RelCenter(newLoc); }

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

	void GuiButton::SetBGColor(ColorF const& newBGCol) noexcept
	{ bgCol_ = newBGCol; }

	void GuiButton::SetTextColor(ColorF const& newTextCol) noexcept
	{ textCol_ = newTextCol; }

	void GuiButton::SetLineColor(ColorF const& newLineCol) noexcept
	{ lineCol_ = newLineCol; }

	void GuiButton::Draw(Grafix& gfx, Camera const& cam)
	{ 
		if (!IsVisible())
		{
			return;
		}

		rect_.Fill(gfx, cam, bgCol_);
		rect_.Draw(gfx, cam, lineCol_, 2.f);

		if (false && !text_.empty())
		{
			auto center{rect_.GetCenter()};
			// offset = scalar * len
			// scalar = (offset / len) = (char offset)
			auto const scalar{Util::AsciiToRenderedTextRatio() * textSize_};
			center.x -= (text_.size() * 0.5f) * scalar;
			center.y -= scalar;
			gfx.DrawString(center, text_, textCol_, textSize_);
		}
	}

	void GuiButton::Update(Mouse const& mouse, Keyboard const& keyboard, float dt) noexcept
	{
		if (!IsEnabled())
		{
			return;
		}

		auto const bContainedLastFrame{rect_.Contains(mouseLocLastFrame_)};
		auto const bContainedCurrFrame{rect_.Contains(mouse.loc)};
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

		mouseLocLastFrame_ = mouse.loc;
	}

	bool GuiButton::IsEnabled() const noexcept
	{ return bActive_; }

	void GuiButton::Enable() noexcept
	{
		ARGUI_ASSERT(!bActive_, "Called Enable when window was already active");
		bActive_ = true;
	}

	void GuiButton::Disable() noexcept
	{
		ARGUI_ASSERT(bActive_, "Called Disable when window was already not active");
		bActive_ = false;
	}

	bool GuiButton::IsVisible() const noexcept
	{ return bVisible_; }

	void GuiButton::Show() noexcept
	{
		ARGUI_ASSERT(!bVisible_, "Called Show when window was already shown");
		bVisible_ = true;
	}

	void GuiButton::Hide() noexcept
	{
		ARGUI_ASSERT(bVisible_, "Called Hide when window was already hidden");
		bVisible_ = false;
	}

	bool GuiButton::IsMouseEntered() const noexcept
	{ return bMouseHovered_; }

	bool GuiButton::IsMouseHovered() const noexcept
	{ return bMouseHovered_; }

	bool GuiButton::IsMouseClicked() const noexcept
	{ return bMouseClicked_; }

	bool GuiButton::IsMouseHeldDown() const noexcept
	{ return bMouseHeld_; }

	bool GuiButton::IsMouseReleased() const noexcept
	{ return bMouseReleased_; }

	bool GuiButton::IsMouseLeft() const noexcept
	{ return bMouseLeft_; }

	Vec2 GuiButton::GetLoc() const noexcept
	{ return rect_.GetCenter(); }

	std::string_view GuiButton::GetText() const noexcept
	{ return text_; }

	float GuiButton::GetWidth() const noexcept
	{ return rect_.GetWidth(); }

	float GuiButton::GetHeight() const noexcept
	{ return rect_.GetHeight(); }

	ColorF const& GuiButton::GetBGColor() const noexcept
	{ return bgCol_; }

	ColorF const& GuiButton::GetTextColor() const noexcept
	{ return textCol_; }

	ColorF const& GuiButton::GetLineColor() const noexcept
	{ return lineCol_; }

	GuiStatus GuiButton::GetStatus() const noexcept
	{ return currStatus_; }

	GuiRectF const& GuiButton::GetRectF() const noexcept
	{ return rect_; }

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

