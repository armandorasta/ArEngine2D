#include "Sprite.h"

#include "IEngineError.h"

namespace ArEngine2D {
	Sprite::Sprite(self const& that)
	{
		CopyFromSprite(that);
	}
	Sprite::Sprite(self&& that) noexcept
	{
		MoveFromSprite(that);
	}
	void Sprite::Initialize(std::wstring_view fileName)
	{
		// Safety checks so, I don't get confused with a nullptr runtime exception
		assert(s_pRenderTarget_ && "Did not call InternalInitialization");
		assert(not IsInitialized() && "Double initialization of Sprite");

		// Cheap to construct so, no need to pollute the class with static data members
		Details::Ptr<IWICBitmapDecoder> pDecoder{};
		Details::Ptr<IWICBitmapFrameDecode> pOnlyFrame{};
		Details::Ptr<IWICFormatConverter> pConverter{};

		// Create the decoder
		HANDLE_GRAPHICS_ERROR(s_pImagingFactory_->CreateDecoderFromFilename(
			fileName.data(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder
		));
		// Get the first and only frame, which the converter will later encapsulate
		HANDLE_GRAPHICS_ERROR(pDecoder->GetFrame(0U, &pOnlyFrame));
		// Create the format converter to be initialized in the next line
		HANDLE_GRAPHICS_ERROR(s_pImagingFactory_->CreateFormatConverter(&pConverter));
		// Initialize the converter using the frame and some other microsoft nonsense
		HANDLE_GRAPHICS_ERROR(pConverter->Initialize(
			pOnlyFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom
		));
		// Use the converter through the render target
		HANDLE_GRAPHICS_ERROR(s_pRenderTarget_->CreateBitmapFromWicBitmap(
			pConverter.Get(), nullptr, &pImage_
		));
		// minor members are there for performance purposes.
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
		assert(that.IsInitialized() && "Tried to copy from an uninitialized Sprite");
		CopyFromSprite(that, that.RectU(), {});
		this->width_ = that.width_;
		this->height_ = that.height_;
	}
	void Sprite::MoveFromSprite(Sprite& that) noexcept
	{
		assert(that.IsInitialized() && "Tried to move from an uninitialized Sprite");
		pImage_ = std::move(that.pImage_);
		this->width_ = that.width_;
		this->height_ = that.height_;
	}
	void Sprite::InternalInitialization(Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget)
	{
		assert(not s_pRenderTarget_ && "Double internal initialization of Sprite");
		// So the user does not have to pass gfx to every Sprite.
		s_pRenderTarget_ = std::move(pRenderTarget);
		// Commenting this out will make the program crash on exit for some reason.
		HANDLE_GRAPHICS_ERROR(CoInitialize(nullptr));
		HANDLE_GRAPHICS_ERROR(CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(s_pImagingFactory_.Get()), &s_pImagingFactory_
		));
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
		return {0U, 0U, static_cast<UINT32>(Width()), static_cast<UINT32>(Height())};
	}
	D2D1_RECT_F Sprite::RectF() const
	{
		IntializationCheck();
		return {0.f, 0.f, Width(), Height()};
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