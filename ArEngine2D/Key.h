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
		 * @brief needs to be called after rendering to the screen for some reason.
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
		NUM0 = VK_NUMPAD0, NUM1 = VK_NUMPAD1, NUM2 = VK_NUMPAD2, NUM3 = VK_NUMPAD3, NUM4 = VK_NUMPAD4, 
		NUM5 = VK_NUMPAD5, NUM6 = VK_NUMPAD6, NUM7 = VK_NUMPAD7, NUM8 = VK_NUMPAD8, NUM9 = VK_NUMPAD9,
		// calculator keys
		MUL = VK_MULTIPLY, DIV = VK_DIVIDE, ADD = VK_ADD, SUB = VK_SUBTRACT, DECIMAL = VK_DECIMAL, 
		// function keys
		F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4  = VK_F4 , F5  = VK_F5 , F6  = VK_F6 , 
		F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,
		// arrow keys
		UP = VK_UP, DOWN = VK_DOWN, LEFT = VK_LEFT, RIGHT = VK_RIGHT,
		// control
		CONTROL = VK_CONTROL, LEFT_CONTROL = VK_LCONTROL, RIGHT_CONTROL = VK_RCONTROL,
		// shift
		SHIFT = VK_SHIFT,  LEFT_SHIFT = VK_LSHIFT, RIGHT_SHIFT = VK_RSHIFT,
		// alt which is called menu for some unknown reason
		ALT = VK_MENU, LEFT_ALT = VK_LMENU, RIGHT_ALT = VK_RMENU,
		// other keys
		ESCAPE = VK_ESCAPE, TAB = VK_TAB, NUMLOCK = VK_NUMLOCK, 
		ENTER = VK_RETURN, SPACE = VK_SPACE, BACKSPACE = VK_BACK, DEL /*DELETE is a macro*/ = VK_DELETE, 
		INSERT = VK_INSERT, HOME = VK_HOME, END = VK_END,
		PRINT = VK_PRINT,
	};

	// this function must be here because, it makes changing the underlying 
	// implementation easier.
	constexpr std::size_t KeyToID(Keys key)
	{
		return static_cast<std::size_t>(key);
	}
}