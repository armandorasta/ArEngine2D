#pragma once

#include "GuiUtil.h"
#include "GuiRectF.h"
#include "GuiStatus.h"
#include "IGuiElement.h"

namespace ArGui {
	class GuiSlider : public IGuiElement
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
		GuiSlider(Vec2 const& loc, float sliderLength, float minValue, float maxValue) noexcept;

	public:
		void Update(Mouse const& mouse) noexcept;

		[[nodiscard]] bool IsMouseGrabbing() const noexcept;

		void SetLoc(Vec2 const& newLoc) noexcept;
		void SetCursorWidth(float newWidth) noexcept;
		void SetCursorHeight(float newHeight) noexcept;
		void SetCursorType(CursorType newType) noexcept;
		void SetLength(float newLength) noexcept;
		void SetThickness(float newThick) noexcept;
		void SetValue(float newValue) noexcept;
		void AddValue(float del) noexcept;

		void SetMinValue(float newMinValue) noexcept;
		void SetMaxValue(float newMaxValue) noexcept;
		void SetMinMaxValue(float newMin, float newMax) noexcept;

	public:
		void Draw(Grafix& gfx, Camera const& cam,
			ColorF const& lineColor, ColorF const& bgColor,
			ColorF const& cursorColor, ColorF const& textColor) noexcept;

	public:
		[[nodiscard]] GuiElementType GetType() const noexcept override;
		[[nodiscard]] Vec2 GetLoc() const noexcept override;
		[[nodiscard]] GuiRectF GetRectF(Camera const& cam) const noexcept override;
		[[nodiscard]] float GetValue() const noexcept;
		[[nodiscard]] float GetCursorWidth() const noexcept;
		[[nodiscard]] float GetCursorHeight() const noexcept;
		[[nodiscard]] CursorType GetCursorType() const noexcept;
		[[nodiscard]] Vec2 GetCursorLoc() const noexcept;

		[[nodiscard]] float GetLength() const noexcept;
		[[nodiscard]] float GetThickness() const noexcept;

		[[nodiscard]] float GetMinValue() const noexcept;
		[[nodiscard]] float GetMaxValue() const noexcept;

	private:
		// looks
		std::string text_;
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
		// logic
		bool bMouseLanded_{};
		float mouseXCache_{};
		float valueCache_{};
	};
}