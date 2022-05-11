#pragma once

#include "GuiUtil.h"
#include "GuiStatus.h"

ARGUI_BEGIN_NAMESPACE

class GuiRectF
{
public:
	GuiRectF()								= default;
	GuiRectF(GuiRectF const&)				= default;
	GuiRectF(GuiRectF&&)					= default;
	GuiRectF& operator=(GuiRectF const&)	= default;
	GuiRectF& operator=(GuiRectF&&)			= default;

	GuiRectF(float x, float y, float w, float h) noexcept;
	GuiRectF(Vec2 const& loc, float w, float h) noexcept;
	GuiRectF(Vec2 const& v0, Vec2 const& v1) noexcept;

public:
	bool Contains(Vec2 const& point) const noexcept;
	bool Contains(GuiRectF const& that) const noexcept;
	bool Overlaps(GuiRectF const& that) const noexcept;
	bool Intersects(GuiRectF const& that) const noexcept;
	void MoveOutOf(GuiRectF const& that) noexcept;
	Vec2 CalcOverlapVec(GuiRectF const& point) const noexcept;

public:
	void Draw(Grafix& gfx, ColorF const& col, float thick) noexcept;
	void DrawDebug(Grafix& gfx, ColorF const& col, float thick) noexcept;
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
	float GetWidth() const noexcept;
	float GetHeight() const noexcept;
	float GetDiagMag() const noexcept;
	float GetArea() const noexcept;
	float GetPerimeter() const noexcept;
	float GetLeft() const noexcept;
	float GetRight() const noexcept;
	float GetTop() const noexcept;
	float GetBot() const noexcept;
	Vec2 const& GetTopLeft() const noexcept;
	Vec2 GetTopRight() const noexcept;
	Vec2 GetBotLeft() const noexcept;
	Vec2 const& GetBotRight() const noexcept;
	Vec2 GetCenter() const noexcept;

private:
	Vec2 v0_;
	Vec2 v1_;
};

ARGUI_END_NAMESPACE