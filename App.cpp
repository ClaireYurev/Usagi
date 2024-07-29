#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 400, "Usagi Engine Main Window")
{}

int App::Go()
{
	MSG msg;
	BOOL gResult;
	// GetMessage BLOCKS whenever there's no message in loop (so when no input is received).
	// In other words, this only works while you press keys / move mouse, etc
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key messages
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		DoFrame();
	}

	// Check whether GetMessage call succeeded
	if (gResult == -1)
	{
		return -1;
	}

	// The wParam is the value passed to PostQuitMessage
	return static_cast<int>(msg.wParam);
}

// DoFrame() contains the top level of our game logic
void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}