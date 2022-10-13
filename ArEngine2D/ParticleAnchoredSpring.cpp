#include "ParticleAnchoredSpring.h"

#include "Particle.h"

namespace Phy {
	ParticleAnchoredSpring::ParticleAnchoredSpring(Vec2 anchor, float springConstant, float restLength) noexcept
		: anchor_{anchor}, springConstant_{springConstant}, restLength_{restLength}
	{ }

	void ParticleAnchoredSpring::SetAnchor(Vec2 newAnchor) noexcept
	{
		anchor_ = newAnchor;
	}

	Vec2 ParticleAnchoredSpring::GetAnchor() const noexcept
	{
		return anchor_;
	}

	void ParticleAnchoredSpring::UpdateForce(Particle* particle, float)
	{
		auto& par{*particle};
		auto const delVec{par.GetPos() - anchor_};
		auto const lenDel{std::abs(delVec.Mag() - restLength_)};
		par.AddForce(-springConstant_ * lenDel * delVec.Normalized());
	}
}

