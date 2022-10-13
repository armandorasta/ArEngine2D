#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleDrag : public ParticleForceGenerator
	{
	public:

		ParticleDrag(float k1, float k2) noexcept;

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		float k1_;
		float k2_;
	};
}