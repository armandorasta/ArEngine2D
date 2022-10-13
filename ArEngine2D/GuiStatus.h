#pragma once

#include "GuiCore.h"

namespace ArGui {
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
}