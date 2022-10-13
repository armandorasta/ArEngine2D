#include "ParticleSpring.h"

#include "Particle.h"

Phy::ParticleSpring::ParticleSpring(Particle const& otherParticle, float springConstant, float restLength) noexcept
	: other_{otherParticle}, springConstant_{springConstant}, restLength_{restLength}
{ 
	AR2D_ASSERT(springConstant != 0, "Spring constant can't be zero.");
}

void Phy::ParticleSpring::UpdateForce(Particle* particle, float)
{ 
	auto& par{*particle};
	auto const delVec{par.GetPos() - other_.GetPos()};
	auto const lenDel{std::abs(delVec.Mag() - restLength_)};
	par.AddForce(-springConstant_ * lenDel * delVec.Normalized());
}
