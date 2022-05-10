#include "Mouse.h"

namespace ArEngine2D {
	auto Mouse::HasWheelEvents() const noexcept(noexcept(wheelQ_.empty()))-> bool
	{
		return not wheelQ_.empty();
	}

	/**
	* @brief calling this before HasWheelEvents is ub.
	* @return the next wheel event.
	*/

	auto Mouse::GetNextWheelEvent() noexcept(noexcept(wheelQ_.empty())
		and noexcept(wheelQ_.front()) and noexcept(wheelQ_.pop()))-> WheelEvent
	{
		assert(not wheelQ_.empty() && "called ArEngine::Mouse::GetNextWheelEvent before calling "
			"ArEngine::Mouse::HasWheelEvents; thats ub baby!");
		auto const& event{wheelQ_.front()};
		wheelQ_.pop();
		return event;
	}
	void Mouse::FrameUpdate()
	{
		std::ranges::for_each(keys_, [&](auto& key) {
			key.FrameUpdate();
		});
		TrimWheelBuffer();
		rawDel_ = std::exchange(rawDelBuffer_, Vec2::Zero);
	}
	void Mouse::OnWheel(bool isUp)
	{
		wheelQ_.push(WheelEvent{isUp});
	}
	void Mouse::TrimWheelBuffer() noexcept(noexcept(wheelQ_.size() and noexcept(wheelQ_.pop())))
	{
		while (wheelQ_.size() > 32)
		{
			wheelQ_.pop();
		}
	}
	void Mouse::OnRawInput(RAWINPUT const& rawInput) noexcept
	{
		rawDelBuffer_ += Vec2{
			static_cast<float>(rawInput.data.mouse.lLastX),
			static_cast<float>(rawInput.data.mouse.lLastY),
		};
	}
}