#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleGravity : public ParticleForceGenerator
	{
	public:

		ParticleGravity(Vec2 gravityVec) noexcept;

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		Vec2 gravityVec_;
	};
}