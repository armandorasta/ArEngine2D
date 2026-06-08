#pragma once

#include "IGuiElement.h"
#include "GuiCore.h"
#include "GuiSize.h"
#include "GuiStatus.h"
#include "GuiRectF.h"

namespace ArGui {
	class GuiButton : public IGuiElement
	{
	private:
		constexpr static auto sc_MinSize{10.f};
		constexpr static auto sc_MaxSize{300.f};

	public:
		GuiButton(Vec2 const& loc, std::string_view text, float width, float height) noexcept;

	public:
		void SetLoc(Vec2 newLoc) noexcept override;
		GuiStatus SetText(std::string_view newText);
		GuiStatus SetWidth(float newWidth) noexcept;
		GuiStatus SetHeight(float newHeight) noexcept;

	public:
		void Draw(Grafix& gfx, Camera const& cam,
			ColorF const& bgColor, ColorF const& lineColor, ColorF const& textColor) noexcept;

	public:
		void Update(Mouse const& mouse, Camera const& cam) noexcept;

	public:
		[[nodiscard]] GuiElementType GetType() const noexcept override;
		[[nodiscard]] Vec2 GetLoc() const noexcept override;
		[[nodiscard]] std::string_view GetText() const noexcept;
		[[nodiscard]] float GetWidth() const noexcept;
		[[nodiscard]] float GetHeight() const noexcept;
		[[nodiscard]] GuiStatus GetStatus() const noexcept;
		[[nodiscard]] GuiRectF GetRectF(Camera const& cam) const noexcept override;

	public:
		[[nodiscard]] bool IsMouseClicked() const noexcept;
		[[nodiscard]] bool IsMouseHeld() const noexcept;
		[[nodiscard]] bool IsMouseJustReleased() const noexcept;
		[[nodiscard]] bool IsMouseEntered() const noexcept;
		[[nodiscard]] bool IsMouseHovered() const noexcept;
		[[nodiscard]] bool IsMouseLeft() const noexcept;

	private:
		void UpdateStatus(bool condition, GuiStatus ifTrueOtherwiseFine) noexcept;
		void ResetStatus() noexcept;
		void UpdateTextSize() noexcept;
		[[nodiscard]] float ClampSize(float size) const noexcept;

	private:
		// looks
		GuiRectF rect_;
		std::string text_;
		float textSize_;

		// logic
		GuiStatus currStatus_{GuiStatus::Fine};

		Vec2 mouseLocLastFrame_;
		bool bMouseClicked_;
		bool bMouseHeld_;
		bool bMouseReleased_;
		bool bMouseLeft_;
		bool bMouseHovered_;
		bool bMouseEntered_;
	};
}
