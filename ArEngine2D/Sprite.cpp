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
	Sprite::self& Sprite::operator=(self const& rhs)
	{
		CopyFromSprite(rhs);
		return *this;
	}
	Sprite::self& Sprite::operator=(self&& rhs) noexcept
	{
		MoveFromSprite(rhs);
		return *this;
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
		InitializationCheck();
		pImage_->CopyFromMemory(&whereTo, pData, static_cast<UINT32>(pitch));

		// size may have changed.
		auto const size{pImage_->GetSize()};
		width_ = size.width;
		height_ = size.height;
	}
	void Sprite::CopyFromSpriteRect(Sprite const& that, D2D1_RECT_U const& from, D2D1_POINT_2U const& whereTo)
	{
		InitializationCheck();
		pImage_->CopyFromBitmap(&whereTo, that.pImage_.Get(), &from);
	}
	void Sprite::CopyFromSprite(Sprite const& that)
	{
		assert(that.IsInitialized() && "Tried to copy from an uninitialized Sprite");
		CopyFromSpriteRect(that, that.RectU(), {});
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
		InitializationCheck();
		return {Width(), Height()};
	}
	float Sprite::Width() const
	{
		InitializationCheck();
		return width_;
	}
	float Sprite::Height() const
	{
		InitializationCheck();
		return height_;
	}
	Details::Ptr<ID2D1Bitmap> Sprite::D2DPtr() const
	{
		InitializationCheck();
		return pImage_;
	}
	D2D1_PIXEL_FORMAT Sprite::PixelFormat() const
	{
		InitializationCheck();
		return pImage_->GetPixelFormat();
	}
	D2D1_SIZE_U Sprite::PixelSize() const
	{
		InitializationCheck();
		return pImage_->GetPixelSize();
	}
	D2D1_RECT_U Sprite::RectU() const
	{
		InitializationCheck();
		return {0U, 0U, static_cast<UINT32>(Width()), static_cast<UINT32>(Height())};
	}
	D2D1_RECT_F Sprite::RectF() const
	{
		InitializationCheck();
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
	void Sprite::InitializationCheck() const noexcept
	{
		assert(IsInitialized() && "Use of uninitialized Sprite");
	}
	void SpriteSheet::Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount)
	{
		base::Initialize(fileName);
		if (std::fmod(Width(), frameWidth) or
			std::fmod(Height(), frameHeight))
		{
			throw EngineError{"Invalid SpriteSheet dimensions passed; sheet width must be a multiple of frame width"};
		}
		this->frameWidth_  = frameWidth;
		this->frameHeight_ = frameHeight;
		this->frameCount_  = frameCount_;
		InitializeMinorMembers();
	}
	float SpriteSheet::FrameWidth() const
	{
		InitializationCheck();
		return frameWidth_;
	}
	float SpriteSheet::FrameHeight() const
	{
		InitializationCheck();
		return frameHeight_;
	}
	std::uint32_t SpriteSheet::RowFrameCount() const
	{
		InitializationCheck();
		return rowFrameCount_;
	}
	std::uint32_t SpriteSheet::ColFrameCount() const
	{
		InitializationCheck();
		return colFrameCount_;
	}
	std::uint32_t SpriteSheet::FrameCount() const
	{
		InitializationCheck();
		return frameCount_;
	}
	D2D1_SIZE_F SpriteSheet::FrameSize() const
	{
		InitializationCheck();
		return {FrameWidth(), FrameHeight()};
	}
	D2D1_RECT_F SpriteSheet::FrameRectF(std::uint32_t number) const
	{
		InitializationCheck();
		assert(number < (colFrameCount_* rowFrameCount_) && "Invalid frame number");

		auto const left{(number % rowFrameCount_)};
		auto const top{(number / rowFrameCount_)};
		return {left * frameWidth_, top * frameHeight_, (left + 1) * frameWidth_, (top + 1) * frameHeight_};
	}
	D2D1_RECT_U SpriteSheet::FrameRectU(std::uint32_t number) const
	{
		InitializationCheck();
		auto const rectF{FrameRectF(number)};
		return {
			static_cast<UINT32>(rectF.left),
			static_cast<UINT32>(rectF.top),
			static_cast<UINT32>(rectF.right),
			static_cast<UINT32>(rectF.bottom),
		};
	}
	void SpriteSheet::InitializeMinorMembers()
	{
		rowFrameCount_ = static_cast<std::uint32_t>(Width() / FrameWidth());
		colFrameCount_ = static_cast<std::uint32_t>(Height() / FrameHeight());
		if (not frameCount_)
		{
			frameCount_ = rowFrameCount_ * colFrameCount_;
		}
	}
	void AnimationSpriteSheet::Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount,
		std::chrono::duration<float> frameTime)
	{
		base::Initialize(fileName, frameWidth, frameHeight, frameCount);
		this->frameTime_ = frameTime;
	}
	void AnimationSpriteSheet::Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount,
		float frameTimeSeconds)
	{
		using namespace std::chrono_literals;
		Initialize(fileName, frameWidth, frameHeight, frameCount, frameTimeSeconds * 1s);
	}
	void AnimationSpriteSheet::Update(std::chrono::duration<float> dt) noexcept
	{
		InitializationCheck();
		if (bStop_)
		{
			return;
		}
		currTime_ += (1 - 2 * bReverse_) * dt;
		auto const animationTime{frameTime_ * FrameCount()};
		while (currTime_ >= animationTime) // no overflowing!
		{
			currTime_ = {};
		}
		while (currTime_ < std::chrono::duration<float>{0.f}) // or underflowing
		{
			// should not ever get below zero if not reversed.
			assert(bReverse_ && "AnimationSpriteSheet has reac22hed an invalid state");
			currTime_ += animationTime;
		}
	}
	void AnimationSpriteSheet::Update(float dt) noexcept
	{
		using namespace std::chrono_literals;
		Update(dt * 1s);
	}
	void AnimationSpriteSheet::PauseAnimation() noexcept
	{
		InitializationCheck();
		bStop_ = true;
	}
	void AnimationSpriteSheet::ResumeAnimation() noexcept
	{
		InitializationCheck();
		bStop_ = false;
	}
	void AnimationSpriteSheet::ToggleAnimation() noexcept
	{
		InitializationCheck();
		bStop_ ^= 1;
	}
	void AnimationSpriteSheet::ReverseAnimation() noexcept
	{
		InitializationCheck();
		bReverse_ ^= 1;
	}
	void AnimationSpriteSheet::SetFrameTime(std::chrono::duration<float> newTime) noexcept
	{
		InitializationCheck();
		auto const count{newTime.count()};
		assert(count >= 0.f && "Invalid call to AnimationSpriteSheet::SetFrameTime");
		// so the animation does not randomly change frames.
		currTime_ *= newTime.count() / frameTime_.count(); 
		frameTime_ = newTime;
	}
	void AnimationSpriteSheet::SetFrameTime(float newTimeInSeconds) noexcept
	{
		using namespace std::chrono_literals;
		SetFrameTime(newTimeInSeconds * 1s);
	}
	void AnimationSpriteSheet::SetAnimationSpeed(float perncent) noexcept
	{
		SetFrameTime(frameTime_ * perncent);
	}
	void AnimationSpriteSheet::SetCurrFrame(std::uint32_t id) noexcept
	{
		InitializationCheck();
		assert(id < FrameCount() && "Invalid call to AnimationSpriteSheet::SetCurrFrame");
		currTime_ = id * frameTime_;
	}
	std::chrono::duration<float> AnimationSpriteSheet::FrameTime() const noexcept
	{
		InitializationCheck();
		return frameTime_;
	}
	std::chrono::duration<float> AnimationSpriteSheet::CurrTime() const noexcept
	{
		InitializationCheck();
		return currTime_;
	}
	std::uint32_t AnimationSpriteSheet::CurrFrame() const noexcept
	{
		InitializationCheck();
		return static_cast<std::uint32_t>(currTime_.count() / frameTime_.count());
	}
	bool AnimationSpriteSheet::IsAnimating() const noexcept
	{
		InitializationCheck();
		return not bStop_;
	}
	}