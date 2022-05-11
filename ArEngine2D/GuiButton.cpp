#include "GuiButton.h"

ARGUI_BEGIN_NAMESPACE

GuiButton::GuiButton(Vec2 const& loc, std::string_view text, Uint width, Uint height, Uint textSize,
	ColorF const& textCol, ColorF const& bgCol, ColorF const& lineCol) noexcept :
	loc_{loc}, text_{text}, width_{width}, height_{height}, textSize_{textSize}, 
	textCol_{textCol}, bgCol_{bgCol}, lineCol_{lineCol}
{ 
	ARGUI_ASSERT(!text.empty(), "Button with empty label");
	ARGUI_ASSERT(textSize_ != 0, "Button Text has size 0");
	ARGUI_ASSERT(width_ != 0, "Button has width 0");
	ARGUI_ASSERT(height_ != 0 , "Button has height 0");
}

GuiStatus GuiButton::SetLocChecked(Vec2 const& newLoc, Vec2 const& minVec, Vec2 const& maxVec) noexcept
{
	if (auto const st{Details::ConditionalStatus(newLoc > minVec || newLoc < maxVec, GuiStatus::OutOfBounds)};
		st != GuiStatus::Fine)
	{
		return st;
	}
	else
	{
		SetLoc(newLoc);
		return GuiStatus::Fine;
	}
}

void GuiButton::SetLoc(Vec2 const& newLoc) noexcept
{ loc_ = newLoc; }

GuiStatus GuiButton::SetText(std::string_view newText)
{
	UpdateStatus(newText.empty(), GuiStatus::EmptyText);
	text_ = newText;
	return currStatus_;
}

GuiStatus GuiButton::SetWidth(Uint newWidth) noexcept
{
	UpdateStatus(newWidth == 0, GuiStatus::SizeTooSmall);
	width_ = newWidth;
	return currStatus_;
}

GuiStatus GuiButton::SetHeight(Uint newHeight) noexcept
{
	UpdateStatus(newHeight == 0, GuiStatus::SizeTooSmall);
	height_ = newHeight;
	return currStatus_;
}

void GuiButton::SetBGColor(ColorF const& newBGCol) noexcept
{ bgCol_ = newBGCol; }

void GuiButton::SetTextColor(ColorF const& newTextCol) noexcept
{ textCol_ = newTextCol; }

void GuiButton::SetLineColor(ColorF const& newLineCol) noexcept
{ lineCol_ = newLineCol; }

void GuiButton::Update(Mouse& mouse) noexcept
{ 
	
}

bool GuiButton::IsActive() const noexcept
{
	return false;
}

void GuiButton::Activate() noexcept
{ }

void GuiButton::Deactivate() noexcept
{ }

void GuiButton::IsVisible() const noexcept
{ }

void GuiButton::Show() noexcept
{ }

void GuiButton::Hide() noexcept
{ }

bool GuiButton::IsMouseEntered() const noexcept
{
	return false;
}

bool GuiButton::IsMouseClicked() const noexcept
{
	return false;
}

bool GuiButton::IsMouseLeft() const noexcept
{
	return false;
}

void GuiButton::UpdateStatus(bool condition, GuiStatus ifTrueOtherwiseFine) noexcept
{ 
	currStatus_ = !condition? ifTrueOtherwiseFine : GuiStatus::Fine;
}

void GuiButton::ResetStatus() noexcept
{ 
	currStatus_ = GuiStatus::Fine;
}

ARGUI_END_NAMESPACE


