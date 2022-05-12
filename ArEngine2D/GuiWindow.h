#pragma once
#include "GuiCore.h"
#include "GuiButton.h"

ARGUI_BEGIN_NAMESPACE

// SetText
// SetColor
// SetBGColor
// SetSize
// SetRect
// SetColor
// BGEnabled
// DisableBG
// EnableBG
// Draw
//
class GuiText;


// for now, no resizeable windows.
class GuiWindow
{
public:

	GuiWindow(std::string_view name, Uint width, Uint height);

public:

	GuiText& AddText(Uint x, Uint y);
	GuiButton& AddButton(Uint x, Uint y);
	GuiSlider& AddSliderFloat(Uint x, Uint y);
};

ARGUI_END_NAMESPACE