#include "ParticleAttractionForce.h"

#include "Particle.h"

namespace Phy {
	ParticleAttractionForce::ParticleAttractionForce(Vec2 centerPoint, float attractionConstant) noexcept
		: constant_{attractionConstant}, center_{centerPoint}
	{ }

	void ParticleAttractionForce::UpdateForce(Particle* particle, float)
	{ 
		auto& par{*particle};
		auto const radVec{center_ - par.GetPos()};

		if (auto const mag2{radVec.Mag2()};
			mag2 >= 0.01f)
		{
			// F = G * m1 / r^2
			auto const forceScalar{constant_ * par.GetMass() / mag2};
			par.AddForce(forceScalar * radVec.Normalized());
		}
	}
}
