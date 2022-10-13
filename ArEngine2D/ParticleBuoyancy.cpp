#include "ParticleBuoyancy.h"
#include "Particle.h"

namespace Phy {
	ParticleBuoyancy::ParticleBuoyancy(float maxDepth, float objVolume, float liquidHeight, float liquidDensity)
		: maxDepth_{maxDepth}, volume_{objVolume}, liquidHeight_{liquidHeight}, liquidDensity_{liquidDensity}
	{ }

	void ParticleBuoyancy::UpdateForce(Particle* particle, float)
	{ 
		auto& par{*particle};
		auto const y{par.GetPos().y};

		// F = mg
		par.AddForce({0.f, 
			[&] {
				if (y > liquidHeight_ + maxDepth_)
					return 0.f;
				else if (y < liquidHeight_ - maxDepth_)
					return volume_ * liquidDensity_;
				else
					return volume_ * liquidDensity_ * ((y - maxDepth_ - liquidHeight_) / (2.f * maxDepth_));
			}()
		});
	}
}
