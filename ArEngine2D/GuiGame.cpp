#include "GuiGame.h"

#include "GuiButton.h"

namespace ArGui {
	void GuiGame::OnUserCreate()
	{
		mySlit.SetLoc({300.f, 300.f});
	}

	void GuiGame::OnUserUpdate(float dt)
	{
		cam_.UpdateDrag(mouse.right);
		cam_.UpdateZoomUsingScrollWheel();
		mySlit.Update(mouse, keyboard, dt);
	}

	void GuiGame::OnUserDraw(Grafix& gfx)
	{
		gfx.ClearScreen(Colors::DeepPink);
		mySlit.Draw(gfx);
	}
}
