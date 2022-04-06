#include "Engine.h"
#include <stdexcept>

namespace User {
	namespace engine = ArEngine2D;
	class MyEngine : public ArEngine2D::Engine
	{
	private:
		using self = MyEngine;
		using base = Engine;

	public:

		using base::base;

	public:

		void OnUserCreate() override
		{
		}

		void OnUserUpdate(float dt) override
		{
		}

		void OnUserDraw(engine::Grafix& gfx) override
		{
			gfx.ClearScreen(engine::Colors::SANDY_BROWN);
		}
	};
}

int main() 
{
	try
	{
		User::MyEngine window{"shit", 800, 600};
		window.Run();
	}
	catch (const std::exception&)
	{

	}

	return 0;
}