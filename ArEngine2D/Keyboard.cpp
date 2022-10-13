#include "Keyboard.h"

namespace ArEngine2D {
	auto Keyboard::HasMoreChars() const noexcept(noexcept(chars_.empty()))-> bool
	{
		return not chars_.empty();
	}
	auto Keyboard::GetNextChar() noexcept(noexcept(chars_.front()) and 
		noexcept(chars_.empty()) and noexcept(chars_.pop()))-> char
	{
		assert(not chars_.empty() && "called ArEngine::Keyboard::GetNextChar before calling "
			"ArEngine::Keyboard::HasMoreChars; thats ub baby!");

		auto const next{chars_.front()};
		chars_.pop();
		return next;
	}
	void Keyboard::FrameUpdate()
	{
		std::ranges::for_each(keys_, [&](auto& key) {
			key.FrameUpdate();
		});
		TrimCharBuffer();
	}
	void Keyboard::PushChar(char what)
	{
		chars_.push(what);
	}
	void Keyboard::TrimCharBuffer() noexcept(noexcept(chars_.size()) and noexcept(chars_.pop()))
	{
		auto const count{static_cast<std::int32_t>(chars_.size())};
		for (std::int32_t i{}; i < count - 32; ++i)
		{
			chars_.pop();
		}
	}
	void Keyboard::Reset()
	{
		std::ranges::fill(keys_, Key{ });
		while (not chars_.empty())
		{
			chars_.pop();
		}
	}
	KeyEvent Keyboard::ReadKey()
	{
		AR2D_ASSERT(!keyEvents_.empty(), "Tried to readed from the keyboard when no keys are to be read.");
		auto const& ev{keyEvents_.front()};
		keyEvents_.pop();
		return ev;
	}
	bool Keyboard::HasMoreKeys() const noexcept
	{ return !keyEvents_.empty(); }
	void Keyboard::FlushKeys()
	{ 
		while (!keyEvents_.empty())
		{
			keyEvents_.pop();
		}
	}
	void Keyboard::FlushChars()
	{
		while (!chars_.empty())
		{ 
			chars_.pop(); 
		}
	}
	void Keyboard::EnableAutoRepeat() noexcept
	{ bAutoRepeat_ = true; }
	void Keyboard::DisableAutoRepeat() noexcept
	{ bAutoRepeat_ = false; }
	bool Keyboard::IsAutoRepeatEnabled() const noexcept
	{ return bAutoRepeat_; }
}