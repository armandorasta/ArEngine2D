#pragma once

#include "IEngineError.h"

namespace ArEngine2D {
	class Key
	{
	public:

		constexpr Key()							= default;
		constexpr Key(Key const&)				= default;
		constexpr Key(Key&&)					= default;
		constexpr Key& operator=(Key const&)	= default;
		constexpr Key& operator=(Key&&)			= default;

	public:
		
		/**
		 * @brief must be called every frame for input to be processed correctly (not by the user).
		*/
		constexpr void FrameUpdate() noexcept
		{
			bPress_ = bStateCurrFrame_ and not bStateLastFrame_;
			bDown_  = bStateCurrFrame_;
			bStateLastFrame_ = bStateCurrFrame_;
		}

		/**
		 * @brief changes the state for the current frame.
		 * @param state => the new state.
		*/
		constexpr void Set(bool state) noexcept
		{
			bStateCurrFrame_ = state;
		}

		/**
		 * @return true if the mouse was down this frame but not the frame before, returns false otherwise.
		*/
		constexpr auto IsPressed() const noexcept
		{ return bPress_; }

		/**
		 * @return true if the mouse is held down, returns false otherwise.
		*/
		constexpr auto IsDown() const noexcept
		{ return bDown_; }

		/**
		 * @return true when Key::Down returns false, returns false otherwise. (yes)
		*/
		constexpr auto IsUp() const noexcept
		{ return not IsDown(); }

	private:
		bool bPress_;
		bool bDown_;
		bool bStateCurrFrame_;
		bool bStateLastFrame_;
	};

	enum class Keys : std::size_t
	{
		// alphabet
		A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', 
		J = 'J', K = 'K', L = 'L', M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R', 
		S = 'S', T = 'T', U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y', Z = 'Z',
		// number pad
		Num0 = VK_NUMPAD0, Num1 = VK_NUMPAD1, Num2 = VK_NUMPAD2, Num3 = VK_NUMPAD3, Num4 = VK_NUMPAD4, 
		Num5 = VK_NUMPAD5, Num6 = VK_NUMPAD6, Num7 = VK_NUMPAD7, Num8 = VK_NUMPAD8, Num9 = VK_NUMPAD9,
		// calculator keys
		Mul = VK_MULTIPLY, Div = VK_DIVIDE, Add = VK_ADD, Sub = VK_SUBTRACT, Decimal = VK_DECIMAL, 
		// function keys
		F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4  = VK_F4 , F5  = VK_F5 , F6  = VK_F6 , 
		F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,
		// arrow keys
		Up = VK_UP, Down = VK_DOWN, Left = VK_LEFT, Right = VK_RIGHT,
		// control
		Control = VK_CONTROL, LeftControl = VK_LCONTROL, RightControl = VK_RCONTROL,
		// shift
		Shift = VK_SHIFT,  LeftShift = VK_LSHIFT, RightShift = VK_RSHIFT,
		// alt which is called menu for some unknown reason
		Alt = VK_MENU, LeftAlt = VK_LMENU, RightAlt = VK_RMENU,
		// other keys
		Escape = VK_ESCAPE, Tab = VK_TAB, NumLock = VK_NUMLOCK, 
		Enter = VK_RETURN, Space = VK_SPACE, Backspace = VK_BACK, Delete /*DELETE is a macro*/ = VK_DELETE, 
		Insert = VK_INSERT, Home = VK_HOME, End = VK_END,
		Print = VK_PRINT,
	};

	// this function must be here because, it makes changing the underlying 
	// implementation easier.
	constexpr std::size_t KeyToID(Keys key)
	{
		return static_cast<std::size_t>(key);
	}
}