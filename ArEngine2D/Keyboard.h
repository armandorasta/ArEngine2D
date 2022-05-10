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
	private:
		constexpr static std::size_t sc_KeyCount{256U};

	public:

		Keyboard() = default;

	public:

		/**
		 * @return the state of the specified key.
		*/
		constexpr auto GetKey(Keys key) const noexcept -> Key const&
		{
			auto const id{KeyToID(key)};
			assert(id < Keyboard::sc_KeyCount);
			return keys_[id];
		}

		/**
		 * @return the state of the specified key.
		*/
		constexpr auto operator()(Keys key) const noexcept -> Key const&
		{
			return GetKey(key);
		}

		/**
		 * @return true if the character buffer has at least one character to be handled.
		*/
		auto HasMoreChars() const noexcept(noexcept(chars_.empty())) -> bool;

		/**
		 * @return the next character in the buffer. (ub if the buffer is empty).
		*/
		auto GetNextChar() noexcept(
			noexcept(chars_.front()) and 
			noexcept(chars_.empty()) and 
			noexcept(chars_.pop())
			) -> char;

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
		constexpr void SetKey(std::size_t keyID, bool state) noexcept
		{
			keys_[keyID].Set(state);
		}

		void FrameUpdate();
		void PushChar(char what);
		void TrimCharBuffer() noexcept(noexcept(chars_.size()) and noexcept(chars_.pop()));
		void Reset();

	private:
		std::array<Key, sc_KeyCount> keys_;
		std::queue<char> chars_;
	};
}

