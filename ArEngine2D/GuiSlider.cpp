#include "GuiSlider.h"

#include "Camera.h"

namespace ArGui {

	GuiSlider::GuiSlider(Vec2 const& loc, float sliderLength, float minValue, float maxValue,
		ColorF bgBaseCol, ColorF bgLineCol, ColorF fgBaseCol, ColorF fgLineCol, ColorF textCol) noexcept :
		loc_{loc},
		bgBaseCol_{bgBaseCol}, bgLineCol_{bgLineCol},
		fgBaseCol_{fgBaseCol}, fgLineCol_{fgLineCol},
		textCol_{textCol}
	{
		ARGUI_ASSERT(minValue < maxValue, "Invalid limits");

		// cursor
		cursorWidth_ = std::clamp(sliderLength * sc_CursorToLenRatio, sc_MinCursorWidth, sc_MaxCursorWidth);
		cursorHeight_ = std::clamp(sliderLength * sc_CursorToLenRatio, sc_MinCursorHeight, sc_MaxCursorHeight);
		// slider
		sliderLen_ = std::clamp(sliderLength, sc_MinSliderLength, sc_MaxSliderLength);
		sliderThick_ = std::clamp(cursorWidth_ / sc_CursorToThickRatio, sc_MinSliderThick, sc_MaxSliderThick);
		// limits
		minValue_ = minValue;
		maxValue_ = maxValue;
		curValue_ = minValue;
	}

	void GuiSlider::Update(Mouse const& mouse, Keyboard const&, float) noexcept
	{
		if (!IsEnabled())
		{
			return;
		}

		if (bMouseLanded_)
		{
			if (mouse.left.IsDown())
			{
				SetValue(valueCache_ + ((mouse.loc.x - mouseXCache_) / sliderLen_) * (maxValue_ - minValue_));
			}
			else
			{
				bMouseLanded_ = false;
			}
		}
		else
		{
			auto const& mouseLoc{mouse.loc};
			Vec2 const cursorCenter{GetCursorLoc()};
			auto const cursorDel = Vec2{cursorWidth_, cursorHeight_} *0.5f;
			if (Util::IsPointInRect(mouseLoc, cursorCenter - cursorDel, cursorCenter + cursorDel) &&
				mouse.left.IsPressed())
			{
				mouseXCache_ = mouse.loc.x;
				valueCache_ = GetValue();
				bMouseLanded_ = true;
			}
		}
	}

	bool GuiSlider::IsMouseGrabbing() const noexcept
	{ return bMouseLanded_; }

	void GuiSlider::SetLoc(Vec2 const& newLoc) noexcept
	{ loc_ = newLoc; }

	void GuiSlider::SetCursorWidth(float newWidth) noexcept
	{
		cursorWidth_ = std::clamp(newWidth, sc_MinCursorWidth, sc_MaxCursorWidth);
	}

	void GuiSlider::SetCursorHeight(float newHeight) noexcept
	{
		cursorHeight_ = std::clamp(newHeight, sc_MinCursorHeight, sc_MaxCursorHeight);
	}

	void GuiSlider::SetCursorType(CursorType newType) noexcept
	{
		cursorType_ = newType;
	}

	void GuiSlider::SetThickness(float newThick) noexcept
	{
		sliderThick_ = std::clamp(newThick, sc_MinSliderThick, sc_MaxSliderThick);
	}

	void GuiSlider::SetValue(float newValue) noexcept
	{
		curValue_ = ClampMinMax(newValue);
	}

	void GuiSlider::AddValue(float del) noexcept
	{ SetValue(curValue_ + del); }

	bool GuiSlider::IsEnabled() const noexcept
	{ return bEnabled_; }

	void GuiSlider::Enable() noexcept
	{
		ARGUI_ASSERT(!bEnabled_, "Tried to enable GuiSlider already enabled");
		bEnabled_ = true;
	}

	void GuiSlider::Disable() noexcept
	{
		ARGUI_ASSERT(bEnabled_, "Tried to disable GuiSlider already disabled");
		bEnabled_ = false;
	}

	void GuiSlider::SetMinValue(float newMinValue) noexcept
	{
		ARGUI_ASSERT(newMinValue < maxValue_, "Invalid min value");
		minValue_ = newMinValue;
		SetValue(curValue_);
	}

	void GuiSlider::SetMaxValue(float newMaxValue) noexcept
	{
		ARGUI_ASSERT(newMaxValue > minValue_, "Invalid max value");
		maxValue_ = newMaxValue;
		curValue_ = ClampMinMax(curValue_);
	}

	void GuiSlider::SetMinMaxValue(float newMin, float newMax) noexcept
	{
		ARGUI_ASSERT(newMin < newMax, "Invalid limits");
		minValue_ = newMin;
		maxValue_ = newMax;
		curValue_ = ClampMinMax(curValue_);
	}

	void GuiSlider::SetLength(float newLength) noexcept
	{
		sliderLen_ = ClampSize(newLength);
	}

	void GuiSlider::SetFGBaseColor(ColorF const& newColor) noexcept
	{ fgBaseCol_ = newColor; }

	void GuiSlider::SetFGLineColor(ColorF const& newColor) noexcept
	{ fgLineCol_ = newColor; }

	void GuiSlider::SetBGBaseColor(ColorF const& newColor) noexcept
	{ bgBaseCol_ = newColor; }

	void GuiSlider::SetBGLineColor(ColorF const& newColor) noexcept
	{ bgLineCol_ = newColor; }

	void GuiSlider::SetTextColor(ColorF const& newColor) noexcept
	{ textCol_ = newColor; }

	void GuiSlider::Draw(Grafix& gfx)
	{
		// the slider in the back
		gfx.FillRectangleCenter(loc_, sliderLen_, sliderThick_, bgBaseCol_);
		gfx.DrawRectangleCenter(loc_, sliderLen_, sliderThick_, bgLineCol_, 2.f);

		// the cursor
		Vec2 const cursorLoc{GetCursorLoc()};
		switch (cursorType_)
		{
		case CursorType::Rectangle:
		{
			gfx.FillRectangleCenter(cursorLoc, cursorWidth_, cursorHeight_, fgBaseCol_);
			gfx.DrawRectangleCenter(cursorLoc, cursorWidth_, cursorHeight_, fgLineCol_, 2.f);
			break;
		}
		case CursorType::Ellipse:
		{
			auto const rx{cursorWidth_ * 0.5f};
			auto const ry{cursorHeight_ * 0.5f};
			gfx.FillEllipse(cursorLoc, rx, ry, fgBaseCol_);
			gfx.DrawEllipse(cursorLoc, rx, ry, fgLineCol_, 2.f);
			break;
		}
		default:
		{
			ARGUI_ASSERT(false, "Invalid cursor type");
			break;
		}
		}

		Vec2 const textOffset{
			cursorLoc.x - cursorWidth_ * 0.5f,
			cursorLoc.y - cursorHeight_ - Util::AsciiToRenderedTextRatio() * 20.f
		};
		gfx.DrawString(textOffset, std::format("{0:.{1}f}", curValue_, 2), textCol_, 20.f);
	}

	void GuiSlider::Draw(Grafix& gfx, Camera const& cam)
	{
		auto const camScale{cam.Scale()};
		// the slider in the back
		gfx.FillRectangleCenter(cam(loc_), sliderLen_ * camScale, sliderThick_ * camScale, bgBaseCol_);
		gfx.DrawRectangleCenter(cam(loc_), sliderLen_ * camScale, sliderThick_ * camScale, bgLineCol_, 2.f);

		// the cursor
		Vec2 const cursorLoc{GetCursorLoc()};
		switch (cursorType_)
		{
		case CursorType::Rectangle:
		{
			gfx.FillRectangleCenter(cam(cursorLoc), cursorWidth_ * camScale, cursorHeight_ * camScale, fgBaseCol_);
			gfx.DrawRectangleCenter(cam(cursorLoc), cursorWidth_ * camScale, cursorHeight_ * camScale, fgLineCol_, 2.f);
			break;
		}
		case CursorType::Ellipse:
		{
			auto const rx{cursorWidth_ * 0.5f * camScale};
			auto const ry{cursorHeight_ * 0.5f * camScale};
			gfx.FillEllipse(cam(cursorLoc), rx, ry, fgBaseCol_);
			gfx.DrawEllipse(cam(cursorLoc), rx, ry, fgLineCol_, 2.f);
			break;
		}
		default:
		{
			ARGUI_ASSERT(false, "Invalid cursor type");
			break;
		}
		}

		Vec2 const textOffset{
			cursorLoc.x - cursorWidth_ * 0.5f,
			cursorLoc.y - cursorHeight_ - Util::AsciiToRenderedTextRatio() * 20.f
		};
		gfx.DrawString(cam(textOffset), std::format("{0:.{1}f}", curValue_, 2), textCol_, 20.f * camScale);
	}

	Vec2 const& GuiSlider::GetLoc() const noexcept
	{ return loc_; }

	float GuiSlider::GetValue() const noexcept
	{ return curValue_; }

	float GuiSlider::GetCursorWidth() const noexcept
	{ return cursorWidth_; }

	float GuiSlider::GetCursorHeight() const noexcept
	{ return cursorHeight_; }

	GuiSlider::CursorType GuiSlider::GetCursorType() const noexcept
	{ return cursorType_; }

	float GuiSlider::GetThickness() const noexcept
	{ return sliderThick_; }

	float GuiSlider::GetMinValue() const noexcept
	{ return minValue_; }

	float GuiSlider::GetMaxValue() const noexcept
	{ return maxValue_; }

	float GuiSlider::GetLength() const noexcept
	{ return sliderLen_; }

	ColorF GuiSlider::GetFGBaseColor() const noexcept
	{ return fgBaseCol_; }

	ColorF GuiSlider::GetFGLineColor() const noexcept
	{ return fgLineCol_; }

	ColorF GuiSlider::GetBGBaseColor() const noexcept
	{ return bgBaseCol_; }

	ColorF GuiSlider::GetBGLineColor() const noexcept
	{ return bgLineCol_; }

	ColorF GuiSlider::GetTextColor() const noexcept
	{ return textCol_; }

	float GuiSlider::ClampSize(float size) noexcept
	{
		return std::clamp(size, sc_MinSliderLength, sc_MaxSliderLength);
	}

	float GuiSlider::ClampMinMax(float value) const noexcept
	{
		return std::clamp(value, minValue_, maxValue_);
	}

	Vec2 GuiSlider::GetCursorLoc() const noexcept
	{
		auto const t{(curValue_ - minValue_) / (maxValue_ - minValue_)};
		return {loc_.x + (t - 0.5f) * sliderLen_, loc_.y};
	}
}