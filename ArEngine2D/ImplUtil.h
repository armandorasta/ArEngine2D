#pragma once

#include "FixedWindowsHeader.h"
#include "ISingle.h"
#include <wrl/client.h>
#include <d3d11.h>
#include <concepts>

namespace ArEngine2D::Details {
	template <class T>
	using Ptr = Microsoft::WRL::ComPtr<T>;

	template <class... T>
	using Unusable = std::nullptr_t;

	template <class T>
	concept Not = std::same_as<T, std::false_type>;

	class ImplUtil
	{
	public:

		ImplUtil() = delete;
	};
}

