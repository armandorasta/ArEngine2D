#pragma once

#include "Window.h"
#include "Grafix.h"

namespace ArEngine2D {
	class Engine : Details::ISingle
	{ 
	public:

		Engine(std::string_view title, std::int32_t windowWidth, std::int32_t windowHeight);

	public:

		/**
		 * @brief starts and runs the game engine until the game window is closed.
		 */
		void Run() noexcept;

		/**
		 * @brief called once after the game window is created.
		 */
		virtual void OnUserCreate() = 0;

		/**
		 * @brief called ever frame.
		 */
		virtual void OnUserUpdate(float dt) = 0;

	private:
		void Initialize();
		float GetFrameDelta();

	public:

		Grafix& gfx{gfx_};
		Keyboard const& keyboard{window_.keyboard};
		Mouse const& mouse{window_.mouse};

	private:
		Window window_;
		Grafix gfx_;
	};
}

