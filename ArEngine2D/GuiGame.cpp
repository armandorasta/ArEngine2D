#include "GuiGame.h"

#include "GuiButton.h"

namespace ArGui {
	void GuiGame::OnUserCreate()
	{
		stack_.Push(std::make_unique<GuiWindow>("Window 1", 500.f, 300.0f));
		stack_.Push(std::make_unique<GuiWindow>("Window 2", 300.f, 500.f));
		stack_.Push(std::make_unique<GuiWindow>("Window 3", 300.f, 300.f));
	}

	void GuiGame::OnUserUpdate(float dt)
	{
		cam_.UpdateDrag(mouse.mid);
		cam_.UpdateZoomUsingScrollWheel();
		//guiWindow_.Update(dt, mouse, keyboard, cam_);
		stack_.Update(mouse, keyboard, cam_);
	}

	void GuiGame::OnUserDraw(Grafix& gfx)
	{
		gfx.ClearScreen(Colors::DeepPink);
		//guiWindow_.Draw(gfx, cam_);
		stack_.Draw(gfx, cam_);
	}
}
