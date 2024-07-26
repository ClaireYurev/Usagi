#include <Windows.h>
#include "WinMsgLogger.h"
#include <sstream>

LRESULT CALLBACK CustomWindowsProcedure(
    HWND handleToTheWindow,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    // Look through the Windows message map and print it out
    static WinMsgLogger messageLogger;
    OutputDebugStringW(messageLogger(msg, lParam, wParam).c_str());

    // Behavior for when the window needs to close
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(73); // Closing - step 1: Application quits
        break;
    case WM_KEYDOWN:
        if (wParam == 'H')
        {
            SetWindowText(handleToTheWindow, L"Palance ! ! !");
        }
        break;
    case WM_KEYUP:
        if (wParam == 'H')
        {
            SetWindowText(handleToTheWindow, L". 1 . 2 . 3 . 4 . ");
        }
        break;
    case WM_CHAR:
        {
        static std::wstring title;
        title.push_back((char)wParam);
        SetWindowText(handleToTheWindow, title.c_str());
        }
        break;
    case WM_LBUTTONDOWN:
        {
        POINTS point = MAKEPOINTS(lParam);
        std::wostringstream oss;
        oss << "(" << point.x << "," << point.y << ")";
        SetWindowText(handleToTheWindow, oss.str().c_str());
        }
        break;
    }
    return DefWindowProc(handleToTheWindow, msg, wParam, lParam); // Closing - step 2: Window is destroyed
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Register window class
    const auto lpClassName = L"Usagi Primary Window Class";  // Wide string
    WNDCLASSEXW primaryWindowClass = { 0 }; // Wide-character version
    primaryWindowClass.cbSize = sizeof(primaryWindowClass);
    primaryWindowClass.style = CS_OWNDC;
    primaryWindowClass.lpfnWndProc = CustomWindowsProcedure;
    primaryWindowClass.cbClsExtra = 0;
    primaryWindowClass.cbWndExtra = 0;
    primaryWindowClass.hInstance = hInstance;
    primaryWindowClass.hIcon = nullptr;
    primaryWindowClass.hCursor = nullptr;
    primaryWindowClass.hbrBackground = nullptr;
    primaryWindowClass.lpszMenuName = nullptr;
    primaryWindowClass.lpszClassName = lpClassName;
    RegisterClassExW(&primaryWindowClass); // Wide-character version

    // Create instance of that class (Create a window)
    const auto lpWindowName = L"Usagi Primary Window Instance";  // Wide string
    HWND handleToTheWindow = CreateWindowExW(
        0,
        lpClassName,
        lpWindowName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 640, 480,
        nullptr, nullptr, hInstance, nullptr
    );
    // Window is only rendered once we call ShowWindow
    ShowWindow(handleToTheWindow, SW_SHOW);

    // Message pump
    MSG msg;
    BOOL getResult;

    while ((getResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (getResult == -1)
    {
        return -1;
    }
    else
    {
        return static_cast<int>(msg.wParam); // Explicitly cast to int to address warning C4244
        /*  Let's return msg.wParam as int, when dealing with Windows applications, assuming the values are within the range of int.
        This might cause loss of data if msg.wParam holds a value larger than the maximum value of int.   */
    }
    // return 0; // Deprecated due to the if / else statement capturing wParam and returning it
}