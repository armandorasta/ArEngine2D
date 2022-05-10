#pragma once
#include "GuiCore.h"
#include "GuiWindow.h"

ARGUI_BEGIN_NAMESPACE

class Gui
{
public:

	static void Initialize(Grafix& gfx);
	static void Deinitialize();
	static GuiWindow NewWindow();
};

ARGUI_END_NAMESPACE