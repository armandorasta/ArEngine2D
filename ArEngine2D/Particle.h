#pragma once

#include "PhyCore.h"

namespace Phy {
	class Particle
	{
	public:

		Particle();
		Particle(float mass, Vec2 pos, Vec2 vel = {}, Vec2 acc = {});

	public:

		void Integrate(float dt) noexcept;
		float GetKineticEnergy() const noexcept;

		Vec2 GetForceAcc() const noexcept;
		void AddForce(Vec2 newForce) noexcept;
		void ClearForceAcc() noexcept;

	public:

		void SetMass(float newMass) noexcept;
		void SetPos(Vec2 newPos) noexcept;
		void SetVel(Vec2 newVel) noexcept;
		void SetAcc(Vec2 newAcc) noexcept;
		void SetDamping(float newDamping) noexcept;
		
		float GetMass() const noexcept;
		Vec2 GetPos() const noexcept;
		Vec2 GetVel() const noexcept;
		Vec2 GetAcc() const noexcept;
		float GetDamping() const noexcept;

	protected:
		Vec2 pos_;
		Vec2 vel_;
		Vec2 acc_;
		float inverseMass_;

		float damping_{1.f};
		Vec2 forceAcc_;
	};
}