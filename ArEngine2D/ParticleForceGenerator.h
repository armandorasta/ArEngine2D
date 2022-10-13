#pragma once

namespace Phy {
	class Particle;

	class ParticleForceGenerator
	{
	public:

		virtual void UpdateForce(Particle* particle, float dt) = 0;

	};
}