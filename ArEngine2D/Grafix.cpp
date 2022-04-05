#include "Grafix.h"

namespace ArEngine2D {
	void Grafix::Initialize(HWND windowHandle)
	{
		assert(not IsInitialized() && "double initialization of Grafix");
		HANDLE_GRAPHICS_ERROR(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory_.GetAddressOf()
		));

		RECT clientRect{};
		GetClientRect(windowHandle, &clientRect);
		auto const clientWidth{static_cast<UINT32>(clientRect.right - clientRect.left)};
		auto const clientHeight{static_cast<UINT32>(clientRect.bottom - clientRect.top)};

		HANDLE_GRAPHICS_ERROR(pFactory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(clientWidth, clientHeight)),
			pRenderTarget_.GetAddressOf()
		));

		HANDLE_GRAPHICS_ERROR(pRenderTarget_->CreateSolidColorBrush(
			{}, pSolidBrush_.GetAddressOf()
		));
		Sprite::InternalInitialization(pRenderTarget_);
	}
	void Grafix::BeginDraw()
	{
		pRenderTarget_->BeginDraw();
	}
	void Grafix::EndDraw()
	{
		HANDLE_ENDDRAW_ERROR(pRenderTarget_.Get());
	}
	void Grafix::ClearScreen(ColorF const& color)
	{
		pRenderTarget_->Clear(color.ToD2DColor());
	}
	void Grafix::DrawLine(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		pRenderTarget_->DrawLine(from.ToD2DPoint(), to.ToD2DPoint(), pSolidBrush_.Get(), thick);
	}
	void Grafix::DrawEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color, float thick) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const elli{D2D1::Ellipse(loc.ToD2DPoint(), rx, ry)};
		pRenderTarget_->DrawEllipse(elli, pSolidBrush_.Get(), thick);
	}
	void Grafix::FillEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const elli{D2D1::Ellipse(loc.ToD2DPoint(), rx, ry)};
		pRenderTarget_->FillEllipse(elli, pSolidBrush_.Get());
	}
	void Grafix::DrawCircle(Vec2 const& loc, float r, ColorF const& color, float thick) noexcept
	{
		DrawEllipse(loc, r, r, color, thick);
	}
	void Grafix::FillCircle(Vec2 const& loc, float r, ColorF const& color) noexcept
	{ 
		FillEllipse(loc, r, r, color);
	}
	void Grafix::DrawRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color, float thick) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const rect{D2D1::RectF(topLeft.x, topLeft.y, botRight.x, botRight.y)};
		pRenderTarget_->DrawRectangle(rect, pSolidBrush_.Get(), thick);
	}
	void Grafix::FillRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const rect{D2D1::RectF(topLeft.x, topLeft.y, botRight.x, botRight.y)};
		pRenderTarget_->FillRectangle(rect, pSolidBrush_.Get());
	}
	void Grafix::DrawRectangle(Vec2 const& loc, float w, float h, ColorF const& color, float thick) noexcept
	{
		DrawRectangle(loc, {loc.x + w, loc.y + h}, color, thick);
	}
	void Grafix::FillRectangle(Vec2 const& loc, float w, float h, ColorF const& color) noexcept
	{
		FillRectangle(loc, {loc.x + w, loc.y + h}, color);
	}
	void Grafix::DrawRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color, float thick) noexcept
	{
		auto const hw{w * 0.5f};
		auto const hh{h * 0.5f};
		DrawRectangle({loc.x - hw, loc.y - hh}, {loc.x + hw, loc.y + hh}, color, thick);
	}
	void Grafix::FillRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color) noexcept
	{
		auto const hw{w * 0.5f};
		auto const hh{h * 0.5f};
		FillRectangle({loc.x - hw, loc.y - hh}, {loc.x + hw, loc.y + hh}, color);
	}
	void Grafix::DrawTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color, float thick)
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const pGeometry{GenerateGeometry(
			p0,
			D2D1_FIGURE_BEGIN_HOLLOW, 
			D2D1_FIGURE_END_CLOSED, 
			[&](auto p) {
				p->AddLine(p1);
				p->AddLine(p2);
			}
		)};
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Translation({loc.x, loc.y}));
		pRenderTarget_->DrawGeometry(pGeometry.Get(), pSolidBrush_.Get(), thick);
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	void Grafix::FillTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color)
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const pGeometry{GenerateGeometry(
			p0,
			D2D1_FIGURE_BEGIN_FILLED,
			D2D1_FIGURE_END_CLOSED,
			[&](auto p) {
				p->AddLine(p1);
				p->AddLine(p2);
			}
		)};
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Translation({loc.x, loc.y}));
		pRenderTarget_->FillGeometry(pGeometry.Get(), pSolidBrush_.Get());
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	void Grafix::DrawPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color, float thick)
	{
		if (vertices.size() <= 1U)
		{
			return;
		}
		else if (vertices.size() == 2U)
		{
			return DrawLine(loc + vertices[0], loc + vertices[1], color, thick);
		}

		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const pGeometry{GenerateGeometry(
			vertices[0],
			D2D1_FIGURE_BEGIN_HOLLOW,
			D2D1_FIGURE_END_CLOSED,
			[&](auto p) {
				for (size_t i{1}, lim{vertices.size()}; i < lim; ++i)
				{
					p->AddLine(vertices[i]);
				}
			}
		)};
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Translation({loc.x, loc.y}));
		pRenderTarget_->DrawGeometry(pGeometry.Get(), pSolidBrush_.Get(), thick);
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	void Grafix::FillPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color)
	{
		if (vertices.size() <= 1U)
		{
			return;
		}
		else if (vertices.size() == 2U)
		{
			return DrawLine(loc + vertices[0], loc + vertices[1], color, 1.f);
		}

		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const pGeometry{GenerateGeometry(
			vertices[0],
			D2D1_FIGURE_BEGIN_FILLED,
			D2D1_FIGURE_END_CLOSED,
			[&](auto p) {
				for (size_t i{1}, lim{vertices.size()}; i < lim; ++i)
				{
					p->AddLine(vertices[i]);
				}
			}
		)};
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Translation({loc.x, loc.y}));
		pRenderTarget_->FillGeometry(pGeometry.Get(), pSolidBrush_.Get());
		pRenderTarget_->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	void Grafix::DrawSprite(Vec2 const& loc, Sprite const& sprite, float opacity, Transform transform)
	{
		D2D1_RECT_F const destRect{
			0.f, 0.f, sprite.Width(), sprite.Height()
		};
		pRenderTarget_->SetTransform(transform.Matrix() * D2D1::Matrix3x2F::Translation(loc.x, loc.y));
		pRenderTarget_->DrawBitmap(sprite.D2DPtr(), destRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
		pRenderTarget_->SetTransform(D2D1::IdentityMatrix());
	}
	bool Grafix::IsInitialized() const noexcept
	{
		return pRenderTarget_;
	}
}
