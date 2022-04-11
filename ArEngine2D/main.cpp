#include "Engine.h"
#include <stdexcept>

namespace User {
	using namespace ArEngine2D;
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
			sprite.Initialize(L"Resources\\Ball.png", 32.f, 32.f, 0, 0.1f);
		}

		void OnUserUpdate(float dt) override
		{
			mouse.ForEachWheel([&](Mouse::WheelEvent e) {
				if (e.IsUp())
					sprite.SetAnimationSpeed(2.f);
				else			
					sprite.SetAnimationSpeed(.5f);
			});

			if (keyboard(Keys::SPACE).IsPressed())
			{
				sprite.SetCurrFrame(3U);
				sprite.ToggleAnimation();
			}

			sprite.Update(dt);
		}

		void OnUserDraw(Grafix& gfx) override
		{
			gfx.ClearScreen();
			gfx.PushTransform(Transform{}.Scale(5.f).Translate(mouse.loc));
			gfx.DrawAnimationSpriteSheet({}, sprite);
			gfx.ResetTransform();
		}

	private:
		float fCounter{};
		float angle{};
		AnimationSpriteSheet sprite{};
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