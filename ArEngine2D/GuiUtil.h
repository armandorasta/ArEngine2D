#pragma once

#include "GuiCore.h"

ARGUI_BEGIN_NAMESPACE

namespace Con {
	template <class T>
	concept Number = std::integral<T> || std::floating_point<T>;
}

class Util
{
public:

};

ARGUI_END_NAMESPACE