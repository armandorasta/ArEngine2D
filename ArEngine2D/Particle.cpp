#include "Particle.h"

namespace Phy {
	Particle::Particle() : Particle{1.f, {}} { }
	Particle::Particle(float mass, Vec2 pos, Vec2 vel, Vec2 acc)
		: pos_{pos}, vel_{vel}, acc_{acc}
	{ 
		AR2D_ASSERT(mass > 0, "Invalid mass passed to Particle");
		SetMass(mass);
	}
	void Particle::Integrate(float dt) noexcept
	{ 
		if (inverseMass_ <= 0.f)
		{
			return;
		}

		AR2D_ASSERT(dt > 0.f, "Frame duration was 0");

		Vec2 resultingAcc{acc_ + forceAcc_ * inverseMass_};

		vel_ += resultingAcc * dt;
		vel_ *= std::pow(damping_, dt);

		pos_ += vel_ * dt;
		ClearForceAcc();
	}
	float Particle::GetKineticEnergy() const noexcept
	{
		return 0.5f * GetMass() * vel_.Mag2();
	}
	Vec2 Particle::GetForceAcc() const noexcept
	{ 
		return forceAcc_;
	}
	void Particle::AddForce(Vec2 newForce) noexcept
	{ 
		forceAcc_ += newForce;
	}
	void Particle::ClearForceAcc() noexcept
	{ 
		forceAcc_ = {};
	}
	void Particle::SetMass(float newMass) noexcept
	{ 
		AR2D_ASSERT(newMass != 0.f, "Tried to set mass to zero.");
		inverseMass_ = 1.f / newMass; 
	}
	void Particle::SetPos(Vec2 newPos) noexcept
	{ 
		pos_ = newPos; 
	}
	void Particle::SetVel(Vec2 newVel) noexcept
	{ 
		vel_ = newVel; 
	}
	void Particle::SetAcc(Vec2 newAcc) noexcept
	{ 
		acc_ = newAcc; 
	}
	void Particle::SetDamping(float newDamping) noexcept
	{ 
		damping_ = newDamping;
	}
	float Particle::GetMass() const noexcept
	{
		return 1.f / inverseMass_;
	}
	Vec2 Particle::GetPos() const noexcept
	{
		return pos_;
	}
	Vec2 Particle::GetVel() const noexcept
	{
		return vel_;
	}
	Vec2 Particle::GetAcc() const noexcept
	{
		return acc_;
	}
	float Particle::GetDamping() const noexcept
	{
		return damping_;
	}
}
