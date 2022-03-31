#include "IEngineError.h"

#include <dxgidebug.h>
#include <format>

#pragma comment(lib, "dxguid.lib")

namespace ArEngine2D
{
    IEngineError::IEngineError(std::string_view message, std::source_location debugInfo)
    {
        message_ = std::format(
            "[basic description] {}.\n"
            "[file] {}\n"
            "[line] {}",
            message,
            debugInfo.line(),
            debugInfo.file_name()
        );
    }
    IEngineError::IEngineError(HRESULT hRes, std::string_view optMessage, std::source_location debugInfo)
    {
        if (optMessage.empty())
        {
            message_ = std::format(
                "[basic description] {}.\n"
                "[full description]\n{}.\n\n"
                "[file] {}\n"
                "[line] {}",
                GetErrorMessage(hRes),
                GetErrorDesc(),
                debugInfo.file_name(),
                debugInfo.line()
            );
        }
        else
        {
            message_ = std::format(
                "[basic description] {}.\n"
                "[full description]\n{}.\n\n"
                "[small note] {}\n"
                "[file] {}\n"
                "[line] {}",
                GetErrorMessage(hRes),
                GetErrorDesc(),
                optMessage,
                debugInfo.file_name(),
                debugInfo.line()
            );
        }
    }
    void IEngineError::InitializeInfoQueue()
    {
        if (IEngineError::s_pInfoQueue_) // already initialized?
        {
            return;
        }

        auto const hModDxgiDebug{
            LoadLibraryExA("dxgidebug.dll", nullptr,
                LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                LOAD_LIBRARY_SEARCH_SYSTEM32 |
                LOAD_LIBRARY_SEARCH_USER_DIRS
            )
        };
        
        if (not hModDxgiDebug)
        {
            throw InitializationError{"could not load dxgidebug.lib"};
        }

        auto const DxgiGetDebugInterface{
            reinterpret_cast<HRESULT(WINAPI*)(REFIID, void**)>(
                reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
            )
        };

        if (not DxgiGetDebugInterface)
        {
            throw InitializationError{"could not get interface from dxgidebug.lib for IDXGIInfoQueue"};
        }

        if (FAILED(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &s_pInfoQueue_)))
        {
            throw InitializationError{"could not get interface from dxgidebug.lib for IDXGIInfoQueue"};
        }

        SetInfoQueueIndex();
    }
    auto IEngineError::GetErrorMessage(HRESULT hRes) -> std::string
    {
        char* messageBuffer = nullptr;
        auto messageLen{FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hRes,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<PSTR>(&messageBuffer),
            0,
            nullptr
        )};

        std::string const messageStr{messageLen ? messageBuffer : "unidentified error code"};
        LocalFree(reinterpret_cast<HLOCAL>(messageBuffer));
        return messageStr;
    }
    auto IEngineError::GetErrorDesc() -> std::string
    {
        auto const desc{PullNewMessages()};
        return desc.empty() ? "no description" : desc;
    }
    void IEngineError::SetInfoQueueIndex()
    {
        s_InfoQueueCurrentIndex = s_pInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
    }
    auto IEngineError::HasNewMessages() -> bool
    {
        return s_pInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL) > s_InfoQueueCurrentIndex;
    }
    auto IEngineError::PullNewMessages() -> std::string
    {
        if (not HasNewMessages()) 
        {
            return {};
        }

        std::string resMessage{};

        auto const currMessageCount{s_pInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL)};
        for (auto i{s_InfoQueueCurrentIndex}, count{1UI64}; i < currMessageCount; ++i, ++count) 
        {
            SIZE_T messageLen{};
            // get the length by passing nullptr
            s_pInfoQueue_->GetMessageW(DXGI_DEBUG_ALL,  i, nullptr, &messageLen);

            // allocate a buffer for the fucking message
            auto buffer{std::make_unique<char[]>(messageLen)};
            // do some undefined behaviour
            auto pMessage{reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(buffer.get())};

            // finally get the shit
            s_pInfoQueue_->GetMessageW(DXGI_DEBUG_ALL, i, pMessage, &messageLen);

            // get the description
            std::string messageStr{pMessage->pDescription};

            // add it to the list
            resMessage = std::format("{} {}: {}\n", resMessage, count, pMessage->pDescription);
        }

        return resMessage;
    }
    std::string WindowError::MessageBoxTitle() const
    {
        return "Window Error";
    }
    std::string EngineError::MessageBoxTitle() const
    {
        return "Engine Error";
    }
    std::string GraphicsError::MessageBoxTitle() const
    {
        return "Graphics Error";
    }
    void GraphicsError::Handle(std::function<HRESULT()>&& callable, std::string_view optMessage, std::source_location debugInfo)
    {
        SetInfoQueueIndex();
        if (auto const hRes{callable()}; FAILED(hRes))
        {
            throw GraphicsError{hRes, optMessage, debugInfo};
        }
    }
    void GraphicsError::HandleDraw(std::function<void()>&& callable, std::string_view optMessage, std::source_location debugInfo)
    {
        SetInfoQueueIndex();
        callable();
        if (HasNewMessages())
        {
            throw GraphicsError{E_INVALIDARG, optMessage, debugInfo};
        }
    }
    std::string InitializationError::MessageBoxTitle() const
    {
        return "Initialization Error";
    }
}