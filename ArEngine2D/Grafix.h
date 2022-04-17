#pragma once

#include "ISingle.h"
#include "ImplUtil.h"
#include "ColorF.h"
#include "Vec2.h"
#include "IEngineError.h"
#include "Sprite.h"
#include "Transform.h"

#include <dwrite.h>
#include <d2d1.h>

#include <wincodec.h>
#include <string>

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
		// what u get with transformations other than the identity is unspecified.
		void DrawRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color, float thick = 1.f) noexcept;
		void FillRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color) noexcept;

		void DrawTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color, float thick = 1.f);
		void FillTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color);

		void DrawPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color, float thick = 1.f);
		void FillPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color);

		void DrawArrow(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick = 1.f);

		void DrawString(Vec2 const& loc, std::string str, ColorF const& color, float size);

		void DrawSprite(Vec2 const& loc, Sprite const& sprite, float opacity = 1.f);
		// what u get with transformations other than the identity is unspecified.
		void DrawSpriteCenter(Vec2 const& loc, Sprite const& sprite, float opacity = 1.f);
		void DrawSpriteRect(Vec2 const& loc, Sprite const& sprite, D2D1_RECT_F rect, float opacity = 1.f);
		void DrawSpriteSheet(Vec2 const& loc, SpriteSheet const& sheet, std::uint32_t frameNumber, float opacity = 1.f);
		void DrawAnimationSpriteSheet(Vec2 const& loc, AnimationSpriteSheet const& sheet, float opacity = 1.f);

	public:

		/**
		 * @brief this will push a transform (multiply a matrix) to the current 
		 *		  transforms pushed before. calling Reset transform undoes all 
		 *		  all of them.
		 * @param newTransform => the new transform to be pushed.
		*/
		void PushTransform(Transform const& newTransform) noexcept;

		/**
		 * @brief sets the transform back to the identity.
		*/
		void ResetTransform() noexcept;

		/**
		 * @param newMode the new interpolation mode used for sprites.
		*/
		void SetInterpolationMode(InterpolationMode newMode);

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

		// called before every render target draw call. 
		// this should probably take a callable as a paramter but, 
		// I really hate templates.
		void BeginTransform() noexcept;
		void BeginTransform(Transform const& whatToAppend) noexcept;
		// called after every render target draw call.
		void EndTransform() noexcept;
		// will be optimized away.
		bool IsInitialized() const noexcept;

	private:
		// actual width of the screen
		float width_;

		// actual height of the screen
		float height_;

		Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget_;
		Details::Ptr<ID2D1Factory> pFactory_;

		// simple shapes
		Details::Ptr<ID2D1SolidColorBrush> pSolidBrush_;

		// for drawing strings (very inefficient!)
		Details::Ptr<IDWriteFactory> pDWriteFactory_{};

		// for sprites
		D2D1_BITMAP_INTERPOLATION_MODE interpolationMode_{D2D1_BITMAP_INTERPOLATION_MODE_LINEAR};

		// extra transformations which can be used for drawing very specific
		// things in the scene. saves me from added 10 billion more overloads 
		// to the drawing rootines.
		Transform pushedTransform_{};
	};
}
