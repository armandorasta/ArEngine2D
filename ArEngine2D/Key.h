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
}