#pragma once

#include "Key.h"
#include "ISingle.h"

#include <array>
#include <cassert>
#include <queue>
#include <functional>

namespace ArEngine2D {
	class Keyboard : Details::ISingle
	{
		friend class Window;
	public:
		inline static constexpr auto KEY_COUNT{256U};

	public:

		Keyboard()						 = default;
		Keyboard(Keyboard const&)		 = delete;
		Keyboard(Keyboard&&)			 = delete;
		auto& operator=(Keyboard const&) = delete;
		auto& operator=(Keyboard&&)		 = delete;

	public:

		/**
		 * @return the key with the specified id.
		*/
		constexpr auto GetKey(size_t id) const noexcept -> Key const&
		{
			assert(id < Keyboard::KEY_COUNT);
			return keys_[id];
		}

		/**
		 * @return the key with the specified id.
		*/
		constexpr auto operator()(size_t id) const noexcept -> Key const&
		{
			return GetKey(id);
		}

		/**
		 * @return true if the character buffer has at least one character to be handled.
		*/
		auto HasMoreChars() const noexcept(noexcept(chars_.empty())) -> bool
		{
			return not chars_.empty();
		}

		/**
		 * @return the next character in the buffer. (ub if the buffer is empty).
		*/
		auto GetNextChar() -> char
		{
			assert(not chars_.empty() && "called ArEngine::Keyboard::GetNextChar before calling "
				"ArEngine::Keyboard::HasMoreChars; thats ub baby!");

			auto const next{chars_.front()};
			chars_.pop();
			return next;
		}

		/**
		 * @brief same as: while (HasMoreChars()) body(GetNextChar());
		 * @param body => callable which is gonna be called on every character.
		*/
		template <std::invocable<char> Callable>
		void ForEachChar(Callable&& body)
		{
			while (HasMoreChars())
			{
				body(GetNextChar());
			}
		}

	private: // these will be accessed by the Window class
		constexpr void SetKey(size_t keyID, bool state) noexcept
		{
			keys_[keyID].Set(state);
		}

		void FrameUpdate()
		{
			std::ranges::for_each(keys_, [&](auto& key) {
				key.FrameUpdate();
			});
			TrimCharBuffer();
		}

		void PushChar(char what)
		{
			chars_.push(what);
		}

		void TrimCharBuffer()
		{
			while (chars_.size() > 32)
			{
				chars_.pop();
			}
		}

		void Reset()
		{
			std::ranges::fill(keys_, Key{});
			while (not chars_.empty())
			{
				chars_.pop();
			}
		}

	private:
		std::array<Key, KEY_COUNT> keys_;
		std::queue<char> chars_;
	};
}

