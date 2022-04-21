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
#include <stack>

namespace ArEngine2D {
	class Grafix : Details::ISingle
	{ 
	private:
		using self = Grafix;

	public:

		Grafix() = default;

	public:

		/**
		 * @brief users are not meant to call this function.
		*/
		void Initialize(HWND windowHandle);

		/**
		 * @brief users are not meant to call this function.
		*/
		void BeginDraw();

		/**
		 * @brief users are not meant to call this function.
		*/
		void EndDraw();

		/**
		 * @brief fills the screen with the specified color.
		 * @param color => the color to fill the screen with (defaults to black).
		*/
		void ClearScreen(ColorF const& color = {});

		/**
		 * @param from => the first end to the line.
		 * @param to => the second end to the line.
		 * @param color => the color of the line.
		 * @param thick => the thickness of the line.
		*/
		void DrawLine(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick = 1.f) noexcept;

		/**
		 * @param loc => the location of the center of the ellipse.
		 * @param rx => the x radius.
		 * @param ry => the y radius.
		 * @param color => the color of the ellipse.
		 * @param thick => the thickness of the line.
		*/
		void DrawEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color, float thick = 1.f) noexcept;
		/**
		 * @param loc => the location of the center of the ellipse.
		 * @param rx => the x radius.
		 * @param ry => the y radius.
		 * @param color => the color of the ellipse.
		*/
		void FillEllipse(Vec2 const& loc, float rx, float ry, ColorF const& color) noexcept;

		/**
		 * @param loc => the location of the center of the ellipse.
		 * @param r => the radius of the circle.
		 * @param color => the color of the circle.
		 * @param thick => the thickness of the line.
		*/
		void DrawCircle(Vec2 const& loc, float r, ColorF const& color, float thick = 1.f) noexcept;
		/**
		 * @param loc => the location of the center of the ellipse.
		 * @param r => the radius of the circle.
		 * @param color => the color of the circle.
		*/
		void FillCircle(Vec2 const& loc, float r, ColorF const& color) noexcept;

		/**
		 * @param topLeft => this will be interpeted as the top-left point.
		 * @param botRight => this will be interpeted as the bottom-right point.
		 * @param color => the color of the rectangle.
		 * @param thick => the thickness of the line.
		*/
		void DrawRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color, float thick = 1.f) noexcept;
		/**
		 * @param topLeft => this will be interpeted as the top-left point.
		 * @param botRight => this will be interpeted as the bottom-right point.
		 * @param color => the color of the rectangle.
		*/
		void FillRectangle(Vec2 const& topLeft, Vec2 const& botRight, ColorF const& color) noexcept;
		/**
		 * @param loc => the top left of the rectangle.
		 * @param w => the width.
		 * @param h => the height.
		 * @param color => the color of the rectangle.
		 * @param thick => the thickness of the line.
		*/
		void DrawRectangle(Vec2 const& loc, float w, float h, ColorF const& color, float thick = 1.f) noexcept;
		/**
		 * @param loc => the top left of the rectangle.
		 * @param w => the width.
		 * @param h => the height.
		 * @param color => the color of the rectangle.
		*/
		void FillRectangle(Vec2 const& loc, float w, float h, ColorF const& color) noexcept;
		/**
		 * @brief what u get with transformations other than the identity is unspecified.
		 * @param loc => the center of the rectangle.
		 * @param w => the width.
		 * @param h => the height.
		 * @param color => the color of the rectangle.
		 * @param thick => the thickness of the line.
		*/
		void DrawRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color, float thick = 1.f) noexcept;
		/**
		 * @brief what u get with transformations other than the identity is unspecified.
		 * @param loc => the center of the rectangle.
		 * @param w => the width.
		 * @param h => the height.
		 * @param color => the color of the rectangle.
		*/
		void FillRectangleCenter(Vec2 const& loc, float w, float h, ColorF const& color) noexcept;

		/**
		 * @param loc => the location of the triangle.
		 * @param p0 => the first vertex relative to the location.
		 * @param p1 => the second vertex relative to the location.
		 * @param p2 => the third vertex relative to the location.
		 * @param color => the color of the triangle.
		 * @param thick => the thickness of the line.
		*/
		void DrawTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color, float thick = 1.f);
		/**
		 * @param loc => the location of the triangle.
		 * @param p0 => the first vertex relative to the location.
		 * @param p1 => the second vertex relative to the location.
		 * @param p2 => the third vertex relative to the location.
		 * @param color => the color of the triangle.
		*/
		void FillTriangle(Vec2 const& loc, Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, ColorF const& color);

		/**
		 * @param loc => the location of the polygon.
		 * @param vertices => the vertex locations relative to the location.
		 * @param color => the color of the polygon.
		 * @param thick => the thickness of the line.
		*/
		void DrawPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color, float thick = 1.f);
		/**
		 * @param loc => the location of the polygon.
		 * @param vertices => the vertex locations relative to the location.
		 * @param color => the color of the polygon.
		*/
		void FillPolygon(Vec2 const& loc, std::vector<Vec2> const& vertices, ColorF const& color);

		/**
		 * @param from => the tail of the arrow.
		 * @param to => the point the arrow will point to.
		 * @param color => the color of the arrow.
		 * @param thick => the thickness of the line.
		*/
		void DrawArrow(Vec2 const& from, Vec2 const& to, ColorF const& color, float thick = 1.f);

		/**
		 * @brief this function is only meant for debugging purposes, it's very slow.
		 * @param loc => the top left of the string.
		 * @param str => the text to be drawn.
		 * @param color => the color of text.
		 * @param size => the size of the string in device independant pixels.
		*/
		void DrawString(Vec2 const& loc, std::string str, ColorF const& color, float size);

		/**
		 * @param loc => the top left of the sprite.
		 * @param sprite => the sprite to be drawn.
		 * @param opacity => the opacity the sprite will be drawn in.
		*/
		void DrawSprite(Vec2 const& loc, Sprite const& sprite, float opacity = 1.f, Transform const& tr = {});
		/**
		 * @brief what u get with transformations other than the identity is unspecified.
		 * @param loc => the center of the sprite.
		 * @param sprite => the sprite to be drawn.
		 * @param opacity => the opacity the sprite will be drawn in.
		*/
		void DrawSpriteCenter(Vec2 const& loc, Sprite const& sprite, float opacity = 1.f, Transform const& tr = {});
		/**
		 * @param loc => the top left of the sprite.
		 * @param sprite => the sprite to be drawn.
		 * @param rect => the part that will drawn.
		 * @param opacity => the opacity the sprite will be drawn in.
		*/
		void DrawSpriteRect(Vec2 const& loc, Sprite const& sprite, D2D1_RECT_F rect, float opacity = 1.f, Transform const& tr = {});
		/**
		 * @brief this will draw one frame at a time.
		 * @param loc => the top left of the frame.
		 * @param sheet => the sprite sheet.
		 * @param frameNumber => the frame id.
		 * @param opacity => the opacity the frame will be drawn in.
		*/
		void DrawSpriteSheet(Vec2 const& loc, SpriteSheet const& sheet, std::uint32_t frameNumber, float opacity = 1.f, Transform const& tr = {});
		/**
		 * @brief this will draw the correct frame.
		 * @param loc => the top left of the frame.
		 * @param sheet => the animation sprite sheet.
		 * @param opacity => the opacity the frame will be drawn in.
		*/
		void DrawAnimationSpriteSheet(Vec2 const& loc, AnimationSpriteSheet const& sheet, float opacity = 1.f, Transform const& tr = {});

	public:

		void PushTransform(Transform const& newTransform);
		void PopTransform();
		void AppendTransform(Transform const& what) noexcept;
		Transform const& GetFullTransform() const noexcept;
		Transform const& GetLastTransform() const noexcept;

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
		Transform pushedTransform_;
		std::stack<Transform> tranStack_;
	};
}
