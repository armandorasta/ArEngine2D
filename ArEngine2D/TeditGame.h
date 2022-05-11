#pragma once
#include "TeditCore.h"
#include "Gui.h"
#include "GuiRectF.h"

ARTED_BEGIN_NAMESPACE

class TeditGame : public Engine
{
public:

	using Engine::Engine;

public:

	void OnUserCreate() override
	{

	}

	void OnUserUpdate(float dt) override
	{
		constexpr auto speed{500.f};

		Vec2 const move{
			(speed * dt) * (keyboard(Keys::D).IsDown() - keyboard(Keys::A).IsDown()),
			(speed * dt) * (keyboard(Keys::S).IsDown() - keyboard(Keys::W).IsDown())
		};

		auto const iterations{5U};
		for (std::size_t i{}; i < iterations; ++i)
		{
			rectm_.Move(move / iterations);
			rectm_.MoveOutOf(rect_);
		}
		
	}

	void OnUserDraw(Grafix& gfx) override
	{
		gfx.ClearScreen(Colors::DarkMagenta);
		rect_.DrawDebug(gfx, Colors::Magenta, 1.f);
		rectm_.DrawDebug(gfx, Colors::Yellow, 1.f);
		auto const v0{rectm_.GetBotRight() - rect_.GetTopLeft()};
		auto const v1{rect_.GetBotRight() - rectm_.GetTopLeft()};
		gfx.DrawArrow(rectm_.GetBotRight(), rect_.GetTopLeft(), Colors::Blue);
		gfx.DrawArrow(rect_.GetBotRight(), rectm_.GetTopLeft(), Colors::Blue);
		gfx.DrawString(rect_.GetTopLeft() + v0 * 0.5f, std::format("{}", v0), Colors::Blue, 20.f);
		gfx.DrawString(rectm_.GetTopLeft() + v1 * 0.5f, std::format("{}", v1), Colors::Blue, 20.f);
	}

private:
	ArGui::GuiRectF rect_{{100.f, 100.f}, 500.f, 300.f};
	ArGui::GuiRectF rectm_{{}, 300.f, 100.f};
	ArGui::GuiRectF rectm2_{{}, 300.f, 100.f};
};

ARTED_END_NAMESPACE