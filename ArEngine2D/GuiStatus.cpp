#include "GuiStatus.h"

ARGUI_BEGIN_NAMESPACE

namespace GuiStr {
	struct LutElement
	{
		GuiStatus status;
		std::string_view text;
	};

	static std::vector<LutElement> const sc_LUT{
		{GuiStatus::Fine, "No Error"},
		{GuiStatus::UnknownError, "Unknown Error"},
		{GuiStatus::EmptyText, "Text was of size 0"},
		{GuiStatus::TextDoesNotFit, "Text does not fit"},
		{GuiStatus::SizeTooBig, "Size was too small"},
		{GuiStatus::SizeTooBig, "Size was too big"},
		{GuiStatus::OutOfBounds, "Out of bounds"},
	};

	std::string ToString(GuiStatus status) noexcept
	{
		auto const it{std::ranges::find(sc_LUT, status, &LutElement::status)};
		ARGUI_ASSERT(it != sc_LUT.end(), "Tried to convert invalid GuiStatus to string");
		return std::string{it->text};
	}
}

std::string GuiStatusToString(GuiStatus status) noexcept
{ 
	return GuiStr::ToString(status); 
}

ARGUI_END_NAMESPACE

