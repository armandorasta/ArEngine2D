#pragma once

#include "Vec2.h"
#include "Key.h"
#include "ISingle.h"

#include <functional>
#include <queue>
#include <algorithm>
#include <array>


namespace ArEngine2D {
	class Mouse : Details::ISingle
	{
		friend class Window;
	private:
		constexpr static std::size_t sc_KeyCount{3U};
		constexpr static std::size_t sc_LeftButtonID{0U};
		constexpr static std::size_t sc_MiddleButtonID{1U};
		constexpr static std::size_t sc_RightMouseID{2U};

	public:

		class WheelEvent
		{
		public:
			constexpr explicit WheelEvent(bool isUp) noexcept : bUp_{isUp} { }

			constexpr bool IsUp() const noexcept 
			{ return bUp_; }
			constexpr bool IsDown() const noexcept 
			{ return not IsUp(); }

		private:
			bool bUp_;
		};

	public:

		Mouse()	= default;

	public:

		/**
		 * @return true if the mouse is inside the window.
		*/
		constexpr auto IsInWindow() const noexcept -> bool
		{ return bInWindow_; }

		/**
		 * @return true if there are mouse wheel events to be handled.
		 */
		auto HasWheelEvents() const noexcept(noexcept(wheelQ_.empty())) -> bool;

		/**
		 * @brief calling this before HasWheelEvents is ub.
		 * @return the next wheel event.
		*/
		auto GetNextWheelEvent() noexcept(
			noexcept(wheelQ_.empty()) and
			noexcept(wheelQ_.front()) and 
			noexcept(wheelQ_.pop())
			)->WheelEvent;

		/**
		 * @brief same as: while (HasWheelEvents()) body(GetNextWheelEvent().IsUp());
		 * @param body => called on every wheel event in the queue.
		*/
		template <std::invocable<WheelEvent> Callable>
		void ForEachWheel(Callable&& body)
		{
			while (HasWheelEvents())
			{
				body(GetNextWheelEvent());
			}
		}

		/**
		 * @brief 
		 *		gets the physical input of the mouse (same as taking a delta
		 *		of the mouse locations of the current frame and the last one).
		 *		calling this function before enabling raw input from the window is well-defined.
		 * @return 
		 *		a vector with the input info if raw input is enabled, (0, 0) otherwise.
		*/
		constexpr Vec2 const& GetRawDelta() const noexcept
		{ return rawDel_; }

	private:

		constexpr void SetKey(std::size_t id, bool state) noexcept
		{
			assert(id < Mouse::sc_KeyCount);
			keys_[id].Set(state);
		}


		constexpr void OnWindowExit() noexcept
		{ bInWindow_ = false; }

		constexpr void OnWindowEnter() noexcept
		{ bInWindow_ = true; }

		constexpr void SetLoc(float x, float y) noexcept
		{ loc_ = {x, y}; }

		void FrameUpdate();
		void OnWheel(bool isUp);
		void TrimWheelBuffer() noexcept(noexcept(wheelQ_.size() and noexcept(wheelQ_.pop())));
		void OnRawInput(RAWINPUT const& rawInput) noexcept;

	public:

		// i like this syntax more than the getters,
		// i have to do it this way because c++ does not support properties.

		/**
		 * @brief the left mouse button.
		*/
		Key const& left{keys_[sc_LeftButtonID]};

		/**
		 * @brief the middle mouse button.
		*/
		Key const& mid{keys_[sc_MiddleButtonID]};

		/**
		 * @brief the right mouse button.
		*/
		Key const& right{keys_[sc_RightMouseID]};

		/**
		 * @brief the mouse location.
		*/
		Vec2 const& loc{loc_};

	private:
		Vec2 loc_{};
		Vec2 rawDel_{};
		Vec2 rawDelBuffer_{};
		std::array<Key, sc_KeyCount> keys_{};
		std::queue<WheelEvent> wheelQ_{};

		// true if the mouse is inside the window or outside while dragging.
		bool bInWindow_{};
	};
}