#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleSpring : public ParticleForceGenerator
	{
	public:

		ParticleSpring(Particle const& otherParticle, float springConstant, float restLength) noexcept;

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		Particle const& other_;
		float springConstant_;
		float restLength_;
	};
}