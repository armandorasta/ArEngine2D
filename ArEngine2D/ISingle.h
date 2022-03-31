#pragma once

namespace ArEngine2D::Details {
	class ISingle
	{
	public:
		ISingle()								 = default;
		ISingle(ISingle const&)					 = delete;
		ISingle(ISingle&&)						 = delete;
		decltype(auto) operator=(ISingle const&) = delete;
		decltype(auto) operator=(ISingle&&)		 = delete;
	};
}
