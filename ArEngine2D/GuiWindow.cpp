#include "GuiWindow.h"

namespace ArGui {
	GuiWindow::GuiWindow(std::string_view name, float width, float height, Vec2 loc)
		: title_{name}, loc_{loc},
		width_{std::max(sc_MinWidth, width)},
		height_{std::max(height, sc_MinHeight)},
		butClose_{{}, "X", sc_CloseButtSize, sc_CloseButtSize},
		butMinimize_{{}, "-", sc_CloseButtSize, sc_CloseButtSize}
	{
		butClose_.SetLoc({
			loc_.x + width_ - sc_CloseButtSize * .5f - 5.f,
			loc_.y + sc_CloseButtSize * 0.5f + sc_TitleBarHeight * .1f
		});
		butMinimize_.SetLoc({
			loc_.x + width_ - sc_CloseButtSize * 1.5f - 10.f,
			loc_.y + sc_CloseButtSize * 0.5f + sc_TitleBarHeight * .1f
		});
		componentOffsets_.push_back({&butClose_, butClose_.GetLoc() - loc_});
		componentOffsets_.push_back({&butMinimize_, butMinimize_.GetLoc() - loc_});
	}
	void GuiWindow::Update(Mouse const& mouse, Keyboard const& keyboard, Camera const& cam)
	{
		if (!Enabled)
		{
			return;
		}

		butClose_.Update(mouse, cam);
		butMinimize_.Update(mouse, cam);

		if (butClose_.IsMouseClicked())
		{
			Visible = Enabled = false;
		}

		bMinimized_ ^= butMinimize_.IsMouseClicked();

		if (mouse.left.IsPressed() &&
			Util::IsPointInRect(cam[mouse.loc], loc_, {loc_.x + width_, loc_.y + sc_TitleBarHeight}))
		{
			mouseLandLoc_ = mouse.loc;
			windowLandLoc_ = loc_;
			bMouseLanded_ = true;
		}

		if (bMouseLanded_)
		{
			bMouseLanded_ = mouse.left.IsDown();
			loc_ = windowLandLoc_ + (mouse.loc - mouseLandLoc_) / cam.Scale();
			UpdateComponentLocs();
		}
	}
	void GuiWindow::Draw(Grafix& gfx, Camera const& cam) noexcept
	{
		auto const camLoc{cam(loc_)};
		auto const camScale{cam.Scale()};
		auto const camWidth{width_ * camScale};
		auto const camHeight{height_ * camScale};
		auto const camTitleBarHeight{sc_TitleBarHeight * camScale};

		if (!Visible)
		{
			return;
		}

		if (bMinimized_)
		{
			gfx.FillRectangle(camLoc, camWidth, camTitleBarHeight, Colors::DarkRed);
			gfx.DrawRectangle(camLoc, camWidth, camTitleBarHeight, Colors::Black);
		}
		else
		{
			gfx.FillRectangle(camLoc, camWidth, camHeight, Colors::Red);
			gfx.FillRectangle(camLoc, camWidth, camTitleBarHeight, Colors::DarkRed);
			gfx.DrawRectangle(camLoc, camWidth, camTitleBarHeight, Colors::Black);
			gfx.DrawRectangle(camLoc, camWidth, camHeight, Colors::Black);
		}

		// Normal or Minimized
		butClose_.Draw(gfx, cam, Colors::Red, Colors::Black, Colors::White);
		butMinimize_.Draw(gfx, cam, Colors::DarkCyan, Colors::Black, Colors::White);
	}
	void GuiWindow::SetLoc(Vec2 newLoc) noexcept
	{ loc_ = newLoc; }
	GuiElementType GuiWindow::GetType() const noexcept
	{ return GuiElementType::Window; }
	Vec2 GuiWindow::GetLoc() const noexcept
	{ return loc_; }
	GuiRectF GuiWindow::GetRectF(Camera const& cam) const noexcept
	{
		if (!Visible)
		{
			// 1 x 1 rectangle at infinity, so no collision is possible
			return {{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}, 1.f, 1.f};
		}
		else if (bMinimized_)
		{
			return {cam(loc_), width_ * cam.Scale(), sc_TitleBarHeight * cam.Scale()};
		}
		else
		{
			return {cam(loc_), width_ * cam.Scale(), height_ * cam.Scale()};
		}
	}
	void GuiWindow::UpdateComponentLocs()
	{
		for (auto& [comp, offset] : componentOffsets_)
		{
			comp->SetLoc(loc_ + offset);
		}
	}
}