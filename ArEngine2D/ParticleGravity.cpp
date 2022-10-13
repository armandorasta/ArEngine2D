#include "ParticleGravity.h"

#include "Particle.h"

namespace Phy {
	ParticleGravity::ParticleGravity(Vec2 gravityVec) noexcept
		: gravityVec_{gravityVec} 
	{ }
	
	void ParticleGravity::UpdateForce(Particle* particle, float)
	{ 
		auto& par{*particle};
		par.AddForce(gravityVec_ * par.GetMass());
	}
}
