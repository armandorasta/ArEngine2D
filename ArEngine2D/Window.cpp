#include "Window.h"

#include "ImplUtil.h"
#include "IEngineError.h"

#define WINDOW_ERROR(str) WindowError{str}

namespace ArEngine2D
{
	Window::Window(std::string_view title, std::int32_t width, std::int32_t height,
		std::int32_t x, std::int32_t y)
		: handle_{}, title_{title}, wtitle_{title.begin(), title.end()},
		width_{width}, height_{height},
		x_{x}, y_{y}
	{
		Initialize();
	}
	Window::~Window()
	{
		if (handle_)
		{
			DestroyWindow(handle_);
			UnregisterClassW(sc_ClassName, GetModuleHandleA(nullptr));
		}
	}
	void Window::UpdateTitle() noexcept
	{
		// may fail yes but, I have never had it fail on me.
		SetWindowTextA(handle_, title_.data());
	}
	void Window::Initialize()
	{
		if (handle_)
		{
			throw WINDOW_ERROR("tried to initialize window(" + title_ + ") twice");
		}

		InitializeClass();

		RECT winRect{
			.left{},
			.top{},
			.right{static_cast<LONG>(width_)},
			.bottom{static_cast<LONG>(height_)}
		};
		if (not AdjustWindowRectEx(&winRect, Window::sc_WinStyle, FALSE, NULL))
		{
			throw WINDOW_ERROR("AdjustWindowRect returned false");
		}
		auto const actWidth{static_cast<int>(winRect.right - winRect.left)};
		auto const actHeight{static_cast<int>(winRect.bottom - winRect.top)};

		bool useDefaultWindowLoc{x_ == 0 and y_ == 0};
		auto const actX{useDefaultWindowLoc ? CW_USEDEFAULT : static_cast<int>(x_)};
		auto const actY{useDefaultWindowLoc ? CW_USEDEFAULT : static_cast<int>(y_)};

		handle_ = CreateWindowExW(NULL, Window::sc_ClassName, wtitle_.c_str(), Window::sc_WinStyle,
			actX, actY, actWidth, actHeight, nullptr, nullptr, GetModuleHandleA(nullptr), nullptr);

		if (not useDefaultWindowLoc)
		{
			auto info = WINDOWPLACEMENT{};
			if (not GetWindowPlacement(handle_, std::addressof(info)))
			{
				throw WINDOW_ERROR("failed to update the window location");
			}

			x_ = info.rcNormalPosition.left;
			y_ = info.rcNormalPosition.top;
		}

		if (not handle_)
		{
			throw WINDOW_ERROR("CreateWindowExW returned null");
		}

		ShowWindow(handle_, SW_SHOW);
		SetForegroundWindow(handle_);
		SetFocus(handle_);
		UpdateWindow(handle_);
		Window::s_ActiveWindow = this;
	}
	auto Window::ProcessMessages() noexcept -> bool
	{
		MSG msg{};
		while (PeekMessageW(&msg, handle_, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return IsWindow(handle_);
	}
	void Window::InputUpdate() noexcept
	{
		keyboard.FrameUpdate();
		mouse.FrameUpdate();
	}
	void Window::InitializeRawInput()
	{
		RAWINPUTDEVICE rawInDev{};
		rawInDev.usUsagePage = 0x1;
		rawInDev.usUsage = 0x2;
		rawInDev.dwFlags = NULL;
		rawInDev.hwndTarget = nullptr;

		if (not RegisterRawInputDevices(&rawInDev, 1U, sizeof rawInDev))
		{
			throw WINDOW_ERROR(
				static_cast<HRESULT>(GetLastError())/*"registering raw input devices failed!"*/
			);

			/*

				TODO: handle this shit better.

			*/
		}

		s_bRawInputInitialized_ = true;
	}
	void Window::EnableRawInput()
	{
		if (not IsRawInputEnabled())
		{
			InitializeRawInput();
		}

		s_bRawInputEnabled_ = true;
	}
	void Window::InitializeClass()
	{
		auto const hInstance{GetModuleHandleA(nullptr)};
		WNDCLASSEX clazz{};
		clazz.cbSize = sizeof clazz;
		clazz.hInstance = hInstance;
		clazz.lpszClassName = Window::sc_ClassName;
		clazz.cbClsExtra = {};
		clazz.cbWndExtra = {};
		clazz.hbrBackground = {};
		clazz.hCursor = LoadCursorW(hInstance, IDC_ARROW);
		clazz.hIcon = {};
		clazz.hIconSm = {};
		clazz.lpfnWndProc = &Window::WinProc;
		clazz.lpszMenuName = L"menu name";
		clazz.style = Window::sc_ClassStyle;

		RegisterClassExW(&clazz);
	}
	auto Window::ActiveWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		-> LRESULT
	{
		switch (msg)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN: // for ALT
			{
				keyboard.SetKey(static_cast<std::size_t>(wParam), true);
				break;
			}

			case WM_KEYUP:
			case WM_SYSKEYUP: // for ALT
			{
				keyboard.SetKey(static_cast<std::size_t>(wParam), false);
				break;
			}

			case WM_CHAR:
			{
				keyboard.PushChar(static_cast<char>(wParam));
				break;
			}

			case WM_KILLFOCUS:
			{
				// this prevents a problem with WM_KEYUP messages.
				keyboard.Reset();
				break;
			}

			case WM_MOUSEMOVE:
			{
				// not using MAKEPOINTS because: i am not sure if it's well defined.
				std::int16_t const x{lParam & 0xFFFF};
				std::int16_t const y{(lParam >> 16) & 0xFFFF};
				if ((x >= 0 and x < width_) and (y >= 0 and y < height_))
					// inside of the window?
				{
					mouse.SetLoc(static_cast<float>(x), static_cast<float>(y));
					if (not mouse.IsInWindow())
					{
						SetCapture(hWnd);
						mouse.OnWindowEnter();
					}
				}
				else
				{
					if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
						// just got out of the window and not dragging anything?
					{
						mouse.SetLoc(
							static_cast<float>(x),
							static_cast<float>(y)
						);
					}
					else
					{
						ReleaseCapture();
						mouse.OnWindowExit();
					}
				}

				break;
			}

			case WM_LBUTTONDOWN:
				mouse.SetKey(Mouse::sc_LeftButtonID, true);  break;
			case WM_MBUTTONDOWN:
				mouse.SetKey(Mouse::sc_MiddleButtonID, true);   break;
			case WM_RBUTTONDOWN:
				mouse.SetKey(Mouse::sc_RightMouseID, true); break;

			case WM_LBUTTONUP:
				mouse.SetKey(Mouse::sc_LeftButtonID, false);  break;
			case WM_MBUTTONUP:
				mouse.SetKey(Mouse::sc_MiddleButtonID, false);   break;
			case WM_RBUTTONUP:
				mouse.SetKey(Mouse::sc_RightMouseID, false); break;

			case WM_MOUSEWHEEL:
			{
				auto del{GET_WHEEL_DELTA_WPARAM(wParam)};
				if (del > 0)
				{
					for (; del > 0; del -= WHEEL_DELTA)
					{
						mouse.OnWheel(true);
					}
				}
				else
				{
					for (; del < 0; del += WHEEL_DELTA)
					{
						mouse.OnWheel(false);
					}
				}
				break;
			}

			case WM_INPUT:
			{
				if (not s_bRawInputEnabled_) // no to alocate memory for nothing.
				{
					break;
				}

				// storage for raw input size.
				UINT rawInputByteCount{};

				// get the size of input
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr /*because we don't
					the size of the input yet.*/, std::addressof(rawInputByteCount), sizeof RAWINPUTHEADER);

				if (rawInputByteCount < 0) // failed
				{
					break;
				}

				if (rawInputData.size() < rawInputByteCount)
					// optimization, just ffs don't use rawInputData.size() 
				{
					rawInputData.resize(rawInputByteCount);
				}

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawInputData.data(),
					std::addressof(rawInputByteCount), sizeof RAWINPUTHEADER) != rawInputByteCount)
				{
					break;
				}

				mouse.OnRawInput(*reinterpret_cast<RAWINPUT*>(rawInputData.data()));
				break;
			}
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	auto Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		->LRESULT
	{
		return s_ActiveWindow->ActiveWinProc(hWnd, msg, wParam, lParam);
	}
}
