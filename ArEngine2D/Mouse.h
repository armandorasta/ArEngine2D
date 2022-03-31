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
	public:
		inline static constexpr auto KEY_COUNT{3U};

	private:
		inline static constexpr auto LEFT_BUTTON_ID{0U};
		inline static constexpr auto MID_BUTTON_ID{1U};
		inline static constexpr auto RIGHT_BUTTON_ID{2U};

	public:
		class WheelEvent
		{
		public:
			constexpr explicit WheelEvent(bool isUp) noexcept : bUp_{isUp}
			{ }

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
		{
			return bInWindow_;
		}

		/**
		 * @return true if there are mouse wheel events to be handled.
		 */
		auto HasWheelEvents() const -> bool
		{
			return not wheelQ_.empty();
		}

		/**
		 * @brief calling this before HasWheelEvents is ub.
		 * @return the next wheel event.
		*/
		auto GetNextWheelEvent() -> WheelEvent
		{
			assert(HasWheelEvents());
			auto const& event{wheelQ_.front()};
			wheelQ_.pop();
			return event;
		}

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
		 * @return 
		 *		a vector with the input info if raw input is enabled, 
		 *		a default constructed vector otherwise.
		*/
		constexpr Vec2 const& GetRawDelta() const noexcept
		{
			return rawDel_;
		}

	private:

		constexpr void SetKey(size_t id, bool state) noexcept
		{
			assert(id < Mouse::KEY_COUNT);
			keys_[id].Set(state);
		}

		constexpr void FrameUpdate()
		{
			std::ranges::for_each(keys_, [&](auto& key) {
				key.FrameUpdate();
			});

			rawDel_ = std::exchange(rawDelBuffer_, Vec2::Zero);
		}

		constexpr void OnWindowExit() noexcept
		{
			bInWindow_ = false;
		}

		constexpr void OnWindowEnter() noexcept
		{
			bInWindow_ = true;
		}

		constexpr void SetLoc(float x, float y) noexcept
		{
			loc_ = {x, y};
		}

		void OnWheel(bool isUp)
		{
			wheelQ_.push(WheelEvent{isUp});
			TrimWheelBuffer();
		}

		void TrimWheelBuffer()
		{
			while (wheelQ_.size() > 32)
			{
				wheelQ_.pop();
			}
		}

		void OnRawInput(RAWINPUT const& rawInput) noexcept
		{
			rawDelBuffer_ += Vec2{
				static_cast<float>(rawInput.data.mouse.lLastX),
				static_cast<float>(rawInput.data.mouse.lLastY),
			};
		}

	public:

		// i like this syntax more than the getters,
		// i have to do it this way because c++ does not support properties.

		/**
		 * @brief the left mouse button.
		*/
		Key const& left{keys_[LEFT_BUTTON_ID]};

		/**
		 * @brief the middle mouse button.
		*/
		Key const& mid{keys_[MID_BUTTON_ID]};

		/**
		 * @brief the right mouse button.
		*/
		Key const& right{keys_[RIGHT_BUTTON_ID]};

		/**
		 * @brief the mouse location.
		*/
		Vec2 const& loc{loc_};

	private:
		Vec2 loc_{};
		Vec2 rawDel_{};
		Vec2 rawDelBuffer_{};
		std::array<Key, KEY_COUNT> keys_{};
		std::queue<WheelEvent> wheelQ_{};

		bool bInWindow_{};
	};
}