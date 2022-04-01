#include "Engine.h"

///
#include <numbers>
///

namespace ArEngine2D {
	Engine::Engine(std::string_view title, std::int32_t windowWidth, std::int32_t windowHeight) :
		window_{title, windowWidth, windowHeight}
	{
	}
    void Engine::Run() noexcept
    {
		try
		{
			Initialize();
			OnUserCreate();
			while (window_.ProcessMessages())
			{
				OnUserUpdate(GetFrameDelta());
				window_.InputUpdate();
				gfx_.BeginDraw();
				gfx_.ClearScreen({.8f, .5f, 0.f});

				// gfx.DrawTriangle(mouse.loc, {-100.f, -100.f}, {-100.f, 100.f}, {100.f, 0.f}, Colors::DARK_GREEN, 5.f);
				gfx.DrawLine(mouse.loc, window_.Center(), Colors::YELLOW, 5.f);

				gfx_.EndDraw();
			}
		}
		catch (IEngineError const& err)
		{
			MessageBoxA(window_.Handle(), err.Message().data(), err.MessageBoxTitle().data(), MB_ICONERROR);
		}
    }
	void Engine::Initialize()
	{
		IEngineError::InitializeInfoQueue();
		gfx_.Initialize(window_.Handle());
	}
	float Engine::GetFrameDelta()
	{
		return 0.f;
	}
}
