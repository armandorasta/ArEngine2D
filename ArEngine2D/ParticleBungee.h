#pragma once

#include "ParticleForceGenerator.h"
#include "PhyCore.h"

namespace Phy {
	class ParticleBungee : public ParticleForceGenerator
	{
	public:

		ParticleBungee(Particle const& other, float springConstant, float restLength) noexcept;

	public:

		void UpdateForce(Particle* particle, float = 0.f) override;

	private:
		Particle const& other_;
		float springConstant_;
		float restLength_;
	};
}