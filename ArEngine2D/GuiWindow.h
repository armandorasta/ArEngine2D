#pragma once
#include "IGuiElement.h"
#include "GuiCore.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "Camera.h"

namespace ArGui {
	// SetText
	// SetColor
	// SetBGColor
	// SetSize
	// SetRect
	// SetColor
	// BGEnabled
	// DisableBG
	// EnableBG
	// Draw
	//

	class GuiWindowRegistery
	{
	private:
		Uint id_;
		Vec2 offset_;
		bool bAlwaysUpdate_;
		IGuiElement* ptr_;
	};


	// for now, no resizeable windows.
	class GuiWindow : public IGuiElement
	{
	private:
		constexpr static auto sc_TitleBarHeight{20.f};
		constexpr static auto sc_MinHeight{sc_TitleBarHeight};
		constexpr static auto sc_MinWidth{2.f * sc_TitleBarHeight + 6.f};
		constexpr static auto sc_CloseButtSize{sc_TitleBarHeight * 0.8f};

	public:

		GuiWindow(std::string_view name, float width, float height, Vec2 loc = {});

	public:

		void Update(Mouse const& mouse, Keyboard const& keyboard, Camera const& cam);
		void Draw(Grafix& gfx, Camera const& cam) noexcept;

		void SetLoc(Vec2 newLoc) noexcept override;

		[[nodiscard]] GuiElementType GetType() const noexcept override;
		[[nodiscard]] Vec2 GetLoc() const noexcept override;
		[[nodiscard]] GuiRectF GetRectF(Camera const& cam) const noexcept override;

	private:
		void UpdateComponentLocs();

	private:
		Vec2 loc_;
		float width_;
		float height_;
		std::string title_;

		// Components
		GuiButton butClose_;
		GuiButton butMinimize_;
		std::vector<std::pair<IGuiElement*, Vec2>> componentOffsets_;

		// Drag info
		bool bMouseLanded_{};
		Vec2 mouseLandLoc_;
		Vec2 windowLandLoc_;

		// Focus info
		bool bMinimized_{false};
	};
}