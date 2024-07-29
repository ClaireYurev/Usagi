#pragma once
#include <sstream>
#include "Window.h"
#include "resource.h"


// Window Class Logic
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	//wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0 ));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	//wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window Logic Implementation
Window::Window(int width, int height, const char* name)
	: width(width), height(height)
{
	// Calculate the final window size based on the desired size of the client region
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	// throw USAGI_EXCEPT(ERROR_ARENA_TRASHED); // Engine exception (detailed)
	// throw std::runtime_error("Something went wrong...");  // Standard exception (minimal)
	// throw 125619846; // Unknown exception (no details available)

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw USAGI_LAST_EXCEPT();
	};
	// Create a window & get handle to the window
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// Check for any exceptions
	if (hWnd == nullptr)
	{
		throw USAGI_LAST_EXCEPT();
	}
	// Windows that were created new start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	
	// Create a graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw USAGI_LAST_EXCEPT();
	}

}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// While there are messages on the queue, remove and dispatch them
	// BUT DO NOT BLOCK IF QUEUE IS EMPTY
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Since peekmessage doesn't signal quitting on its return value, check if it is WM_QUIT
		if (msg.message == WM_QUIT)
		{
			// Return an optional wrapping int (argument to PostQuitMessage is in wparam) signifies a quit
			return msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Return an empty optional when not quitting application
	return {};
}

Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		throw USAGI_NOGFX_EXCEPT();
	}
	return *pGfx;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data to store a pointer to window class
		#pragma warning(push)
		#pragma warning(disable : 6215)
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		#pragma warning(pop)

		// Set the message proc to normal (non-setup) handler now that setup is finished
		#pragma warning(push)
		#pragma warning(disable : 6215)
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		#pragma warning(pop)

		// Forward the message to our window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// In case we receive a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Retrieve the pointer that points to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Forward the message to our window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		// Return 0 (not "break" !) as we don't want DefProc to handle this message
		return 0; // Since our destructor should be the one to destroy the window
	// Clear keystate if window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/*********** KEYBOARD INPUT LOGIC  ***********/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// Check bit 30 to see if it was 1 or 0: mask the lParam and check if it's false
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// Check bit 30 to see if it was 1 or 0: mask the lParam and check if it's false
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	/*********** KEYBOARD LOGIC DONE ***********/

	/************* MOUSE INPUT LOGIC ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// If inside client window -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// Else if outside client window -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// Button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// If outside client window, release the mouse
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// If outside client window, release the mouse
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** MOUSE LOGIC DONE **************/

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Window Exception Functionality
// gone after commit 4a2e05...

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// Windows will allocate memory for error string and have our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// IF returned string length is zero, there is a problem
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// Copy the error string from buffer allocated by Windows to std::string
	std::string errorString = pMsgBuf;
	// Free up the Windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Usagi Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGfxException::GetType() const noexcept
{
	return "Usagi Window Exception [No Graphics]";
}