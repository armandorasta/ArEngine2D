#pragma once

#include "GuiCore.h"

ARGUI_BEGIN_NAMESPACE

enum class GuiStatus : Uint
{
	Fine = 0,
	UnknownError,

	EmptyText,
	TextDoesNotFit,
	
	SizeTooSmall,
	SizeTooBig,
	OutOfBounds,
};

std::string GuiStatusToString(GuiStatus status) noexcept;

namespace Details {
	GuiStatus ConditionalStatus(bool condition, GuiStatus ifFalse) noexcept;
}

ARGUI_END_NAMESPACE