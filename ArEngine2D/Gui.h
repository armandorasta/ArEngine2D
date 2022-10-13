#pragma once
#include "GuiCore.h"
#include "GuiWindow.h"

namespace ArGui {
	class Gui
	{
	public:

		static void Initialize(Grafix& gfx);
		static void Deinitialize();
		static GuiWindow NewWindow();
	};
}
