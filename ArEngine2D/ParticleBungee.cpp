#include "ParticleBungee.h"
#include "Particle.h"

namespace Phy {
	Phy::ParticleBungee::ParticleBungee(Particle const& other, float springConstant, float restLength) noexcept
		: other_{other}, springConstant_{springConstant}, restLength_{restLength}
	{ }
	void ParticleBungee::UpdateForce(Particle* particle, float)
	{
		auto& par{*particle};
		auto const delVec{par.GetPos() - other_.GetPos()};
		if (auto const lenDel{delVec.Mag() - restLength_};
			lenDel > 0.f)
		{
			par.AddForce(-springConstant_ * lenDel * delVec.Normalized());
		}
	}
}

