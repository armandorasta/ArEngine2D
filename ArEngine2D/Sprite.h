#pragma once

#include "ImplUtil.h"

#include <string>
#include <string_view>
#include <chrono>

#include <wincodec.h>
#include <d2d1.h>

namespace ArEngine2D {
	enum class InterpolationMode : std::uint32_t
	{
		Linear,
		NearestNeighbor,
	};

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
		self& operator=(self const& rhs);
		self& operator=(self&& rhs) noexcept;

	public:

		/**
		 * @brief used to initialize the data.
		 * @param fileName => the name of the file containing the data, 
		 *					  does not have to be a bitmap.
		*/
		void Initialize(std::wstring_view fileName);

		/**
		 * @return true if Sprite::Initialize was already called.
		*/
		bool IsInitialized() const noexcept;

		/**
		 * @brief copies data from memory into the current sprite.
		 * @param pData => pointer to the buffer containing the data.
		 * @param whereTo => region inside the current sprite to be replaced.
		 * @param pitch => the width of the data.
		*/
		void CopyFromMemory(void const* pData, D2D1_RECT_U const& whereTo, std::size_t pitch);

		/**
		 * @brief copies data from another sprite.
		 * @param that => the other sprite.
		 * @param from => rectangle inside the other sprite containing the data.
		 * @param whereTo => point inside the current sprite where the data will end up.
		*/
		void CopyFromSpriteRect(Sprite const& that, D2D1_RECT_U const& from, D2D1_POINT_2U const& whereTo);

		/**
		 * @brief same as the copy constructor.
		*/
		void CopyFromSprite(Sprite const& that);

		/**
		 * @brief same as the move constructor.
		*/
		void MoveFromSprite(Sprite& that) noexcept;

	public:

		/**
		 * @brief users may not call this function.
		*/
		static void InternalInitialization(Details::Ptr<ID2D1HwndRenderTarget> pRenderTarget);

	public:

		/**
		 * @return the width and height of the sprite.
		*/
		D2D1_SIZE_F Coord() const;

		/**
		 * @return the width of the sprite.
		*/
		float Width() const;

		/**
		 * @return the height of the sprite.
		*/
		float Height() const;

		/**
		 * @return a pointer to a direct2d bitmap containing the data.
		 *		   users may not release the data through this pointer.
		*/
		Details::Ptr<ID2D1Bitmap> D2DPtr() const;

		/**
		 * @return the pixel format of the sprite.
		*/
		D2D1_PIXEL_FORMAT PixelFormat() const;

		/**
		 * @return the pixel size of the sprite.
		*/
		D2D1_SIZE_U PixelSize() const;

		/**
		 * @return a rectangle with the extact same condinates and size of the sprite.
		*/
		D2D1_RECT_U RectU() const;

		/**
		 * @return a rectangle with the extact same condinates and size of the sprite.
		*/
		D2D1_RECT_F RectF() const;

	private:

		void InitializeMinorMembers();
		
	protected:
		// will be optimized away.
		void InitializationCheck() const noexcept;

	private:
		Details::Ptr<ID2D1Bitmap> pImage_;
		float width_;
		float height_;
	};

	class SpriteSheet : public Sprite
	{
	private:
		using self = SpriteSheet;
		using base = Sprite;

	public:

		SpriteSheet() = default;

		// sprites copied or moved from must be initialized.
		SpriteSheet(self const& that)	     = default;
		SpriteSheet(self&& that) noexcept    = default;
		self& operator=(self const& rhs)     = default;
		self& operator=(self&& rhs) noexcept = default;

	public:

		/**
		 * @brief used to initialize the data.
		*/


		/**
		 * @brief used to initialize the data.
		 * @param fileName => the name of the file containing the data,
		 *					  does not have to be a bitmap.
		 * @param frameWidth => the width of what to be considered a frame in the sprite.
		 * @param frameHeight => the height of what to be considered a frame in the sprite.
		 * @param frameCount => the number of frames in the image. 
		 *						if set to zero the function is gonna figure it out using
		 *						some simple math. use if the image contains more than one 
		 *						animation.
		*/
		void Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount = {});

	public:

		/**
		 * @return the width of a single frame in the sprite.
		*/
		float FrameWidth() const;

		/**
		 * @return the height of a single frame in the sprite.
		*/
		float FrameHeight() const;

		/**
		 * @return the max number of frames can be contained a single row.
		*/
		std::uint32_t RowFrameCount() const;

		/**
		 * @return the max number of frames can be contained a single column.
		*/
		std::uint32_t ColFrameCount() const;

		/**
		 * @return the number of frames in the current sprite.
		*/
		std::uint32_t FrameCount() const;

		/**
		 * @return both width and height of a single frame.
		*/
		D2D1_SIZE_F FrameSize() const;

		/**
		 * @param number => the id of the frame (0 is the first frame).
		 * @return a rectangle with the extact same condinates and size of a frame in the sprite.
		*/
		D2D1_RECT_F FrameRectF(std::uint32_t number) const;

		/**
		 * @param number => the id of the frame (0 is the first frame).
		 * @return a rectangle with the extact same condinates and size of a frame in the sprite.
		*/
		D2D1_RECT_U FrameRectU(std::uint32_t number) const;

	private:
		// initializes optimization related members that may be removed in the future.
		void InitializeMinorMembers();

	private:
		float frameWidth_;
		float frameHeight_;
		std::uint32_t frameCount_;

		// minor members
		std::uint32_t rowFrameCount_;
		std::uint32_t colFrameCount_;
	};

	class AnimationSpriteSheet : public SpriteSheet
	{
	private:
		using self = AnimationSpriteSheet;
		using base = SpriteSheet;

	public:

		// sprites copied or moved from must be initialized.
		AnimationSpriteSheet()						= default;
		AnimationSpriteSheet(self const& that)		= default;
		AnimationSpriteSheet(self && that) noexcept = default;
		self& operator=(self const& rhs)			= default;
		self& operator=(self && rhs)	   noexcept = default;

	public:

		/**
		 * @brief must be the first function to get called using the current instance.
		 * @param fileName => the name of the file containing the data,
		 *					  does not have to be a bitmap.
		 * @param frameWidth => the width of what to be considered a frame in the sprite.
		 * @param frameHeight => the height of what to be considered a frame in the sprite.
		 * @param frameCount => the number of frames in the image. 
		 *						if set to zero the function is gonna figure it out using
		 *						some simple math. use if the image contains more than one 
		 *						animation.
		 * @param frameTime => the amount of time a single frame stays in screen.
		*/
		void Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount, 
			std::chrono::duration<float> frameTime);

		/**
		 * @brief must be the first function to get called using the current instance.
		 * @param fileName => the name of the file containing the data,
		 *					  does not have to be a bitmap.
		 * @param frameWidth => the width of what to be considered a frame in the sprite.
		 * @param frameHeight => the height of what to be considered a frame in the sprite.
		 * @param frameCount => the number of frames in the image.
		 *						if set to zero the function is gonna figure it out using
		 *						some simple math. use if the image contains more than one
		 *						animation.
		 * @param frameTimeSeconds => the amount of time a single frame stays in screen.
		*/
		void Initialize(std::wstring_view fileName, float frameWidth, float frameHeight, std::uint32_t frameCount, 
			float frameTimeSeconds);

		/**
		 * @brief basically adds dt to an internal timer.
		 *		  this function does nothing when the animation is paused.
		 *		  users should not use this function to manipulate animation speed.
		 * @param dt => the amount of time elapsed since the last call.
		*/
		void Update(std::chrono::duration<float> dt) noexcept;

		/**
		 * @brief basically adds dt to an internal timer.
		 * @param dt => the amount of time "elapsed" since the last call.
		*/
		void Update(float dt) noexcept;

		/**
		 * @brief stops the sprite from animating temporarily. 
		 *		  undo with ResumeAnimation.
		*/
		void PauseAnimation() noexcept;

		/**
		 * @brief lets the sprite animate once again. 
		 *		  undo with PauseAnimation.
		*/
		void ResumeAnimation() noexcept;

		/**
		 * @brief alternates between resuming and pausing the animation.
		*/
		void ToggleAnimation() noexcept;

		/**
		 * @brief reverses the animation, undo with another call
		 *		  to this same function. 
		*/
		void ReverseAnimation() noexcept;

	public:

		/**
		 * @brief use to change the speed of the animation.
		 *		  users should not use this function to stop the animation.
		 * @param newTime => the new time a single frame takes on screen.
		*/
		void SetFrameTime(std::chrono::duration<float> newTime) noexcept;

		/**
		 * @brief use to change the speed of the animation.
		 *		  users should not use this function to stop the animation.
		 * @param newTimeInSeconds => the new time a single frame takes on screen.
		*/
		void SetFrameTime(float newTimeInSeconds) noexcept;

		/**
		 * @brief changes the speed of the animation.
		 *		  users should not use this function to stop the animation.
		 * @param perncent => the new speed will be (current_speed * percent).
		*/
		void SetAnimationSpeed(float perncent) noexcept;

		/**
		 * @brief calling this function with an out of bounds id is undefined behaviour.
		 *		  users should not use this function to stop the animation.
		 * @param id => the id of the frame to be set to.
		*/
		void SetCurrFrame(std::uint32_t id) noexcept;

		/**
		 * @return the amount of time a single frame takes on screen.
		*/
		std::chrono::duration<float> FrameTime() const noexcept;

		/**
		 * @return the duration between the start of the animation and the current state.
		*/
		std::chrono::duration<float> CurrTime() const noexcept;

		/**
		 * @return the id of the current frame.
		*/
		std::uint32_t CurrFrame() const noexcept;

		/**
		 * @return true if the animation is not paused, false otherwise.
		*/
		bool IsAnimating() const noexcept;

	private:
		// better than setting the frame time to infinity and having the sprite update constantly for no reason.
		bool bStop_{};
		bool bReverse_{};
		std::chrono::duration<float> frameTime_{};
		std::chrono::duration<float> currTime_{};
	};
}

