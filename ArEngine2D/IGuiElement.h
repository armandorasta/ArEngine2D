#pragma once

#include "GuiUtil.h"

/*
	- GuiWindow
	- GuiButton
	- GuiTextBuffer
	- GuiSlider
	- GuiCeckBox
	- GuiMenu
	- GuiDropDownMenu
*/

namespace ArEngine2D {
	class Camera;
}

namespace ArGui {
	enum class GuiElementType
	{
		Button,
		Slider,
		Window,
	};

	class IGuiElement
	{
	public:
		IGuiElement() = default;

	public:
		[[nodiscard]] virtual GuiElementType GetType() const noexcept = 0;
		[[nodiscard]] virtual Vec2 GetLoc() const noexcept = 0;
		[[nodiscard]] virtual GuiRectF GetRectF(Camera const& cam) const noexcept = 0;

		virtual void SetLoc(Vec2 newLoc) noexcept = 0;
	public:
		bool Visible{true};
		bool Enabled{true};
	};
}