#pragma once

#include "ImplUtil.h"

#include <string>
#include <string_view>

#include <wincodec.h>
#include <d2d1.h>

namespace ArEngine2D {
	class Sprite
	{ 
	private:
		using self = Sprite;

	private:
		inline static Details::Ptr<IWICImagingFactory> s_pImagingFactory_{};
		inline static Details::Ptr<ID2D1RenderTarget> s_pRenderTarget_{};
	public:

		Sprite() = default; // can't (and shouldn't) do loading in the constructor.

		// sprites copied or moved from must be initialized.
		Sprite(self const& that);
		Sprite(self&& that) noexcept;

	public:

		void Initialize(std::wstring_view fileName);
		bool IsInitialized() const noexcept;
		void CopyFromMemory(void const* pData, D2D1_RECT_U const& whereTo, std::size_t pitch);
		void CopyFromSprite(Sprite const& that, D2D1_RECT_U const& from, D2D1_POINT_2U const& whereTo);
		void CopyFromSprite(Sprite const& that);
		void MoveFromSprite(Sprite& that) noexcept;

	public:

		static void InternalInitialization(Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget);

	public:

		D2D1_SIZE_F Size() const;
		float Width() const;
		float Height() const;
		ID2D1Bitmap* D2DPtr() const;
		D2D1_PIXEL_FORMAT PixelFormat() const;
		D2D1_SIZE_U PixelSize() const;
		D2D1_RECT_U RectU() const;
		D2D1_RECT_F RectF() const;

	private:

		void InitializeMinorMembers();
		void IntializationCheck() const noexcept;

	private:
		Details::Ptr<ID2D1Bitmap> pImage_;
		float width_;
		float height_;
	};
}

