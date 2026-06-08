#pragma once

#include "GuiCore.h"
#include "GuiButton.h"
#include "Camera.h"
#include "GuiSlider.h"
#include "GuiWindow.h"
#include "GuiManager.h"

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
		GuiWindow guiWindow_{"My Window", 500.f, 300.f};

		GuiWindowStack stack_{};
	};
}
