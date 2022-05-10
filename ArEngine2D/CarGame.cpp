#include "CarGame.h"

namespace ArTran {
	void CarGame::OnUserCreate()
	{
		car_.Initialize(L"Resources\\car.png");
	}

	void CarGame::OnUserUpdate(float dt)
	{
		mouse.ForEachWheel([&](auto e) {
			if (e.IsUp())	arrowAngle_ += 5.f * dt;
			else			arrowAngle_ -= 5.f * dt;
		});
	}

	void CarGame::OnUserDraw(Grafix& gfx)
	{
		gfx.ClearScreen(Colors::DarkOrange);
		auto const topTran{Transform{}.Rotate(arrowAngle_).Translate(mouse.loc)};
		gfx.PushTransform(topTran);
		{
			Vec2 const headLoc{200.f, 0.f};
			gfx.DrawArrow({}, headLoc, Colors::MediumPurple, 5.f);

			gfx.PushTransform(Transform{}
				.Translate(-mouse.loc)
				.Translate(headLoc)
				.Rotate(arrowAngle_)
				.Translate(mouse.loc)
			);
			{
				gfx.DrawArrow({}, headLoc * 0.5f, Colors::Yellow, 3.f);
				gfx.PopTransform();
			}
			gfx.PopTransform();
		}

		//car_.Draw(gfx, 5.f);
	}

	void CarGame::UpdateInput(float dt)
	{
		constexpr auto accForce{160'000'000.f};

		auto const UpdateKeyDown = [&](Keys key, std::invocable auto&& callable) {
			if (keyboard(key).IsDown()) 
			{
				callable();
			}
		};

		auto const UpdateKeyPressed = [&](Keys key, std::invocable auto&& callable) {
			if (keyboard(key).IsPressed())
			{
				callable();
			}
		};

		UpdateKeyDown(Keys::W, [&] { car_.Accelerate(accForce * dt); });
		UpdateKeyDown(Keys::S, [&] { car_.Decelerate(accForce * dt); });
		UpdateKeyDown(Keys::D, [&] { car_.TurnRight(accForce * 1e-6f * dt); });
		UpdateKeyDown(Keys::A, [&] { car_.TurnLeft(accForce * 1e-6f * dt); });
		UpdateKeyDown(Keys::Space, [&] { car_.Brake(accForce * 0.25f * dt); });
		UpdateKeyPressed(Keys::Escape, [&] { car_.loc_ = {}; });
	}

}