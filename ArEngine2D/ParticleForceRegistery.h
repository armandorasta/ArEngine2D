#pragma once

#include <vector>
#include "ParticleForceGenerator.h"

namespace Phy {
	class Particle;

	class ParticleForceRegistery
	{
	public:

		struct ParticleForceRegisteration
		{
			Particle* Part;
			ParticleForceGenerator* ForceGen;

			bool operator==(ParticleForceRegisteration const& rhs) const noexcept = default;
		};

		using Registery = std::vector<ParticleForceRegisteration>;

	public:

		ParticleForceRegistery() = default;

	public:

		void Add(Particle* particle, ParticleForceGenerator* forceGen);
		void Remove(Particle* particle, ParticleForceGenerator* forceGen);
		void Clear() noexcept;
		void UpdateForces(float dt);

	private:
		Registery registery_;
	};
}