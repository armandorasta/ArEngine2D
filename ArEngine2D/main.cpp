#include "GuiGame.h"

#include <fstream>
#include <filesystem>
#include <execution>
#include <iostream>

#include "Random.h"

namespace User {
	namespace eng = ArEngine2D;

	class Game : public eng::Engine
	{
	public:

		using Engine::Engine;

	public:

		void OnUserCreate() override
		{
			std::cout << "hello?\n";
		}

		void OnUserUpdate(float dt) override
		{
			
		}

		void OnUserDraw(eng::Grafix& gfx) override
		{
			gfx.ClearScreen(eng::Colors::HotPink);
		}

	private:
		

	private:


	};
}

//INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
int main()
{
	User::Game engine{"my eng", 1280, 720};
	engine.Run();
	return 0;
}