#include "Engine.h"
#include <stdexcept>


namespace User {
	using namespace ArEngine2D;
	class MyEngine : public ArEngine2D::Engine
	{
	private:

		class Car
		{
		private:
			constexpr static float sc_BrakeConstant{1000.f};
			constexpr static float sc_MaxWheelTurn{Util::DegreeToRadian(30.f)};
			constexpr static float sc_MaxSpeed{1.f};
			constexpr static float sc_DragConstant{5.f};

		public:

			Car() = default;

		public:

			/*
				F_traction = C_engine * U			// unit vector
				F_drag = -C_drag * v * mag(v)
				F_roll = -C_roll * v
				F_long = F_traction + F_drag + F_roll
				a = F / M
				v = v0 + a * dt;
				p = p0 + v * dt;
				F_brake = -C_brake * U				// unit vector

				// realism
				F_drag = 0.5 * C_d * A * rho * v^2 
				{
					C_d = coefficient of friction (0.3 for a corvette)
					A = frontal area (2.2 m^2)
					rho = density of air (1.29 kg/m^3)
				}

				F_max_tyre_friction = mu * W 
				{
					mu = 1 for street and 1.5 for racing
					W = M * g
				}

				when the car is stationary
				{
					W_front = (c / L) * W
					W_back  = (b / L) * W 
					{
						c = distance from the back wheel to the center of gravity
						b = distance from the front wheel to the center of gravity
					}
				}

				when the car is moving 
				{
					W_front = (c / L) * W - (h / L) * M * a
					W_back  = (b / L) * W + (h / L) * M * a
					{
						h = the height of the center of gravity.
						a = the acceleration of the car (negative incase of deceleration)
					}

					simplified
					{
						W_front = 0.5 * W - (h / L) * M * a
						W_back  = 0.5 * W + (h / L) * M * a
					}
				}
			*/

			void Initialize(std::wstring_view fileName) 
			{
				image_.Initialize(fileName);
			}

			void MoveForward(float amount) noexcept 
			{
			}
			void MoveBackward(float amount) noexcept 
			{
			}
			void MoveTo(Vec2 const& whereTo) noexcept 
			{
				loc_ = whereTo;
			}

			void TurnRight(float delAngle) noexcept 
			{
				wheelDir_ = std::min(wheelDir_ + delAngle, sc_MaxWheelTurn);
			}
			void TurnLeft(float delAngle) noexcept 
			{
				wheelDir_ = std::max(wheelDir_ - delAngle, -sc_MaxWheelTurn);
			}
			void SetDir(float newDir) noexcept 
			{
				dir_ = newDir;
			}

			void Brake(float dt) noexcept 
			{
				if (auto const sqMag{vel_.Mag2()}; sqMag > 0.f)
				{
					if (sqMag > sc_BrakeConstant)
					{
						vel_ -= vel_.Normalized() * (sc_BrakeConstant * dt);
					}
					else { vel_ = {}; }
				}
			}

			void ApplyForce(Vec2 force, float dt) noexcept 
			{
				vel_ += (force / mass_) * dt;
			}

			void Update(float dt) noexcept 
			{
				if (Util::Abs(wheelDir_) > dt)
				{
					wheelDir_ += (wheelDir_ > 0) ? -dt : dt;
				}
			}

			void Draw(Grafix& gfx, float scale = 1.f) noexcept 
			{
				gfx.SetInterpolationMode(InterpolationMode::NEAREST_NEIGHBOR);
				gfx.PushTransform(Transform{}.Rotate(dir_).Scale(scale).Translate(loc_));
				gfx.DrawSpriteCenter({}, image_);
				gfx.ResetTransform();
			}

		public:

			float Dir() const noexcept 
			{ return dir_; }
			float WheelDir() const noexcept 
			{ return wheelDir_; }
			Vec2 Loc() const noexcept 
			{ return loc_; }
			Vec2 Vel() const noexcept 
			{ return vel_; }
			Transform CurrTransform() const noexcept {
				return Transform{}.Rotate(dir_).Translate(loc_);
			}
			Sprite const& Image() const noexcept 
			{ return image_; }

			bool IsMoving() const noexcept 
			{ return vel_.Mag2() > 0.f; }
			bool IsMovingForward() const noexcept {
				return Vec2::FromDir(dir_) * vel_ > 0.f;
			}
			bool IsMovingBackward() const noexcept {
				return Vec2::FromDir(dir_) * vel_ < 0.f;
			}

		public:
			float wheelDir_; // wheel direction
			float mass_{1.f}; 
			float dir_; // the direction the car is facing

			Vec2 loc_{400.f, 300.f};
			Vec2 vel_;
			Vec2 acc_;

			Sprite image_;
		};

	public:

		using Engine::Engine;

	public:

		void OnUserCreate() override
		{
			car_.Initialize(L"Resources\\car.png");
		}

		void OnUserUpdate(float dt) override
		{
			if (keyboard(Keys::W).IsDown())		 car_.MoveForward(2500.f * dt);
			else if (keyboard(Keys::S).IsDown()) car_.MoveBackward(2500.f * dt);

			if (keyboard(Keys::D).IsDown())		 car_.TurnRight(10.f * dt);
			else if (keyboard(Keys::A).IsDown()) car_.TurnLeft(10.f * dt);

			if (keyboard(Keys::SPACE).IsDown())
			{
				car_.Brake(dt);
			}

			if (mouse.mid.IsPressed())
			{
				car_.MoveTo(window.Center());
			}

			mouse.ForEachWheel([&](auto e) {
				if (e.IsDown()) thick_ += 15.f * dt;
				else			thick_ -= 15.f * dt;
			});

			car_.Update(dt);
		}

		void OnUserDraw(Grafix& gfx) override
		{
			auto CarScale{thick_};

			gfx.ClearScreen();
			gfx.DrawString({}, std::format("speed: {:.2}", car_.Vel()), Colors::RED, 30.f);
			car_.Draw(gfx, CarScale);

			gfx.PushTransform(Transform{}
				.Translate({car_.Image().Width() * 0.5f * CarScale, 0.f})
				.Rotate(car_.WheelDir() + car_.Dir())
				.Translate(car_.Loc())
			);
			gfx.DrawArrow({}, {car_.Image().Width() * 0.25f * CarScale, 0.f}, Colors::RED, 2.f);
			gfx.ResetTransform();
		}

	private:
		float thick_{5.f};

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