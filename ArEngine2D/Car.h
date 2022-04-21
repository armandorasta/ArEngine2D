#pragma once

#include "Engine.h"

namespace User {
	using namespace ArEngine2D;
	class Car
	{
	public:
		constexpr static float sc_DragConstant{0.3f};
		constexpr static float sc_RollingResistanceConstant{0.3f};
		constexpr static float sc_fGravity{9.8f};
		constexpr static float sc_MaxTurnAngle{Util::DegreeToRadian(30.f)};

	public:

		Car() = default;

	public:

		void Initialize(std::wstring_view spriteFileName)
		{
			image_.Initialize(spriteFileName);
		}

		void Accelerate(float force)
		{
			// direction depends on the gear
			fTraction_ += (currGear_ > 0) ? force : -force;
			bAcceleratingThisFrame_ = true;
		}

		void Deccelerate(float force)
		{
			auto const some{currGear_};
			currGear_ = -1;
			Accelerate(force);
			currGear_ = some;
		}

		void ShiftToNextGear()
		{

		}

		void ShiftToPrevGear()
		{

		}

		void TurnLeft(float force)
		{
			turnAngle_ -= force;
			turnAngle_ = std::clamp(turnAngle_, -sc_MaxTurnAngle, sc_MaxTurnAngle);
		}

		void TurnRight(float force)
		{
			TurnLeft(-force);
		}

		void Brake(float force)
		{
			fBrake_ -= force;
		}

		void Draw(Grafix& gfx, float scale = 1.f) const
		{
			gfx.SetInterpolationMode(InterpolationMode::NEAREST_NEIGHBOR);
			gfx.DrawSpriteCenter({}, image_, 1.f, Transform{}
				.Rotate(dir_).Scale(scale).Translate(loc_));
			gfx.DrawArrow(loc_, loc_ + Vec2::FromDir(dir_ + turnAngle_) * 5.f * breadth_ * scale, Colors::DARK_KHAKI);

			gfx.DrawString({}, std::format("velocity: {}", vel_), Colors::WHITE, 30.f);
			gfx.DrawString({0.f, 30.f}, std::format("gear: {}", currGear_), Colors::WHITE, 30.f);
		}

		void Update(float dt)
		{
			auto const dirVec{Vec2::FromDir(dir_)};
			Vec2 const fDrag{-sc_DragConstant * vel_ * vel_.Mag()};
			Vec2 const fRollRes{-sc_RollingResistanceConstant * mass_ * sc_fGravity * vel_};
			Vec2 const fRes{fDrag + fRollRes};
			Vec2 const fBrakeVec{fBrake_ * vel_.Normalized()};
			Vec2 const fTractionVec{fTraction_ * dirVec / GetGearRatio(currGear_)};
			Vec2 const fCornering{400.f * turnAngle_ * dirVec.Rotated(Util::HalfPi)};
			Vec2 const fNet{fTractionVec + fCornering + fBrakeVec + dirVec + fRes};

			acc_ = fNet / mass_;
			vel_ += acc_ * dt;
			if (vel_ * fBrakeVec > 0.f)
			{
				vel_ = {};
			}

			loc_ += vel_ * dt;

			//angAcc_ = 0.3f * (turnAngle_ / sc_MaxTurnAngle) / (vel_ * dirVec);
			//angVel_ += angAcc_ * dt;
			dir_ += 0.1f * dt * (turnAngle_ / Util::TwoPi) * (vel_ * dirVec);
			fTraction_ = {};
			fBrake_ = {};

			turnAngle_ -= Util::Sign(turnAngle_) * std::min(dt, std::abs(turnAngle_));
		}


	private:

		static float GetGearRatio(std::int32_t id)
		{
			using LookUpTable = const std::unordered_map<std::int32_t, float>;
			static LookUpTable s_LUT{
				{1, 3.0f}, 
				{2, 2.0f}, 
				{3, 1.5f}, 
				{4, 1.0f}, 
				{5, 0.5f}
			};

			if (s_LUT.contains(id))
			{
				return s_LUT.at(id);
			}
			else
			{
				return 3.f; // R or invalid
			}
		}

		float GetEngineTorque(float rate)
		{
			rate = std::clamp(rate, 6.f, 5500.f);
			if (rate < 1000.f) // constant
			{
				return 220.f;
			}
			else if (rate < 4200.f) // going up
			{
				constexpr auto slope{(320.f - 220.f) / (4200.f - 1000.f)};
				return 220.f + slope * (rate - 1000.f);
			}
			else // going down with the same slope
			{
				constexpr auto slope{-(320.f - 220.f) / (4200.f - 1000.f)};
				return 320.f + slope * (rate - 4200.f);
			}
		}

	public:
		/// for debug
		Vec2 slipAngles_{};
		Vec2 fCornering_{};
		///

		// the width of the car
		float width_{2.5f};
		// the breadth of the car
		float breadth_{4.f};
		float corneringStiffness_{4.f};
		float mass_{1000.f};

		// 1 through 5 and R
		std::int32_t currGear_{1};

		Vec2 loc_{400.f, 300.f};
		Vec2 vel_{};
		Vec2 acc_;

		float angAcc_{};
		float angVel_{};
		float dir_{};

		bool bAcceleratingThisFrame_{};

		float turnAngle_{Util::DegreeToRadian(0.f)};
		float fTraction_{};
		// will be converted to a vector in the oppsite direction of the velocity
		float fBrake_{};
		// shown on the speedometer, not net velocity
		float speed_{};

		Vec2 size_{2.f, 4.f};
		float wheelRadius_{0.3f};

		Sprite image_;
	};
}