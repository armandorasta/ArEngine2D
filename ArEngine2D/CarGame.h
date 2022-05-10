#pragma once

#include "Engine.h"
#include "Car.h"

namespace ArTran {
	using namespace ArEngine2D;
	class CarGame : public ArEngine2D::Engine
	{
	public:

		using ArEngine2D::Engine::Engine;

	public:

		void OnUserCreate() override;
		void OnUserUpdate(float dt) override;
		void OnUserDraw(Grafix& gfx) override;

	private:

		void UpdateInput(float dt);

	private:
		Car car_{};

		float arrowAngle_{};
	};
}