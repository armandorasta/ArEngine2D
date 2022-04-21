#include "Engine.h"
#include "Car.h"

namespace User {
	using namespace ArEngine2D;
	class MyEngine : public ArEngine2D::Engine
	{
	public:

		using Engine::Engine;

	public:

		void OnUserCreate() override
		{
			car_.Initialize(L"Resources\\car.png");
		}

		void OnUserUpdate(float dt) override
		{
			camLoc_.x += 100.f * dt;
			UpdateCar(dt);
		}

		void OnUserDraw(Grafix& gfx) override
		{
			gfx.ClearScreen();
			// gfx.PushTransform(Transform{}.Translate(-camLoc_)); 
			{
				car_.Draw(gfx, 5.f);
				// gfx.PopTransform();
			}

			// gfx.DrawString({0.f, window.Height() - 35.f}, std::format("{}: {}", currName_, *currTarget_), Colors::GHOST_WHITE, 30.f);
		}

	private:
		void UpdateCar(float dt)
		{
			constexpr auto accForce{160'000'000.f};

			if (keyboard(Keys::W).IsDown())
				car_.Accelerate(accForce * dt);
			if (keyboard(Keys::S).IsDown())
				car_.Deccelerate(accForce * dt);
			if (keyboard(Keys::SPACE).IsDown())
				car_.Brake(0.5f * accForce * dt);
			if (keyboard(Keys::D).IsDown())
				car_.TurnRight(5.f * dt);
			if (keyboard(Keys::A).IsDown())
				car_.TurnLeft(5.f * dt);
			if (keyboard(Keys::ESCAPE).IsPressed())
				car_.loc_ = {window.Center()};
			if (keyboard(Keys::SHIFT).IsPressed())
				car_.currGear_ = std::min(car_.currGear_ + 1, 5);
			if (keyboard(Keys::CONTROL).IsPressed())
				car_.currGear_ = std::max(car_.currGear_ - 1, 0);

			if (true or keyboard(Keys::SPACE).IsPressed())
			{
				car_.Update(dt);
			}

			mouse.ForEachWheel([&](auto e) {
				if (e.IsUp())	*currTarget_ += currDelta_ * 100.f * dt;
				else			*currTarget_ -= currDelta_ * 100.f * dt;
			});

			struct MyType
			{
				float* Address;
				float Delta;
				char const* Name;
			};

			std::unordered_map<Keys, MyType> KeyActionLUT{
				{Keys::NUM1, {&car_.angVel_, 0.1f, "angular velocity"}},
				{Keys::NUM2, {&car_.vel_.y, 1.f, "lateral velocity"}},
				{Keys::NUM3, {&car_.vel_.x, 1.f, "front velocity"}},
				{Keys::NUM4, {&car_.turnAngle_, 0.1f, "turningAngle"}},
			};

			for (auto const& [key, newTarget] : KeyActionLUT)
			{
				if (keyboard(key).IsPressed())
				{
					currTarget_ = newTarget.Address;
					currDelta_  = newTarget.Delta;
					currName_ = newTarget.Name;
				}
			}
		}

	private:
		float* currTarget_{&car_.angVel_};
		float currDelta_{0.1f};
		char const* currName_{"angular velocity"};

		Vec2 camLoc_{20.f, 20.f};
		Car car_;
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