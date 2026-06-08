#include "GuiSlider.h"

#include "Camera.h"

namespace ArGui {

	GuiSlider::GuiSlider(Vec2 const& loc, float sliderLength, float minValue, float maxValue) noexcept 
		: loc_{loc}
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

	void GuiSlider::Update(Mouse const& mouse) noexcept
	{
		if (!Enabled)
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
		curValue_ = std::clamp(curValue_, minValue_, maxValue_);
	}

	void GuiSlider::AddValue(float del) noexcept
	{ SetValue(curValue_ + del); }

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
		SetValue(curValue_);
	}

	void GuiSlider::SetMinMaxValue(float newMin, float newMax) noexcept
	{
		ARGUI_ASSERT(newMin < newMax, "Invalid limits");
		minValue_ = newMin;
		maxValue_ = newMax;
		SetValue(curValue_);
	}

	void GuiSlider::SetLength(float newLength) noexcept
	{
		sliderLen_ = std::clamp(newLength, sc_MinSliderLength, sc_MaxSliderLength);
	}

	void GuiSlider::Draw(Grafix& gfx, Camera const& cam,
		ColorF const& lineColor, ColorF const& bgColor,
		ColorF const& cursorColor, ColorF const& textColor) noexcept
	{
		auto const camScale{cam.Scale()};
		// the slider in the back
		gfx.FillRectangleCenter(cam(loc_), sliderLen_ * camScale, sliderThick_ * camScale, bgColor);
		gfx.DrawRectangleCenter(cam(loc_), sliderLen_ * camScale, sliderThick_ * camScale, lineColor, 2.f);

		// the cursor
		Vec2 const cursorLoc{GetCursorLoc()};
		switch (cursorType_)
		{
		case CursorType::Rectangle:
		{
			gfx.FillRectangleCenter(cam(cursorLoc), cursorWidth_ * camScale, cursorHeight_ * camScale, cursorColor);
			gfx.DrawRectangleCenter(cam(cursorLoc), cursorWidth_ * camScale, cursorHeight_ * camScale, lineColor, 2.f);
			break;
		}
		case CursorType::Ellipse:
		{
			auto const rx{cursorWidth_ * 0.5f * camScale};
			auto const ry{cursorHeight_ * 0.5f * camScale};
			gfx.FillEllipse(cam(cursorLoc), rx, ry, cursorColor);
			gfx.DrawEllipse(cam(cursorLoc), rx, ry, lineColor, 2.f);
			break;
		}
		default:
		{
			ARGUI_ASSERT(false, "Invalid cursor type");
			break;
		}
		}

		// TODO: Handle drawing text here.
	}

	GuiElementType GuiSlider::GetType() const noexcept
	{ return GuiElementType::Slider; }

	Vec2 GuiSlider::GetLoc() const noexcept
	{ return loc_; }

	GuiRectF GuiSlider::GetRectF(Camera const& cam) const noexcept
	{ return {cam(loc_), sliderLen_ * cam.Scale(), sliderThick_ * cam.Scale()}; }

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

	Vec2 GuiSlider::GetCursorLoc() const noexcept
	{
		auto const t{(curValue_ - minValue_) / (maxValue_ - minValue_)};
		return {loc_.x + (t - 0.5f) * sliderLen_, loc_.y};
	}
}