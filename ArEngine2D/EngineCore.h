#pragma once

#include <cassert>
#include <chrono>
#include <d2d1.h>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "FixedWindowsHeader.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define ARENGINE2D_							::ArEngine2D::
#define AR2D_BEGIN_NAMESPACE				namespace ArEngine2D {
#define AR2D_END_NAMESPACE					}
#define AR2D_ASSERT(_condition, _message)	assert(_condition && _message)