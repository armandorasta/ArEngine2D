#include "ParticleDrag.h"

#include "Particle.h"

namespace Phy {
	ParticleDrag::ParticleDrag(float k1, float k2) noexcept
		: k1_{k1}, k2_{k2}
	{ }
	
	void ParticleDrag::UpdateForce(Particle* particle, float)
	{ 
		auto& par{*particle};
		auto const vel{par.GetVel()};
		auto const velNorm{vel.Normalized()};
		auto const velMag{vel.Mag()};
		auto const velDel{-velNorm * (k1_ * velMag + k2_ * velMag * velMag)};
		par.SetVel(vel + velDel);
	}
}
