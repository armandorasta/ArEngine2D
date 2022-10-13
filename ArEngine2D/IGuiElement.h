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
	class IGuiElement
	{
	public:
		IGuiElement() = default;

	public:
		virtual void Draw(Grafix& gfx);
		virtual void Draw(Grafix& gfx, Camera const& cam) = 0;
		virtual void Update(Mouse const& mouse, Keyboard const& keyboard, float dt) noexcept = 0;

	public:
		bool Visible;
		bool Enabled;
	};
}