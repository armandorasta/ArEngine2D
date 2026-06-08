#pragma once
#include "GuiCore.h"
#include "IGuiElement.h"
#include "GuiWindow.h"
#include "GuiButton.h"
#include "GuiSlider.h"

namespace ArGui {
	class GuiWindowStack
	{
	public:

		GuiWindowStack() = default;
		GuiWindowStack(GuiWindowStack const&) = delete;
		GuiWindowStack(GuiWindowStack&&) = delete;
		auto operator=(GuiWindowStack const&) = delete;
		auto operator=(GuiWindowStack&&) = delete;

	public:

		void Push(std::unique_ptr<GuiWindow> pNewWindow)
		{
			data_.push_back(std::move(pNewWindow));
		}
		void Pop()
		{
			data_.pop_back();
		}
		void Clear()
		{
			data_.clear();
		}
		GuiWindow const& ActiveWindow() const noexcept
		{
			return *data_.back();
		}
		void Draw(Grafix& gfx, Camera const& cam) const noexcept
		{
			for (auto const& p : data_)
			{
				p->Draw(gfx, cam);
			}
		}
		void Update(Mouse const& mouse, Keyboard const& keyboard, Camera const& cam)
		{
			if (mouse.left.IsPressed())
			{
				// Puts the right window on top.
				EnableTopWindowContainsMouse(mouse, cam);
			}

			data_.back()->Update(mouse, keyboard, cam);
		}

	private:
		void EnableTopWindowContainsMouse(Mouse const& mouse, Camera const& cam)
		{
			// Go from top to bottom...
			for (auto it{data_.rbegin()}; it != data_.rend(); ++it)
			{
				// First find a window the click landed within.
				if (!(*it)->GetRectF(cam).Contains(mouse.loc))
				{
					continue;
				}

				// Deactivate the current window at top.
				data_.back()->Enabled = false;

				// Bubble up to the top of the stack
				for (; it != data_.rbegin(); --it)
				{
					std::swap(*it, *std::prev(it));
				}

				// Activate the new top window.
				data_.back()->Enabled = true;
				return;
			}
		}

	private:
		std::vector<std::unique_ptr<GuiWindow>> data_;
	};

	class GuiManager
	{
	public:

		GuiManager(Grafix& gfx)
		{
		}
		~GuiManager();

	private:
		GuiWindowStack stack_;
	};
}
