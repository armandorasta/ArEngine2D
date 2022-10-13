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
		widthSlider_.SetLoc({300.f, 50.f});
		heightSlider_.SetLoc({300.f, 100.f});
		sizeSlider_.SetLoc({300.f, 150.f});
		widthSlider_.SetValue(width_);
		heightSlider_.SetValue(height_);
		sizeSlider_.SetValue(size_);
	}

	void OnUserUpdate(float dt) override
	{
		Vec2 const del{
			1.f * (keyboard(Keys::D).IsDown() - keyboard(Keys::A).IsDown()),
			1.f * (keyboard(Keys::S).IsDown() - keyboard(Keys::W).IsDown())
		};

		UpdateUI(dt);
	}

	void OnUserDraw(Grafix& gfx) override 
	{
		gfx.ClearScreen(Colors::DarkMagenta);

		auto const center{window.Center()};
		// gfx.DrawStringRect("This is your boy's long string of text", Colors::LimeGreen, size_,
		// 	{center.x, center.y, center.x + width_, center.y + height_});
		
		// gfx.DrawRectangle(window.Center(), width_, height_, Colors::Yellow);

		// DrawUI(gfx);

		auto const myString{"hello"};
		auto const fontSize{30.f};
		gfx.DrawString(mouse.loc, myString, Colors::Lime, fontSize);
		gfx.DrawRectangle(mouse.loc, 30.f * sizeof myString, fontSize * 1.25f, Colors::Magenta, 2.f);

		// tits_.Draw(gfx, 6U);
		// gfx.DrawString(mouse.loc, std::format("{}", mouse.loc), Colors::LimeGreen, 20.f);
	}

private:
	void UpdateUI(float dt)
	{
		widthSlider_.Update(mouse);
		heightSlider_.Update(mouse);
		sizeSlider_.Update(mouse);

		width_ = widthSlider_.GetValue();
		height_ = heightSlider_.GetValue();
		size_ = sizeSlider_.GetValue();
	}

	void DrawUI(Grafix& gfx)
	{
		gfx.DrawString({10.f, 20.f}, "width: ", Colors::White, 30.f);
		widthSlider_.Draw(gfx);
		gfx.DrawString({10.f, 70.f}, "height: ", Colors::White, 30.f);
		heightSlider_.Draw(gfx);
		gfx.DrawString({10.f, 120.f}, "size: ", Colors::White, 30.f);
		sizeSlider_.Draw(gfx);
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
	gui::GuiSlider widthSlider_{{}, 300.f, 0.f, 700.f,
		Colors::DarkBlue, Colors::Blue,
		Colors::DarkGreen, Colors::Green,
		Colors::White};
	gui::GuiSlider heightSlider_{{}, 300.f, 0.f, 700.f,
		Colors::DarkBlue, Colors::Blue,
		Colors::DarkGreen, Colors::Green,
		Colors::White};
	gui::GuiSlider sizeSlider_{{}, 300.f, 1.f, 100.f,
		Colors::DarkBlue, Colors::Blue,
		Colors::DarkGreen, Colors::Green,
		Colors::White};

	float width_{200.f};
	float height_{150.f};
	float size_{30.f};
};

ARTED_END_NAMESPACE