#pragma once

#include "GuiCore.h"
#include "GuiButton.h"
#include "Camera.h"
#include "GuiSlider.h"

namespace ArGui {
	class GuiGame : public ArEngine2D::Engine
	{
	public:

		using Engine::Engine;

	public:

		void OnUserCreate() override;
		void OnUserUpdate(float dt) override;
		void OnUserDraw(Grafix& gfx) override;

	private:
		DraggableCamera cam_{mouse};

		std::vector<IGuiElement*> guiElements{};

		GuiButton myButt{window.Center(), "Your butt", 300.f, 200.f, Colors::Yellow, Colors::DarkGoldenrod, Colors::Yellow};
		GuiSlider mySlit{{100.f, 100.f}, 500.f, -200.f, 400.f, 
			Colors::DarkRed, Colors::Red, 
			Colors::DarkGreen, Colors::Green, Colors::White};
		Vec2 mouseLocLastFrame{};
	};
}
