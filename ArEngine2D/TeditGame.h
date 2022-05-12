#pragma once
#include "TeditCore.h"
#include "Gui.h"
#include "GuiRectF.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include <thread>

ARTED_BEGIN_NAMESPACE
namespace gui = ArGui;

class TeditGame : public Engine
{
private:

	struct ContactAndNormal
	{
		Vec2 contact;
		Vec2 normal;
		float t;
	};

	struct MyRect
	{
		ArGui::GuiRectF rect;
		Vec2 vel;
	};

public:

	using Engine::Engine;

public:

	void OnUserCreate() override
	{
		 //tits_.SetCursorWidth(0.f);
		//tits_.SetCursorHeight(10000.f);
		//tits_.SetThickness(0.f);
		tits_.SetCursorType(gui::GuiSlider::CursorType::Ellipse);
	}

	void OnUserUpdate(float dt) override
	{
		Vec2 const del{
			1.f * (keyboard(Keys::D).IsDown() - keyboard(Keys::A).IsDown()),
			1.f * (keyboard(Keys::S).IsDown() - keyboard(Keys::W).IsDown())
		};

		tits_.Update(mouse);

		if (keyboard(Keys::Tab).IsPressed())
		{
			if (tits_.IsEnabled())
			{
				tits_.Disable();
			}
			else
			{
				tits_.Enable();
			}
		}
	}

	void OnUserDraw(Grafix& gfx) override
	{
		gfx.ClearScreen(Colors::DarkMagenta);
		tits_.Draw(gfx, 6U);
		gfx.DrawString(mouse.loc, std::format("{}", mouse.loc), Colors::LimeGreen, 20.f);
	}

private:
	std::optional<ContactAndNormal> RectVsRay(Vec2 const& p, Vec2 const& dir, ArGui::GuiRectF const& rect)
	{
		auto const& topLeft{rect.GetTopLeft()};
		auto const& botRight{rect.GetBotRight()};
		Vec2 tNear{(topLeft.x - p.x) / dir.x, (topLeft.y - p.y) / dir.y,};
		Vec2 tFar{(botRight.x - p.x) / dir.x, (botRight.y - p.y) / dir.y};
		if (std::isnan(tNear.x) || std::isnan(tNear.y) ||
			std::isnan(tFar.x) || std::isnan(tFar.y))
		{
			return {};
		}
		// sort them so, they are direction neutral
		if (tNear.x > tFar.x)
		{
			std::swap(tNear.x, tFar.x);
		}
		if (tNear.y > tFar.y)
		{
			std::swap(tNear.y, tFar.y);
		}
		// check for collision
		if (tNear.x > tFar.y || tNear.y > tFar.x)
		{
			return {};
		}
		// get where the ray hits the rectangle
		auto const tHitNear{std::max(tNear.x, tNear.y)};
		auto const tHitFar{std::min(tFar.x, tFar.y)};
		if (tHitFar < 0.f)
		{
			return {};
		}

		ContactAndNormal res{};
		res.contact = p + tHitNear * dir;
		res.t = tHitNear;
		if (tNear.x > tNear.y)
		{
			res.normal.x = 1.f - 2.f * (dir.x > 0);
		}
		else
		{
			res.normal.y = 1.f - 2.f * (dir.y > 0);
		}
		return {res};
	}
	std::optional<ContactAndNormal> DynRectVsRect(ArGui::GuiRectF const& moving, Vec2 const& vel, float dt, ArGui::GuiRectF stat)
	{
		auto const hw{moving.GetWidth() * 0.5f};
		auto const hh{moving.GetHeight() * 0.5f};
		stat.SetLeft(stat.GetLeft() - hw);
		stat.SetRight(stat.GetRight() + hw);
		stat.SetTop(stat.GetTop() - hh);
		stat.SetBot(stat.GetBot() + hh);
		auto const res{RectVsRay(moving.GetCenter(), vel * dt, stat)};
		if (!res || res->t > 1.f)
		{
			return {};
		}
		else
		{
			return res;
		}
	}

private:
	gui::GuiSlider tits_{window.Center(), 100000.f, -360.f, 360.f,
		Colors::DarkBlue, Colors::Blue,
		Colors::DarkGreen, Colors::Green,
		Colors::White};
};

ARTED_END_NAMESPACE