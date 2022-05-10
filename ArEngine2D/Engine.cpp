#include "Engine.h"

#include <chrono>

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
				auto const dt{GetFrameDelta()};

				// update keyboard and mouse input
				window_.InputUpdate();

				// update the user's code
				OnUserUpdate(dt);

				// Render to the screen
				gfx_.BeginDraw();
				OnUserDraw(gfx_);
				gfx_.EndDraw();

				// show the fps and other stuff
				UpdateTitle(dt);
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
		static std::chrono::time_point s_Last{std::chrono::steady_clock::now()};
		static std::chrono::time_point s_Current{std::chrono::steady_clock::now()};
		
		s_Current = std::chrono::steady_clock::now();
		std::chrono::duration<float> const deltaTime{s_Current - s_Last};
		s_Last = s_Current;
		return deltaTime.count();
	}
	void Engine::UpdateTitle(float dt)
	{
		static std::chrono::time_point s_Last{std::chrono::steady_clock::now()};
		static std::chrono::time_point s_Current{std::chrono::steady_clock::now()};

		s_Current = std::chrono::steady_clock::now();
		std::chrono::duration<float> const deltaTime{s_Current - s_Last};

		using namespace std::chrono_literals;
		if (deltaTime > 0.5s) // update every 0.1f
		{
			static auto const sc_InitialTitle{window_.Title()};
			window_.SetTitle(std::format(
				"[{}] FPS: {}, Frame Delta: {}",
				sc_InitialTitle,
				dt > 0.001f ? std::to_string(1.f / dt) : "+1000",
				dt
			));
			window_.UpdateTitle();
			window_.SetTitle(sc_InitialTitle);
			s_Last = s_Current;
		}
	}
}
