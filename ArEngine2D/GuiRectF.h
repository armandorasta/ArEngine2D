#pragma once

#include "GuiUtil.h"
#include "GuiStatus.h"

namespace ArEngine2D {
	class Camera;
}

namespace ArGui {
	class GuiRectF
	{
	public:
		GuiRectF() = default;
		GuiRectF(GuiRectF const&) = default;
		GuiRectF(GuiRectF&&) = default;
		GuiRectF& operator=(GuiRectF const&) = default;
		GuiRectF& operator=(GuiRectF&&) = default;

		/**
		 * @param x => X coord.
		 * @param y => Y coord.
		 * @param w => Width.
		 * @param h => Height.
		*/
		GuiRectF(float x, float y, float w, float h) noexcept;

		/**
		 * @param loc => Location.
		 * @param w => Width.
		 * @param h => Height.
		*/
		GuiRectF(Vec2 const& loc, float w, float h) noexcept;

		/**
		 * @param v0 => Top left.
		 * @param v1 => Bottom right.
		*/
		GuiRectF(Vec2 const& v0, Vec2 const& v1) noexcept;

	public:


		/**
		 * @return true if the point is inside the rectangle, false otherwise.
		*/
		[[nodiscard]]
		bool Contains(Vec2 const& point) const noexcept;

		/**
		 * @return true if the passed rectangle is full contained within the
		 *		   current rectangle; returns false otherwise.
		*/
		[[nodiscard]]
		bool Contains(GuiRectF const& that) const noexcept;

		/**
		 * @return true if the two rectangles overlap; returns false otherwise.
		*/
		[[nodiscard]]
		bool Overlaps(GuiRectF const& that) const noexcept;

		/**
		 * @return true if the two rectangles are overlapping,
		 *         but do not contain one another; returns false otherwise.
		*/
		[[nodiscard]]
		bool Intersects(GuiRectF const& that) const noexcept;

		/**
		 * @return Resolves the collision between the two rectangles by moving
		 *		   the current rectangle out of the passed rectangle.
		*/
		void MoveOutOf(GuiRectF const& that) noexcept;

		/**
		 * @return A vector with the least amount of distance required to
		 *         move the point out of the rectangle.
		*/
		[[nodiscard]]
		Vec2 CalcOverlapVec(GuiRectF const& point) const noexcept;

	public:

		/**
		 * @param gfx => Grafix object used to display things on screen.
		 * @param col => Color of the line.
		 * @param thick => Thickness of the line.
		*/
		void Draw(Grafix& gfx, ColorF const& col, float thick) noexcept;

		/**
		 * @brief
		 * @param gfx => Grafix object used to display things on screen.
		 * @param cam => Camera to look from.
		 * @param col => Color of the line.
		 * @param thick => Thickness of the line.
		*/
		void Draw(Grafix& gfx, Camera cam, ColorF const& col, float thick) noexcept;
		void DrawDebug(Grafix& gfx, ColorF const& col, float thick) noexcept;
		void Fill(Grafix& gfx, Camera cam, ColorF const& col) noexcept;
		void Fill(Grafix& gfx, ColorF const& col) noexcept;

	public:
		void SetWidth(float newWidth) noexcept;
		void SetHeight(float newHeight) noexcept;
		void ScaleTopLeft(float newScale) noexcept;
		void ScaleCenter(float newScale) noexcept;
		void Move(Vec2 const& delta) noexcept;

		// raw setters (only make sure that left is left and right is right)
		void SetLeft(float newLeft) noexcept;
		void SetTop(float newTop) noexcept;
		void SetRight(float newRight) noexcept;
		void SetBot(float newBot) noexcept;
		void SetTopLeft(Vec2 const& newTopLeft) noexcept;
		void SetTopRight(Vec2 const& newTopRight) noexcept;
		void SetBotLeft(Vec2 const& newBotLeft) noexcept;
		void SetBotRight(Vec2 const& newBotRight) noexcept;
		void SetCenter(Vec2 const& newCenter) noexcept;

		// maintain the dimensions of the rectangle.
		void RelLeft(float newLeft) noexcept;
		void RelTop(float newTop) noexcept;
		void RelRight(float newRight) noexcept;
		void RelBot(float newBot) noexcept;
		void RelTopLeft(Vec2 const& newTopLeft) noexcept;
		void RelTopRight(Vec2 const& newTopRight) noexcept;
		void RelBotLeft(Vec2 const& newBotLeft) noexcept;
		void RelBotRight(Vec2 const& newBotRight) noexcept;
		void RelCenter(Vec2 const& newCenter) noexcept;

	private:
		void SwapXIfNeeded() noexcept;
		void SwapYIfNeeded() noexcept;
		void SwapAnyIfNeeded() noexcept;

	public:
		[[nodiscard]] float GetWidth() const noexcept;
		[[nodiscard]] float GetHeight() const noexcept;
		[[nodiscard]] float GetDiagMag() const noexcept;
		[[nodiscard]] float GetArea() const noexcept;
		[[nodiscard]] float GetPerimeter() const noexcept;
		[[nodiscard]] float GetLeft() const noexcept;
		[[nodiscard]] float GetRight() const noexcept;
		[[nodiscard]] float GetTop() const noexcept;
		[[nodiscard]] float GetBot() const noexcept;
		[[nodiscard]] Vec2 GetTopLeft() const noexcept;
		[[nodiscard]] Vec2 GetTopRight() const noexcept;
		[[nodiscard]] Vec2 GetBotLeft() const noexcept;
		[[nodiscard]] Vec2 GetBotRight() const noexcept;
		[[nodiscard]] Vec2 GetCenter() const noexcept;

	private:
		Vec2 v0_;
		Vec2 v1_;
	};
}
