#pragma once

#include "Engine.h"

namespace ArTran {
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

		void Initialize(std::wstring_view spriteFileName);
		void Accelerate(float force);
		void Decelerate(float force);
		void ShiftToNextGear();
		void ShiftToPrevGear();
		void TurnLeft(float force);
		void TurnRight(float force);
		void Brake(float force);
		void Draw(Grafix& gfx, float scale = 1.f) const;
		void Update(float dt);

	private:

		static float GetGearRatio(std::int32_t id);
		float GetEngineTorque(float rate);

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