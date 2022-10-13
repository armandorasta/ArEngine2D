#pragma once

#include "PhyCore.h"
#include "Particle.h"
#include "Camera.h"
#include "Timer.h"
#include "ParticleAnchoredSpring.h"
#include "ParticleSpring.h"
#include "ParticleAttractionForce.h"
#include "ParticleBungee.h"
#include "ParticleGravity.h"
#include "ParticleForceRegistery.h"

namespace Phy {
	class PhyGame : public Engine
	{
	private:

		constexpr static std::size_t sc_ParticleCount{10};
		constexpr static auto sc_SpringConstant{50.f};
		constexpr static auto sc_RestLength{100.f};
		constexpr static Vec2 sc_Gravity{0.f, 500.f};

	public:

		using ArEngine2D::Engine::Engine;

	public:

		void OnUserCreate() override;
		void OnUserUpdate(float dt) override;
		void OnUserDraw(Grafix& gfx) override;

	private:
		void Ex_3_5(float dt) noexcept;
		
	private:
		DraggableCamera cam_{mouse};
		Timer timer_{};
		std::vector<std::unique_ptr<ParticleForceGenerator>> gens_{};
		std::vector<std::unique_ptr<Particle>> parts_{};
		ParticleForceRegistery reg_{};
		std::vector<Vec2> forceAccs_{};
	};
}