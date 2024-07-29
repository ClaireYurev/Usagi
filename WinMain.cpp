#pragma
#include <sstream>
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
            
            // New mouse input test 2: Scrolling
            static int i = 0;
            while (!wnd.mouse.IsEmpty())
            {
                const auto e = wnd.mouse.Read();
                switch (e.GetType())
                {
                case Mouse::Event::Type::WheelUp:
                    i++;
                    {
                        std::ostringstream oss;
                        oss << "Up: " << i;
                        wnd.SetTitle(oss.str());
                    }
                    break;
                case Mouse::Event::Type::WheelDown:
                    i--;
                    {
                        std::ostringstream oss;
                        oss << "Down: " << i;
                        wnd.SetTitle(oss.str());
                    }
                    break;
                }
            }

            /* OLD TEST LOGIC: v0.2
            // New mouse input test: Coordinates
            while (!wnd.mouse.IsEmpty())
            {
                const auto e = wnd.mouse.Read();
                switch (e.GetType())
                {
                case Mouse::Event::Type::Leave:
                    wnd.SetTitle("Cursor is out of client window");
                    break;
                case Mouse::Event::Type::Move:
                {
                    std::ostringstream oss;
                    oss << "Cursor moved to (" << e.GetPosX() << "," << e.GetPosY() << ")";
                    wnd.SetTitle(oss.str());
                }
                break;
                }
            }

            */

            /* OLD TEST LOGIC: v0.1
            
             if (wnd.kbd.IsKeyPressed(VK_MENU))
            {
                MessageBox(nullptr, "Alt Key Was Pressed", "Keyboard input:", MB_OK | MB_ICONEXCLAMATION);
            }
            // Initial mouse pointer logic test
            while (!wnd.mouse.IsEmpty())
            {
                const auto e = wnd.mouse.Read();
                if (e.GetType() == Mouse::Event::Type::Move)
                {
                    std::ostringstream oss;
                    oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
                    wnd.SetTitle(oss.str());
                }
            }
            */

        }

        // Check whether GetMessage call succeeded
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
