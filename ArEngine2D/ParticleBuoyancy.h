#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleBuoyancy : public ParticleForceGenerator
	{
	public:

		ParticleBuoyancy(float maxDepth, float objVolume, float liquidHeight, float liquidDensity = 1000.f);

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		float maxDepth_;
		float volume_;
		float liquidHeight_;
		float liquidDensity_;
	};
}