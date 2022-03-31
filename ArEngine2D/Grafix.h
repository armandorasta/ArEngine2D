#pragma once

#include "ISingle.h"
#include "ImplUtil.h"
#include "ColorF.h"
#include "Vec2.h"
#include "IEngineError.h"

#include <dwrite.h>
#include <d2d1.h>

namespace ArEngine2D {
	class Grafix : Details::ISingle
	{ 
	private:
		using self = Grafix;

	public:

		Grafix() = default;

	public:

		void Initialize(HWND windowHandle);
		void BeginDraw();
		void EndDraw();
		void ClearScreen(ColorF const& color = {});

		void DrawLine(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick = 1.f) noexcept;

		void DrawEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color, float thick = 1.f) noexcept;
		void FillEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color) noexcept;
		
		void DrawCircle(Vec2 const& loc, float r, ColorF const& color, float thick = 1.f) noexcept;
		void FillCircle(Vec2 const& loc, float r, ColorF const& color) noexcept;

		void DrawRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color, float thick = 1.f) noexcept;
		void FillRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color) noexcept;
		void DrawRectangle(Vec2 const& loc, float w, float h, ColorF const& color, float thick = 1.f) noexcept;
		void FillRectangle(Vec2 const& loc, float w, float h, ColorF const& color) noexcept;
		void DrawRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color, float thick = 1.f) noexcept;
		void FillRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color) noexcept;

		void DrawTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color, float thick = 1.f);
		void FillTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color);

		void DrawPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color, float thick = 1.f);
		void FillPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color);

		void DrawString(std::string str, Vec2 const& loc, ColorF const& color, float size, std::wstring fontName = L"Consolas") 
		{
			pSolidBrush_->SetColor(color);

			Details::Ptr<IDWriteFactory> pDFactory{};
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDFactory), &pDFactory);

			Details::Ptr<IDWriteTextFormat> pFormat{};
			pDFactory->CreateTextFormat(fontName.data(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, size, L"", &pFormat);
			// pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			// pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

			D2D1_RECT_F const rect{loc.x, loc.y, size * 100.f, size * 100.f};
			std::wstring const wstr{str.begin(), str.end()};

			pRenderTarget_->DrawTextW(L"some bullshit", static_cast<UINT32>(std::size(L"some bullshit")), pFormat.Get(), rect, pSolidBrush_.Get());

			/*
			
				TODO: fix this code (the text behaves strangely but, it does show up).
			
			*/
		}

	private:

		template <std::invocable<ID2D1GeometrySink*> Callable>
		auto GenerateGeometry(Vec2 const& beginPoint, D2D1_FIGURE_BEGIN beginFlag, D2D1_FIGURE_END endFlag, Callable&& callable)
		{
			Details::Ptr<ID2D1PathGeometry> pGeometry{};
			Details::Ptr<ID2D1GeometrySink> pSink{};
			HANDLE_GRAPHICS_ERROR(pFactory_->CreatePathGeometry(&pGeometry));
			HANDLE_GRAPHICS_ERROR(pGeometry->Open(&pSink));
			pSink->BeginFigure(beginPoint.ToD2DPoint(), beginFlag);
			callable(pSink.Get());
			pSink->EndFigure(endFlag);
			HANDLE_GRAPHICS_ERROR(pSink->Close());
			return pGeometry;
		}

		bool IsInitialized() const noexcept;

	private:
		float width_;
		float height_;

		Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget_;
		Details::Ptr<ID2D1Factory> pFactory_;

		// simple shapes
		Details::Ptr<ID2D1SolidColorBrush> pSolidBrush_;
	};
}
