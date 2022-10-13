#include "GuiRectF.h"

#include "Camera.h"

namespace ArGui {
	GuiRectF::GuiRectF(float x, float y, float w, float h) noexcept
		: v0_{x, y}, v1_{x + w, y + h}
	{ }

	GuiRectF::GuiRectF(Vec2 const& loc, float w, float h) noexcept
		: GuiRectF{loc.x, loc.y, w, h}
	{ }

	GuiRectF::GuiRectF(Vec2 const& v0, Vec2 const& v1) noexcept
		: GuiRectF{v0.x, v0.y, v1.x, v1.y}
	{
		if (v0_.x > v1_.x)
		{
			std::swap(v0_.x, v1_.x);
		}
		if (v0_.y > v1_.y)
		{
			std::swap(v0_.y, v1_.y);
		}
	}

	bool GuiRectF::Contains(Vec2 const& point) const noexcept
	{
		return {
			point.x > v0_.x && point.x < v1_.x&&
			point.y > v0_.y && point.y < v1_.y
		};
	}

	bool GuiRectF::Contains(GuiRectF const& that) const noexcept
	{
		return {
			v0_.x <= that.v0_.x && v1_.x >= that.v1_.x &&
			v0_.y <= that.v0_.y && v1_.y >= that.v1_.y
		};
	}

	bool GuiRectF::Overlaps(GuiRectF const& that) const noexcept
	{
		return { // operators < and > from Vec2 don't do the right thing :/ 
			v0_.x <= that.v1_.x && v1_.x >= that.v0_.x &&
			v0_.y <= that.v1_.y && v1_.y >= that.v0_.y
		};
	}

	bool GuiRectF::Intersects(GuiRectF const& that) const noexcept
	{ return Overlaps(that) && !Contains(that) && !that.Contains(*this); }

	void GuiRectF::MoveOutOf(GuiRectF const& that) noexcept
	{
		ARGUI_ASSERT(this != &that, "Tried to move out of self, which is never gonna happen, baby.");
		if (Overlaps(that))
		{
			Move(CalcOverlapVec(that));
		}
	}

	Vec2 GuiRectF::CalcOverlapVec(GuiRectF const& that) const noexcept
	{
		auto const del0{that.v1_ - v0_};
		auto const del1{that.v0_ - v1_};

		auto const xDel = [&]
		{
			if (std::abs(del0.x) < std::abs(del1.x))
			{
				return del0.x;
			}
			else
			{
				return del1.x;
			}
		}();

		auto const yDel = [&]
		{
			if (std::abs(del0.y) < std::abs(del1.y))
			{
				return del0.y;
			}
			else
			{
				return del1.y;
			}
		}();

		// move out with the least amount of distance possible.
		if (std::abs(yDel) < std::abs(xDel))
		{
			return Vec2{0.f, yDel};
		}
		else // if greater than or equal which (hopefully) will pioritize the x-axis.
		{
			return Vec2{xDel, 0.f};
		}
	}

	void GuiRectF::Draw(Grafix& gfx, ColorF const& col, float thick) noexcept
	{ gfx.DrawRectangle(v0_, v1_, col, thick); }

	void GuiRectF::Draw(Grafix& gfx, Camera cam, ColorF const& col, float thick) noexcept
	{ gfx.DrawRectangle(cam(v0_), cam(v1_), col, thick); }

	void GuiRectF::DrawDebug(Grafix& gfx, ColorF const& col, float thick) noexcept
	{
		Draw(gfx, col, thick);
		gfx.FillCircle(v0_, 5.f, Colors::Red);
		gfx.FillCircle(v1_, 5.f, Colors::Red);
		gfx.DrawString({v0_.x, v0_.y}, std::format("{}", v0_), Colors::Red, 20.f);
		gfx.DrawString({v1_.x, v1_.y}, std::format("{}", v1_), Colors::Red, 20.f);
		gfx.DrawString({v0_.x + 0.5f * (v1_.x - v0_.x), v1_.y}, std::format("{}", GetWidth()), Colors::Red, 20.f);
		gfx.DrawString({v0_.x, v0_.y + 0.5f * (v1_.y - v0_.y)}, std::format("{}", GetHeight()), Colors::Red, 20.f);
	}

	void GuiRectF::Fill(Grafix& gfx, Camera cam, ColorF const& col) noexcept
	{ gfx.FillRectangle(cam(v0_), cam(v1_), col); }

	void GuiRectF::Fill(Grafix& gfx, ColorF const& col) noexcept
	{ gfx.FillRectangle(v0_, v1_, col); }

	void GuiRectF::SetWidth(float newWidth) noexcept
	{
		ARGUI_ASSERT(newWidth > 0, "Invalid width");
		v1_.x = v0_.x + newWidth;
	}

	void GuiRectF::SetHeight(float newHeight) noexcept
	{
		ARGUI_ASSERT(newHeight > 0, "Invalid height");
		v1_.y = v0_.y + newHeight;
	}

	void GuiRectF::ScaleTopLeft(float newScale) noexcept
	{
		ARGUI_ASSERT(newScale > 0, "Invalid scale");
		v1_ = {
			v0_.x + (v1_.x - v0_.x) * newScale,
			v0_.y + (v1_.y - v0_.y) * newScale,
		};
	}

	void GuiRectF::ScaleCenter(float newScale) noexcept
	{
		auto offVec = (v1_ - v0_) * (newScale * 0.5f);
		v0_ -= offVec;
		v1_ += offVec;
	}

	void GuiRectF::Move(Vec2 const& delta) noexcept
	{
		v0_ += delta;
		v1_ += delta;
	}

	void GuiRectF::SetLeft(float newLeft) noexcept
	{
		v0_.x = newLeft;
		SwapXIfNeeded();
	}

	void GuiRectF::SetTop(float newTop) noexcept
	{
		v0_.y = newTop;
		SwapYIfNeeded();
	}

	void GuiRectF::SetRight(float newRight) noexcept
	{
		v1_.x = newRight;
		SwapXIfNeeded();
	}

	void GuiRectF::SetBot(float newBot) noexcept
	{
		v1_.y = newBot;
		SwapYIfNeeded();
	}

	void GuiRectF::SetTopLeft(Vec2 const& newTopLeft) noexcept
	{
		v0_ = newTopLeft;
		SwapAnyIfNeeded();
	}

	void GuiRectF::SetTopRight(Vec2 const& newTopRight) noexcept
	{
		v1_.x = newTopRight.x;
		v0_.y = newTopRight.y;
		SwapAnyIfNeeded();
	}

	void GuiRectF::SetBotLeft(Vec2 const& newBotLeft) noexcept
	{
		v0_.x = newBotLeft.x;
		v1_.y = newBotLeft.y;
		SwapAnyIfNeeded();
	}

	void GuiRectF::SetBotRight(Vec2 const& newBotRight) noexcept
	{
		v1_ = newBotRight;
		SwapAnyIfNeeded();
	}

	void GuiRectF::SetCenter(Vec2 const& newCenter) noexcept
	{
		v1_ = 2.f * (newCenter - v0_);
		SwapAnyIfNeeded();
	}

	void GuiRectF::RelLeft(float newLeft) noexcept
	{
		auto const xDel{v1_.x - v0_.x};
		v0_.x = newLeft;
		v1_.x = v0_.x + xDel;
	}

	void GuiRectF::RelTop(float newTop) noexcept
	{
		auto const yDel{v1_.y - v0_.y};
		v0_.y = newTop;
		v1_.y = v0_.y + yDel;
	}

	void GuiRectF::RelRight(float newRight) noexcept
	{
		auto const xDel{v1_.x - v0_.x};
		v1_.x = newRight;
		v0_.x = v1_.x - xDel;
	}

	void GuiRectF::RelBot(float newBot) noexcept
	{
		auto const yDel{v1_.y - v0_.y};
		v1_.y = newBot;
		v0_.y = v1_.y - yDel;
	}

	void GuiRectF::RelTopLeft(Vec2 const& newTopLeft) noexcept
	{
		auto const del{v1_ - v0_};
		v0_ = newTopLeft;
		v1_ = v0_ + del;
	}

	void GuiRectF::RelTopRight(Vec2 const& newTopRight) noexcept
	{
		auto const del{v1_ - v0_};
		v1_.x = newTopRight.x;
		v0_.x = v1_.x - del.x;
		v0_.y = newTopRight.y;
		v1_.y = v0_.y + del.y;
	}

	void GuiRectF::RelBotLeft(Vec2 const& newBotLeft) noexcept
	{
		auto const del{v1_ - v0_};
		v0_.x = newBotLeft.x;
		v1_.x = v0_.x + del.x;
		v1_.y = newBotLeft.y;
		v0_.y = v1_.y - del.y;
	}

	void GuiRectF::RelBotRight(Vec2 const& newBotRight) noexcept
	{
		auto const del{v1_ - v0_};
		v1_ = newBotRight;
		v0_ = v1_ - del;
	}

	void GuiRectF::RelCenter(Vec2 const& newCenter) noexcept
	{
		auto const halfDel{0.5f * (v1_ - v0_)};
		v0_ = newCenter - halfDel;
		v1_ = newCenter + halfDel;
	}

	void GuiRectF::SwapXIfNeeded() noexcept
	{
		if (v0_.x > v1_.x)
		{
			std::swap(v0_.x, v1_.x);
		}
	}

	void GuiRectF::SwapYIfNeeded() noexcept
	{
		if (v0_.y > v1_.y)
		{
			std::swap(v0_.y, v1_.y);
		}
	}

	void GuiRectF::SwapAnyIfNeeded() noexcept
	{
		SwapXIfNeeded();
		SwapYIfNeeded();
	}

	float GuiRectF::GetWidth() const noexcept
	{ return v1_.x - v0_.x; }

	float GuiRectF::GetHeight() const noexcept
	{ return v1_.y - v0_.y; }

	float GuiRectF::GetDiagMag() const noexcept
	{
		auto const w{GetWidth()};
		auto const h{GetHeight()};
		return std::sqrt(w * w + h * h);
	}

	float GuiRectF::GetArea() const noexcept
	{
		auto const w{GetWidth()};
		auto const h{GetHeight()};
		return w * w + h * h;
	}

	float GuiRectF::GetPerimeter() const noexcept
	{ return 2 * (GetWidth() + GetHeight()); }

	float GuiRectF::GetLeft() const noexcept
	{ return v0_.x; }

	float GuiRectF::GetRight() const noexcept
	{ return v1_.x; }

	float GuiRectF::GetTop() const noexcept
	{ return v0_.y; }

	float GuiRectF::GetBot() const noexcept
	{ return v1_.y; }

	Vec2 GuiRectF::GetTopLeft() const noexcept
	{ return v0_; }

	Vec2 GuiRectF::GetTopRight() const noexcept
	{ return {v1_.x, v0_.y}; }

	Vec2 GuiRectF::GetBotLeft() const noexcept
	{ return {v0_.x, v1_.y}; }

	Vec2 GuiRectF::GetBotRight() const noexcept
	{ return v1_; }

	Vec2 GuiRectF::GetCenter() const noexcept
	{ return 0.5f * (v1_ + v0_); }
}