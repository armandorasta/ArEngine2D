#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	public:

		ParticleAnchoredSpring(Vec2 anchor, float springConstant, float restLength) noexcept;

	public:

		void SetAnchor(Vec2 newAnchor) noexcept;
		Vec2 GetAnchor() const noexcept;

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		Vec2 anchor_;
		float springConstant_;
		float restLength_;
	};
}