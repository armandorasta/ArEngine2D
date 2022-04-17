#include "Engine.h"
#include <stdexcept>

namespace User {
	using namespace ArEngine2D;
	class MyEngine : public ArEngine2D::Engine
	{
	public:

		using ArEngine2D::Engine::Engine;

	public:

		void OnUserCreate() override
		{
		}

		void OnUserUpdate(float dt) override
		{
		}

		void OnUserDraw(Grafix& gfx) override
		{
		}

	private:
		/*user variables here*/
	};
}

INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	User::MyEngine window{"shit", 800, 600};
	window.Run();
	return 0;
}