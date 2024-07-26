#include "Window.h"
#include <string>

Window::WindowClass Window::WindowClass::customWindowClass;

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
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(WindowClass::customWindowClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
    return customWindowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return customWindowClass.hInst;
}


// Window Stuff
Window::Window(int width, int height, const char* name)
    :
    width(width),
    height(height)
{
    // convert name to wide string
    std::wstring wideName(name, name + strlen(name));

    // calculate window size based on desired client region size
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    // create window & get hWnd
    handleToTheWindow = CreateWindow(
        WindowClass::GetName(), wideName.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );
    // Display window
    ShowWindow(handleToTheWindow, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(handleToTheWindow);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND handleToTheWindow, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (message == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pointerToOurWindow = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(handleToTheWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pointerToOurWindow));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(handleToTheWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pointerToOurWindow->HandleMsg(handleToTheWindow, message, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(handleToTheWindow, message, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND handleToTheWindow, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window instance
    Window* const pointerToOurWindow = reinterpret_cast<Window*>(GetWindowLongPtr(handleToTheWindow, GWLP_USERDATA));
    // forward message to window instance handler
    return pointerToOurWindow->HandleMsg(handleToTheWindow, message, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND handleToTheWindow, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
        // we don't want the DefProc to handle this message because
        // we want our destructor to destroy the window, so return 0 instead of break
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        // clear keystate when window loses focus to prevent input getting "stuck"
    default:
        return DefWindowProc(handleToTheWindow, message, wParam, lParam);
    }
}
