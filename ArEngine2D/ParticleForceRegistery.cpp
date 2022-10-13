#include "ParticleForceRegistery.h"

#include <algorithm>
#include <ranges>

namespace Phy {
	void ParticleForceRegistery::Add(Particle* particle, ParticleForceGenerator* forceGen)
	{ 
		registery_.push_back({particle, forceGen});
	}

	void ParticleForceRegistery::Remove(Particle* particle, ParticleForceGenerator* forceGen)
	{ 
		ParticleForceRegisteration const reg{particle, forceGen};
		if (auto const it{std::ranges::find(registery_, reg)};
			it != registery_.end())
		{
			registery_.erase(it);
		}
	}

	void ParticleForceRegistery::Clear() noexcept
	{ 
		registery_.clear();
	}

	void ParticleForceRegistery::UpdateForces(float dt)
	{ 
		for (auto& [par, gen] : registery_)
		{
			gen->UpdateForce(par, dt);
		}
	}
}
