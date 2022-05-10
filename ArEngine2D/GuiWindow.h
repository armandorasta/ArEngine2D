#pragma once
#include "GuiCore.h"

ARGUI_BEGIN_NAMESPACE

// SetText
// SetTextColor
// SetBGColor
// SetLineColor
// SetWidth
// SetHeight
// SetMouseHoverListener
// SetMouseClickListener
// SetMouseLeaveListener
// AddKeyboardShortcut
// SetKeybaordListener
// Draw
//
class GuiButton;

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

// Set Text
// SetBGColor
// SetTextColor
// SetLineColor
// SetListener
// SetMin
// SetMax
// Draw
//
class GuiSlider;

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