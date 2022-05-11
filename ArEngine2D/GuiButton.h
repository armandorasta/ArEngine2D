#pragma once

#include "GuiCore.h"
#include "GuiSize.h"
#include "GuiStatus.h"

ARGUI_BEGIN_NAMESPACE

class GuiButton
{
public:
	GuiButton(Vec2 const& loc, std::string_view text, Uint width, Uint height, Uint textSize, 
		ColorF const& textCol, ColorF const& bgCol, ColorF const& lineCol = Colors::Black) noexcept;

public:
	GuiStatus SetLocChecked(Vec2 const& newLoc, Vec2 const& minVec, Vec2 const& maxVec) noexcept;
	void SetLoc(Vec2 const& newLoc) noexcept;
	GuiStatus SetText(std::string_view newText);
	GuiStatus SetWidth(Uint newWidth) noexcept;
	GuiStatus SetHeight(Uint newHeight) noexcept;
	void SetBGColor(ColorF const& newBGCol) noexcept;
	void SetTextColor(ColorF const& newTextCol) noexcept;
	void SetLineColor(ColorF const& newLineCol) noexcept;

public:
	void Update(Mouse& mouse) noexcept;
	
	bool IsActive() const noexcept;
	void Activate() noexcept;
	void Deactivate() noexcept;
	
	void IsVisible() const noexcept;
	void Show() noexcept;
	void Hide() noexcept;
	
	bool IsMouseEntered() const noexcept;
	bool IsMouseClicked() const noexcept;
	bool IsMouseLeft() const noexcept;

public:
	Vec2 const& GetLoc() const noexcept;
	std::string_view GetText() const noexcept;
	Uint GetWidth() const noexcept;
	Uint GetHeight() const noexcept;
	ColorF const& GetBGColor() const noexcept;
	ColorF const& GetTextColor() const noexcept;
	ColorF const& GetLineColor() const noexcept;
	GuiStatus GetStatus() const noexcept;

private:
	void UpdateStatus(bool condition, GuiStatus ifTrueOtherwiseFine) noexcept;
	void ResetStatus() noexcept;

private:
	// looks
	Vec2 loc_;
	ColorF bgCol_;
	ColorF textCol_;
	ColorF lineCol_;
	Uint width_;
	Uint height_;
	std::string text_;
	Uint textSize_;

	// logic
	GuiStatus currStatus_{GuiStatus::Fine};
	bool bActive_;
	bool bVisible_;
	bool bMouseClicked_;
	bool bMouseHovered_;
	bool bMouseLeft_;
};

ARGUI_END_NAMESPACE