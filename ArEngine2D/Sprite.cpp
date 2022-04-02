#include "Sprite.h"

#include "IEngineError.h"

namespace ArEngine2D {
	Sprite::Sprite(self const&)
	{
	}
	void Sprite::Initialize(std::wstring_view fileName)
	{
		assert(not IsInitialized() && "Double initialization of Sprite");
		Details::Ptr<IWICBitmapDecoder> pDecoder{};
		Details::Ptr<IWICBitmapFrameDecode> pFrameDecode{};

		HANDLE_GRAPHICS_ERROR(s_pImagingFactory_->CreateDecoderFromFilename(
			fileName.data(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder
		));
		HANDLE_GRAPHICS_ERROR(pDecoder->GetFrame(0U, &pFrameDecode));
		HANDLE_GRAPHICS_ERROR(s_pConverter_->Initialize(
			pFrameDecode.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom
		));
		HANDLE_GRAPHICS_ERROR(s_pRenderTarget_->CreateBitmapFromWicBitmap(
			s_pConverter_.Get(), nullptr, &pImage_
		));
		InitializeMinorMembers();
	}
	void Sprite::CopyFromMemory(void const* pData, D2D1_RECT_U const& whereTo, std::size_t pitch)
	{
		IntializationCheck();
		pImage_->CopyFromMemory(&whereTo, pData, static_cast<UINT32>(pitch));
	}
	void Sprite::CopyFromSprite(Sprite const& that, D2D1_RECT_U const& from, D2D1_POINT_2U const& whereTo)
	{
		IntializationCheck();
		pImage_->CopyFromBitmap(&whereTo, that.D2DPtr(), &from);
	}
	void Sprite::CopyFromSprite(Sprite const& that)
	{
		assert(that.IsInitialized() && "Tried to copy from uninitialized Sprite");
		CopyFromSprite(that, that.RectU(), {});
		InitializeMinorMembers();
	}
	void Sprite::MoveFromSprite(Sprite& that)
	{
		assert(that.IsInitialized() && "Tried to move from uninitialized Sprite");
		pImage_ = std::move(that.pImage_);
		InitializeMinorMembers();
	}
	void Sprite::InternalInitialization(Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget)
	{
		assert(not s_pRenderTarget_ && "Double Internal initialization of Sprite");
		s_pRenderTarget_ = std::move(pRenderTarget);
		HANDLE_GRAPHICS_ERROR(CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), &s_pImagingFactory_
		));
		HANDLE_GRAPHICS_ERROR(s_pImagingFactory_->CreateFormatConverter(&s_pConverter_));
	}
	D2D1_SIZE_F Sprite::Size() const
	{
		IntializationCheck();
		return {Width(), Height()};
	}
	float Sprite::Width() const
	{
		IntializationCheck();
		return width_;
	}
	float Sprite::Height() const
	{
		IntializationCheck();
		return height_;
	}
	ID2D1Bitmap* Sprite::D2DPtr() const
	{
		IntializationCheck();
		return pImage_.Get();
	}
	D2D1_PIXEL_FORMAT Sprite::PixelFormat() const
	{
		IntializationCheck();
		return pImage_->GetPixelFormat();
	}
	D2D1_SIZE_U Sprite::PixelSize() const
	{
		IntializationCheck();
		return pImage_->GetPixelSize();
	}
	D2D1_RECT_U Sprite::RectU() const
	{
		IntializationCheck();
		return {0U, 0U, static_cast<UINT32>(width_), static_cast<UINT32>(height_)};
	}
	D2D1_RECT_F Sprite::RectF() const
	{
		IntializationCheck();
		return {0.f, 0.f, width_, height_};
	}
	void Sprite::InitializeMinorMembers()
	{
		auto const size{pImage_->GetSize()};
		width_ = size.width;
		height_ = size.height;
	}
	bool Sprite::IsInitialized() const noexcept
	{
		return pImage_;
	}
	void Sprite::IntializationCheck() const noexcept
	{
		assert(IsInitialized() && "Use of uninitialized Sprite");
	}
}