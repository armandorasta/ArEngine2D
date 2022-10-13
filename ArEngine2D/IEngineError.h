#pragma once

#include "ImplUtil.h"

#include <comdef.h>
#include <dxgidebug.h>
#include <d2d1.h>

#include <string>
#include <string_view>
#include <cstdint>
#include <source_location>
#include <tuple>
#include <functional>

namespace ArEngine2D {
	/**
	 * @brief abstract base class of all errors thrown by the eng.
	*/
	class IEngineError
	{
	private:

		inline static UINT64 s_InfoQueueCurrentIndex{};
		inline static Details::Ptr<IDXGIInfoQueue> s_pInfoQueue_{};

	public:

		IEngineError(std::string_view message, std::source_location debugInfo = std::source_location::current());
		IEngineError(HRESULT hRes, std::string_view optMessage = {}, std::source_location debugInfo = std::source_location::current());

	public:

		static void InitializeInfoQueue();
		static auto HasNewMessages() -> bool;

	protected:
		static void SetInfoQueueIndex();

	private:
		static auto GetErrorMessage(HRESULT hRes)->std::string;
		static auto GetErrorDesc()->std::string;
		static auto PullNewMessages()->std::string;

	public:

		/**
		 * @return a decent title for a an error message box.
		*/
		virtual std::string MessageBoxTitle() const = 0;

		/**
		 * @return the error message with the answer to everything.
		*/
		constexpr auto Message() const { return message_; }

	private:
		std::string message_;
	};

	/**
	 * @brief errors thrown by Engine class.
	*/
	class EngineError : public IEngineError
	{
		using IEngineError::IEngineError;
		virtual std::string MessageBoxTitle() const override;
	};

	/**
	 * @brief errors thrown by Window class.
	*/
	class WindowError : public IEngineError
	{
		using IEngineError::IEngineError;
		std::string MessageBoxTitle() const override;
	};

	/**
	 * @brief errors thrown during intialization of the error handling class.
	*/
	class InitializationError : public IEngineError
	{
		using IEngineError::IEngineError;
		virtual std::string MessageBoxTitle() const override;
	};

	/**
	 * @brief errors thrown by Grafix class.
	*/
	class GraphicsError : public IEngineError
	{
	public:

		using IEngineError::IEngineError;
		virtual std::string MessageBoxTitle() const override;

	public:
		// Ok to use std::function because, the macro does not call this function is release mode.
		static void Handle(std::function<HRESULT()>&& callable, std::string_view optMessage = { }, 
			std::source_location debugInfo = std::source_location::current());
		static void HandleEndDrawError(ID2D1HwndRenderTarget* pRenderTarget,
			std::source_location debugInfo = std::source_location::current());
	};
}


#ifndef NDEBUG
// These two are probably gonna remain as macros forever.
#define HANDLE_GRAPHICS_ERROR(hResCall) GraphicsError::Handle([&]{ return hResCall; })
#define HANDLE_ENDDRAW_ERROR(_pRenderTarget) GraphicsError::HandleEndDrawError(_pRenderTarget)
#else
#define HANDLE_GRAPHICS_ERROR(hResCall) hResCall
#define HANDLE_ENDDRAW_ERROR(_pRenderTarget) _pRenderTarget->EndDraw()
#endif

#define DEVICE_REMOVED_ERROR(pDevice) \
	GraphicsError{pDevice->GetDeviceRemovedReason(), "device removed?!"}
