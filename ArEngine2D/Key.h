#pragma once

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
		
		constexpr void FrameUpdate() noexcept
		{
			bPress_ = bStateCurrFrame_ and not bPress_;
			bDown_  = bStateCurrFrame_;
		}

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
	};

	enum class Keys
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
		MUL, DIV, ADD, SUB, DECIMAL, 
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		CONTROL, LEFT_CONTROL, RIGHT_CONTROL,
		SHIFT,  LEFT_SHIFT, RIGHT_SHIFT,
		ALT, LEFT_ALT, RIGHT_ALT,
		ESCAPE, TAB, CAPSLOCK, NUMLOCK, 
		ENTER, SPACE, BACKSPACE, DEL /*DELETE is a macro*/, INSERT, HOME, END,
		PRINT,
	};
}