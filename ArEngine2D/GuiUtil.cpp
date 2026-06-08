#include "GuiUtil.h"

#include "GuiRectF.h"

namespace ArGui {
	bool Util::IsPointInRect(Vec2 const& p, GuiRectF const& rect)
	{
		auto const rtl{rect.GetTopLeft()};
		auto const rbr{rect.GetBotRight()};
		return (p.x > rtl.x && p.x < rbr.x) && (p.y > rtl.y && p.y < rbr.y);
	}
	void Util::HandleDrag(GuiRectF const& dragRegion, Mouse const& mouse,
		bool* bMouseLanded, Vec2* mouseLandLoc, Vec2* otherLandLoc, Vec2* otherLoc)
	{ 
		return HandleDrag(dragRegion.GetTopLeft(), dragRegion.GetBotRight(),
			mouse, bMouseLanded, mouseLandLoc, otherLandLoc, otherLoc);
	}
}

