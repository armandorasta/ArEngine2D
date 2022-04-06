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

		/**
		 * @brief called every frame. user can only render to screen through here.
		 * @param gfx => a handle to the graphics interface.
		*/
		virtual void OnUserDraw(Grafix& gfx) = 0;

	private:
		// Initialization has it's own function for convience. (see the cpp file)
		void Initialize();
		// currently uses std::chrono.
		float GetFrameDelta();
		void UpdateTitle(float dt);

	public:
		/**
		 * @brief a handle to all the keyboard input.
		*/
		Keyboard const& keyboard{window_.keyboard};

		/**
		 * @brief a handle to all the mouse input.
		*/
		Mouse const& mouse{window_.mouse};

	private:
		Window window_;
		Grafix gfx_;
	};
}

