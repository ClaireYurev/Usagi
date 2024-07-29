#include "Window.h"

/* Usagi Game Engine */

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    try
    {
        Window wnd(800, 400, "Usagi Engine Main Window");

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            // TranslateMessage will post auxiliary WM_CHAR messages from key msgs
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (wnd.kbd.IsKeyPressed(VK_MENU))
            {
                MessageBox(nullptr, "Alt Key Was Pressed", "Keyboard input:", MB_OK | MB_ICONEXCLAMATION);
            }
        }

        // Check if GetMessage call succeeded
        if (gResult == -1)
        {
            return -1;
        }

        // The wParam is the value passed to PostQuitMessage
        return static_cast<int>(msg.wParam);
    }
    catch (const UsagiException& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}
