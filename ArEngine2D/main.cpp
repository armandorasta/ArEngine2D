#include "Engine.h"
#include <stdexcept>

namespace User {
	namespace engine = ArEngine2D;
	class MyEngine : public ArEngine2D::Engine
	{
	private:
		using self = MyEngine;
		using base = Engine;

	public:

		using base::base;

	public:

		void OnUserCreate() override
		{
			sprite.Initialize(L"MyPNG.png");
		}

		void OnUserUpdate(float dt) override
		{
			mouse.ForEachWheel([&](engine::Mouse::WheelEvent e) {
				if (e.IsDown())
				{
					angle += 10 * dt;
				}
				else
				{
					angle -= 10 * dt;
				}
			});
		}

		void OnUserDraw(engine::Grafix& gfx) override
		{
			gfx.ClearScreen();
			gfx.PushTransform(engine::Transform{}
				.Rotate(angle)
				.Scale(.5f)
				.Translate(mouse.loc)
				.Translate({100.f, 0.f}));

			gfx.DrawRectangleCenter({}, 500.f, 400.f, engine::Colors::MEDIUM_SEA_GREEN);
			gfx.ResetTransform();
		}

	private:
		float angle{};
		engine::Sprite sprite{};
	};
}

INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	try
	{
		User::MyEngine window{"shit", 800, 600};
		window.Run();
	}
	catch (const std::exception&)
	{

	}

	return 0;
}