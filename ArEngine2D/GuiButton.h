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
		constexpr static auto sc_MinSize{50.f};
		constexpr static auto sc_MaxSize{300.f};

	public:
		GuiButton(Vec2 const& loc, std::string_view text, float width, float height,
			ColorF const& activeTextCol, ColorF const& activeBGCol, ColorF const& activeLineCol
		) noexcept;

	public:
		void SetLoc(Vec2 const& newLoc) noexcept;
		GuiStatus SetText(std::string_view newText);
		GuiStatus SetWidth(float newWidth) noexcept;
		GuiStatus SetHeight(float newHeight) noexcept;
		void SetBGColor(ColorF const& newBGCol) noexcept;
		void SetTextColor(ColorF const& newTextCol) noexcept;
		void SetLineColor(ColorF const& newLineCol) noexcept;

	public:
		void Draw(Grafix& gfx, Camera const& cam) override;

	public:
		void Update(Mouse const& mouse, Keyboard const& keyboard, float dt) noexcept override;

		[[nodiscard]] bool IsEnabled() const noexcept;
		void Enable() noexcept;
		void Disable() noexcept;

		[[nodiscard]] bool IsVisible() const noexcept;
		void Show() noexcept;
		void Hide() noexcept;

		[[nodiscard]] bool IsMouseEntered() const noexcept;
		[[nodiscard]] bool IsMouseHovered() const noexcept;
		[[nodiscard]] bool IsMouseLeft() const noexcept;
		[[nodiscard]] bool IsMouseClicked() const noexcept;
		[[nodiscard]] bool IsMouseHeldDown() const noexcept;
		[[nodiscard]] bool IsMouseReleased() const noexcept;

	public:
		[[nodiscard]] Vec2 GetLoc() const noexcept;
		[[nodiscard]] std::string_view GetText() const noexcept;
		[[nodiscard]] float GetWidth() const noexcept;
		[[nodiscard]] float GetHeight() const noexcept;
		[[nodiscard]] ColorF const& GetBGColor() const noexcept;
		[[nodiscard]] ColorF const& GetTextColor() const noexcept;
		[[nodiscard]] ColorF const& GetLineColor() const noexcept;
		[[nodiscard]] GuiStatus GetStatus() const noexcept;
		[[nodiscard]] GuiRectF const& GetRectF() const noexcept;

	private:
		void UpdateStatus(bool condition, GuiStatus ifTrueOtherwiseFine) noexcept;
		void ResetStatus() noexcept;
		void UpdateTextSize() noexcept;
		[[nodiscard]] float ClampSize(float size) const noexcept;

	private:
		// looks
		GuiRectF rect_;
		ColorF bgCol_;
		ColorF textCol_;
		ColorF lineCol_;
		std::string text_;
		float textSize_;

		// logic
		GuiStatus currStatus_{GuiStatus::Fine};
		bool bActive_{true};
		bool bVisible_{true};

		Vec2 mouseLocLastFrame_{};
		bool bMouseClicked_{};
		bool bMouseReleased_{};
		bool bMouseHeld_{};
		bool bMouseEntered_{};
		bool bMouseLeft_{};
		bool bMouseHovered_{};
	};
}
