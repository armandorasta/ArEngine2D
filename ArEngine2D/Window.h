#pragma once

#include "IEngineError.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <string>

namespace ArEngine2D {
	class Window : Details::ISingle
	{
		friend class Engine;
	private:
		constexpr static auto sc_ClassName  = L"class name";
		constexpr static UINT sc_ClassStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		constexpr static UINT sc_WinStyle   = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;

		inline static Window* s_ActiveWindow{};

	public:

		/**
		 * @brief only constructor.
		 * @param title => text that will apear on the top left (mostly) of your window.
		 * @param width => the actual width of the client area.
		 * @param height => the actual height of the client area.
		*/
		Window(std::string_view title, std::int32_t width, std::int32_t height, std::int32_t x = 0, std::int32_t y = 0);
		~Window();

	public:

		/**
		 * @brief changes the title of the window to the one passed but,
		 *		  does not update the text yet.
		 * @param title => the new title.
		*/
		void SetTitle(std::string const& title) noexcept
		{ title_ = title; }

		/**
		 * @brief updates the title text if it was changed (through SetTitle).
		*/
		void UpdateTitle() noexcept;

	protected:
		void Initialize();
		auto ProcessMessages() noexcept -> bool;
		
		void InputUpdate() noexcept;

		void InitializeRawInput();
		void EnableRawInput();
		constexpr void DisableRawInput() noexcept
		{ s_bRawInputEnabled_ = false; }

	private:
		void InitializeClass();

	private:
		auto CALLBACK ActiveWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
			->LRESULT;
		static auto CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
			->LRESULT;
	public:
		/**
		 * @return the handle of the game window (HWND).
		*/
		constexpr auto Handle() const noexcept
		{ return handle_; }

		/**
		 * @return the actual width of the game window.
		*/
		constexpr auto Width() const noexcept
		{ return width_;  }

		/**
		 * @return the actual height of the game window.
		*/
		constexpr auto Height() const noexcept
		{ return height_; }
		
		/**
		 * @return the title of the game window.
		*/
		constexpr auto Title() const noexcept
		{ return title_;  }

		/**
		 * @return the location of the game window.
		*/
		constexpr auto Loc() const noexcept
		{ return Vec2{static_cast<float>(x_), static_cast<float>(y_)}; }

		/**
		 * @return the location of the center of the game window.
		*/
		constexpr auto Center() const noexcept
		{ return Vec2{static_cast<float>(width_), static_cast<float>(height_)} * 0.5f; }

		/**
		 * @return 
		 *		true if raw input (input taken directly from the 
		 *		hardware) is enabled, false otherwise.
		*/
		constexpr auto IsRawInputEnabled() const noexcept
		{ return s_bRawInputEnabled_; }

	public:

		/**
		 * @brief provides a way to access keyboard related functions.
		*/
		Keyboard keyboard{};

		/**
		 * @brief provides a way to access mouse related functions.
		*/
		Mouse mouse{};

	private:
		HWND handle_;

		std::int32_t x_;
		std::int32_t y_;

		std::int32_t width_;
		std::int32_t height_;

		std::string title_;
		std::wstring wtitle_;

		inline static bool s_bRawInputEnabled_{};
		inline static bool s_bRawInputInitialized_{};
		std::vector<std::byte> rawInputData{};
	};
}

