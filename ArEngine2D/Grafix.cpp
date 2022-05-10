#include "Grafix.h"

#include "Camera.h"

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

		// width and height are not used like... ever.
		this->width_  = static_cast<float>(clientWidth);
		this->height_ = static_cast<float>(clientHeight);

		HANDLE_GRAPHICS_ERROR(pFactory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(clientWidth, clientHeight)),
			pRenderTarget_.GetAddressOf()
		));

		HANDLE_GRAPHICS_ERROR(pRenderTarget_->CreateSolidColorBrush(
			{}, pSolidBrush_.GetAddressOf()
		));

		HANDLE_GRAPHICS_ERROR(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory_), &pDWriteFactory_
		));

		// so the program does not have to check for empty stack for every call to PopTransform.
		PushTransform({});
		
		Sprite::InternalInitialization(pRenderTarget_);
		Camera::InternalInitialization(&width_, &height_);
	}
	void Grafix::BeginDraw()
	{
		pRenderTarget_->BeginDraw();
	}
	void Grafix::EndDraw()
	{
		HANDLE_ENDDRAW_ERROR(pRenderTarget_.Get());
	}
	void Grafix::ClearScreen(ColorF const& color) noexcept
	{
		pRenderTarget_->Clear(color.ToD2DColor());
	}
	void Grafix::DrawLine(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		BeginTransform();
		pRenderTarget_->DrawLine(from.ToD2DPoint(), to.ToD2DPoint(), pSolidBrush_.Get(), thick);
		EndTransform();
	}
	void Grafix::DrawEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color, float thick) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const elli{D2D1::Ellipse(loc.ToD2DPoint(), rx, ry)};
		BeginTransform();
		pRenderTarget_->DrawEllipse(elli, pSolidBrush_.Get(), thick);
		EndTransform();
	}
	void Grafix::FillEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const elli{D2D1::Ellipse(loc.ToD2DPoint(), rx, ry)};
		BeginTransform();
		pRenderTarget_->FillEllipse(elli, pSolidBrush_.Get());
		EndTransform();
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
		BeginTransform();
		pRenderTarget_->DrawRectangle(rect, pSolidBrush_.Get(), thick);
		EndTransform();
	}
	void Grafix::FillRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color) noexcept
	{
		pSolidBrush_->SetColor(color.ToD2DColor());
		auto const rect{D2D1::RectF(topLeft.x, topLeft.y, botRight.x, botRight.y)};
		BeginTransform();
		pRenderTarget_->FillRectangle(rect, pSolidBrush_.Get());
		EndTransform();
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
		BeginTransform(D2D1::Matrix3x2F::Translation(loc.x, loc.y));
		pRenderTarget_->DrawGeometry(pGeometry.Get(), pSolidBrush_.Get(), thick);
		EndTransform();
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
		BeginTransform(D2D1::Matrix3x2F::Translation(loc.x, loc.y));
		pRenderTarget_->FillGeometry(pGeometry.Get(), pSolidBrush_.Get());
		EndTransform();
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
		BeginTransform(D2D1::Matrix3x2F::Translation(loc.x, loc.y));
		pRenderTarget_->DrawGeometry(pGeometry.Get(), pSolidBrush_.Get(), thick);
		EndTransform();
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
		BeginTransform(D2D1::Matrix3x2F::Translation(loc.x, loc.y));
		pRenderTarget_->FillGeometry(pGeometry.Get(), pSolidBrush_.Get());
		EndTransform();
	}
	void Grafix::DrawArrow(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick)
	{
		constexpr auto MyMin{10.f};
		constexpr auto MyRatio{0.05f};

		auto const vNorm{Vec2::Subtract(to, from).Normalized()};
		auto const vEast{vNorm.ClockwiseNormal()};
		auto const vWest{-vEast};
		auto const vSouthWest{(vWest - vNorm).Normalized()};
		auto const vSouthEast{(vEast - vNorm).Normalized()};
		auto const dist{Vec2::Dist(from, to)};
		auto const legSize{std::max(MyRatio * dist, MyMin)};
		
		DrawLine(from, to, color, thick);
		DrawLine(to, to + vSouthEast * legSize, color, thick);
		DrawLine(to, to + vSouthWest * legSize, color, thick);
	}
	void Grafix::DrawString(Vec2 const& loc, std::string str, ColorF const& color, float size)
	{
		pSolidBrush_->SetColor(color);

		Details::Ptr<IDWriteTextFormat> pFormat{ };
		HANDLE_GRAPHICS_ERROR(pDWriteFactory_->CreateTextFormat(
			L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, size, L"", &pFormat
		));

		D2D1_RECT_F const rect{
			loc.x,
			loc.y,
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity()
		};
		std::wstring const wstr{str.begin(), str.end()};

		BeginTransform();
		pRenderTarget_->DrawText(wstr.c_str(), static_cast<UINT32>(std::size(wstr)),
			pFormat.Get(), rect, pSolidBrush_.Get());
		EndTransform();
	}
	void Grafix::DrawSprite(Vec2 const& loc, Sprite const& sprite, float opacity, Transform const& tr)
	{
		DrawSpriteRect(loc, sprite, sprite.RectF(), opacity, tr);
	}
	void Grafix::DrawSpriteCenter(Vec2 const& loc, Sprite const& sprite, float opacity, Transform const& tr)
	{
		auto const [w, h] {sprite.Coord()};
		DrawSprite({loc.x - w * 0.5f, loc.y - h * 0.5f}, sprite, opacity, tr);
		// D2D1_RECT_F const destRect{0.f, 0.f, w, h};
		// BeginTransform(D2D1::Matrix3x2F::Translation(loc.x - w * 0.5f, loc.y - h * 0.5f));
		// pRenderTarget_->DrawBitmap(sprite.D2DPtr().Get(), destRect, opacity, interpolationMode_);
		// EndTransform();
	}
	void Grafix::DrawSpriteRect(Vec2 const& loc, Sprite const& sprite, D2D1_RECT_F rect, float opacity, Transform const& tr)
	{
		D2D1_RECT_F const destRect{
			loc.x, loc.y, loc.x + (rect.right - rect.left), loc.y + (rect.bottom - rect.top)
		}; 
		BeginTransform(tr);
		pRenderTarget_->DrawBitmap(sprite.D2DPtr().Get(), destRect, opacity, interpolationMode_, rect);
		EndTransform();
	}
	void Grafix::DrawSpriteSheet(Vec2 const& loc, SpriteSheet const& sheet, std::uint32_t frameNumber, float opacity, Transform const& tr)
	{
		DrawSpriteRect(loc, sheet, sheet.FrameRectF(frameNumber), opacity, tr);
	}
	void Grafix::DrawAnimationSpriteSheet(Vec2 const& loc, AnimationSpriteSheet const& sheet, float opacity, Transform const& tr)
	{
		DrawSpriteRect(loc, sheet, sheet.FrameRectF(sheet.CurrFrame()), opacity, tr);
	}
	void Grafix::PushTransform(Transform const& newTransform)
	{
		tranDoubleStack_.push({});
		tranDoubleStack_.top().push(pushedTransform_);
		pushedTransform_.Append(newTransform);
	}
	void Grafix::PopTransform()
	{
		assert(not tranDoubleStack_.empty() && "Tried to pop an empty transform stack");
		tranDoubleStack_.pop();
		pushedTransform_ = tranDoubleStack_.top().top();
	}
	void Grafix::AppendTransform(Transform const& what) noexcept
	{
		tranDoubleStack_.top().push(pushedTransform_);
		pushedTransform_.Append(what);
	}
	void Grafix::UndoTransform() noexcept
	{
		pushedTransform_ = tranDoubleStack_.top().top();
		tranDoubleStack_.top().pop();
	}
	Transform const& Grafix::GetFullTransform() const noexcept
	{
		return pushedTransform_;
	}
	void Grafix::ResetTransform() noexcept
	{
		for (size_t i{}, lim{tranDoubleStack_.size()}; i < lim; ++i)
		{
			tranDoubleStack_.pop();
		}
		pushedTransform_.Reset();
	}
	void Grafix::SetInterpolationMode(InterpolationMode newMode)  
	{
		switch (newMode) 
		{
		case InterpolationMode::Linear: 
			interpolationMode_ = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			break;
		case InterpolationMode::NearestNeighbor: 
			interpolationMode_ = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
			break;
		default:
			throw EngineError{"Invalid InterpolationMode passed to Grafix::SetInterpolationMode"};
		}
	}
	void Grafix::BeginTransform() noexcept
	{
		pRenderTarget_->SetTransform(pushedTransform_.Matrix());
	}
	void Grafix::BeginTransform(Transform const& whatToAppend) noexcept
	{
		pRenderTarget_->SetTransform((whatToAppend >> pushedTransform_).Matrix());
	}
	void Grafix::EndTransform() noexcept
	{
		// this assumes that some drawing rootines do not use BeginTransform,
		// even tho that's not the case at the moment but, I don't want to lose
		// my mind when I add something later, and forget this function even exists.
		pRenderTarget_->SetTransform(D2D1::IdentityMatrix());
	}
	bool Grafix::IsInitialized() const noexcept
	{
		return pRenderTarget_;
	}
}
