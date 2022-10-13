#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleAttractionForce : public ParticleForceGenerator
	{
	public:

		ParticleAttractionForce(Vec2 centerPoint, float attractionConstant) noexcept;

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		float constant_;
		Vec2 center_;
	};
}