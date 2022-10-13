#include "PhyGame.h"
#include "Random.h"
#include "ScopedTimer.h"
#include "Random.h"

namespace Phy {
	void PhyGame::OnUserCreate()
	{ 
		for (std::size_t i{}; i < sc_ParticleCount; ++i)
		{
			parts_.emplace_back(std::make_unique<Particle>(
				5.f, // Random::RandomFloat(0.f, 5.f), 
				Random::RandomVec2(
					-Vec2{400.f, 400.f}, 
					Vec2{400.f, 400.f}
				)
			))->SetDamping(0.9f);
			gens_.emplace_back(std::make_unique<ParticleSpring>(*parts_.back(), sc_SpringConstant, sc_RestLength));
		}

		for (std::size_t i{}; i < parts_.size(); ++i)
		{
			reg_.Add(parts_[i].get(), gens_[(i + 1) % parts_.size()].get());
			reg_.Add(parts_[i].get(), gens_[(i + parts_.size() - 1) % parts_.size()].get());
		}

		gens_.emplace_back(std::make_unique<ParticleGravity>(sc_Gravity));
		for (std::size_t i{}; i < parts_.size(); ++i)
		{
			reg_.Add(parts_[i].get(), gens_.back().get());
		}
	}
	
	void PhyGame::OnUserUpdate(float dt)
	{ 
		cam_.UpdateDrag(mouse.right);
		cam_.UpdateZoomUsingScrollWheel();
		reg_.UpdateForces(dt);
		for (auto& part : parts_)
		{
			forceAccs_.emplace_back(part->GetForceAcc());
			part->Integrate(dt);
		}
		parts_.back()->SetPos(cam_[mouse.loc]);
	}

	void PhyGame::OnUserDraw(Grafix& gfx)
	{
		gfx.ClearScreen(Colors::DarkBlue);

		for (std::size_t i{}; i < parts_.size(); ++i)
		{
			auto const& p{*parts_[i]};
			auto const loc{p.GetPos()};
			auto const mass{p.GetMass()};

			gfx.DrawArrow(cam_(parts_[i]->GetPos()), cam_(parts_[(i + 1) % parts_.size()]->GetPos()), Colors::Yellow, 3.f);
			// gfx.DrawLine(cam_(loc), cam_(parts_.back()->GetPos()), Colors::Yellow);
			// gfx.DrawArrow(cam_(loc), cam_(loc + [&] {
			// 	auto const myVec{parts_.back()->GetPos() - loc};
			// 	auto const mag{myVec.Mag()};
			// 	return myVec * std::min(mag, sc_RestLength / mag);
			// }()), Colors::Chocolate);
			gfx.FillCircle(cam_(loc), cam_.Scale() * mass * 10.f, Colors::Red);
			gfx.DrawCircle(cam_(loc), cam_.Scale() * mass * 10.f, Colors::Black, 3.f);
			// gfx.DrawArrow(cam_(loc), cam_(loc + p.GetVel()), Colors::Green, 3.f);
			// gfx.DrawArrow(cam_(loc), cam_(loc + sc_Gravity), Colors::Pink, 3.f);
			// gfx.DrawArrow(cam_(loc), cam_(loc + forceAccs_[i]), Colors::Maroon, 3.f);
		}
		forceAccs_.clear();

		/*constexpr std::size_t CellsPerRow{8U};

		auto const camLoc{cam_.Loc()};
		auto const cellHeight{window.Height() * 1.f / CellsPerRow};
		auto const cellWidth{cellHeight};
		float const x{std::fmod(camLoc.x, cellWidth)};
		float const y{std::fmod(camLoc.y, cellHeight)};
		Vec2 const offset{x, y};

		for (float i{-cellWidth}; i < window.Width() + cellWidth; i += cellWidth)
		{
			for (float j{-cellHeight}; j < window.Height() + cellHeight; j += cellHeight)
			{
				gfx.DrawRectangle({i - offset.x, j - offset.y}, cellWidth, cellHeight, Colors::Black);
			}
		}

		gfx.DrawCircle(cam_(camLoc), 5.f * cam_.Scale(), Colors::Black);
		gfx.DrawCircle(cam_({}), 5.f * cam_.Scale(), Colors::Black);
		gfx.DrawLine(cam_(camLoc), cam_(myOtherParticle.GetPos()), Colors::LightPink);
		gfx.FillCircle(cam_(myOtherParticle.GetPos()), 10.f * cam_.Scale(), Colors::LightPink);
		gfx.DrawString({}, std::format("{}", myOtherParticle.GetPos()), Colors::HotPink, 30.f);*/
	}

	void PhyGame::Ex_3_5(float dt) noexcept
	{
		static float obj0{}, obj1{};
		static constexpr float Damping{0.9999999f};
		static constexpr float Gravity{10.f};

		using namespace std::chrono_literals;
		static constexpr auto updateDelta{1s};

		obj0 = obj0 * Damping + Gravity * dt;
		obj1 = obj1 * std::pow(Damping, dt) + Gravity * dt;

		static std::chrono::duration<float> s_Acc{};
		s_Acc += 1s * dt;
		if (s_Acc > 1s)
		{
			s_Acc -= 1s;
			std::cout << std::format("obj0: {}, obj1: {}\n", obj0, obj1);
		}
	}
}
