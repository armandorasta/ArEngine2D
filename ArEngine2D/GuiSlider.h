#pragma once

#include "GuiUtil.h"
#include "GuiRectF.h"
#include "GuiStatus.h"

ARGUI_BEGIN_NAMESPACE

class GuiSlider
{
public:
	enum class CursorType
	{
		Rectangle,
		Ellipse,
	};

protected:
	constexpr static auto sc_CursorToLenRatio{1.f / 20.f};
	constexpr static auto sc_CursorToThickRatio{1.5f};
	constexpr static auto sc_MinSliderLength{30.f};
	constexpr static auto sc_MaxSliderLength{500.f};
	constexpr static auto sc_MinSliderThick{10.f};
	constexpr static auto sc_MaxSliderThick{30.f};
	constexpr static auto sc_MinCursorWidth{10.f};
	constexpr static auto sc_MaxCursorWidth{30.f};
	constexpr static auto sc_MinCursorHeight{20.f};
	constexpr static auto sc_MaxCursorHeight{30.f};

public:
	GuiSlider(Vec2 const& loc, float sliderLength, float minValue, float maxValue, ColorF bgBaseCol, ColorF bgLineCol, ColorF fgBaseCol,
		ColorF fgLineCol, ColorF textCol) noexcept;

public:
	void Update(Mouse const& mouse) noexcept;

	[[nodiscard]] bool IsMouseGrabbing() const noexcept;

	void SetCursorWidth(float newWidth) noexcept;
	void SetCursorHeight(float newHeight) noexcept;
	void SetCursorType(CursorType newType) noexcept;
	void SetLength(float newLength) noexcept;
	void SetThickness(float newThick) noexcept;
	void SetValue(float newValue) noexcept;
	void AddValue(float del) noexcept;

	[[nodiscard]] bool IsEnabled() const noexcept;
	void Enable() noexcept;
	void Disable() noexcept;
	
	void SetMinValue(float newMinValue) noexcept;
	void SetMaxValue(float newMaxValue) noexcept;
	void SetMinMaxValue(float newMin, float newMax) noexcept;
	
	void SetFGBaseColor(ColorF const& newColor) noexcept;
	void SetFGLineColor(ColorF const& newColor) noexcept;
	void SetBGBaseColor(ColorF const& newColor) noexcept;
	void SetBGLineColor(ColorF const& newColor) noexcept;
	void SetTextColor(ColorF const& newColor) noexcept;

public:
	void Draw(Grafix& gfx, Uint precision = 2U);

public:
	[[nodiscard]] float GetValue() const noexcept;
	[[nodiscard]] float GetCursorWidth() const noexcept;
	[[nodiscard]] float GetCursorHeight() const noexcept;
	[[nodiscard]] CursorType GetCursorType() const noexcept;
	[[nodiscard]] Vec2 GetCursorLoc() const noexcept;

	[[nodiscard]] float GetLength() const noexcept;
	[[nodiscard]] float GetThickness() const noexcept;

	[[nodiscard]] float GetMinValue() const noexcept;
	[[nodiscard]] float GetMaxValue() const noexcept;

	[[nodiscard]] ColorF GetFGBaseColor() const noexcept;
	[[nodiscard]] ColorF GetFGLineColor() const noexcept;
	[[nodiscard]] ColorF GetBGBaseColor() const noexcept;
	[[nodiscard]] ColorF GetBGLineColor() const noexcept;
	[[nodiscard]] ColorF GetTextColor() const noexcept;

protected:
	[[nodiscard]] static float ClampSize(float size) noexcept;
	[[nodiscard]] float ClampMinMax(float value) const noexcept;

private:
	// looks
	Vec2 loc_;
	// cursor
	float cursorWidth_;
	float cursorHeight_;
	CursorType cursorType_;
	// slider
	float sliderLen_;
	float sliderThick_;
	// limits
	float minValue_;
	float maxValue_;
	float curValue_;
	// colors
	ColorF bgBaseCol_;
	ColorF bgLineCol_;
	ColorF fgBaseCol_;
	ColorF fgLineCol_;
	ColorF textCol_;

	// logic
	bool bEnabled_{true};
	bool bMouseLanded_{};
	float mouseXCache_{};
	float valueCache_{};
};

ARGUI_END_NAMESPACE