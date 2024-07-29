#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 400, "Usagi Engine Main Window")
{}

int App::Go()
{   
	while (true)
	{
		// Evaluate all pending messages, but never block in anticipation of new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// If the return optional has some value, that signifies a quit, so we return an exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().EndFrame();
	/* Old test code
	const float t = timer.Peek();
    std::ostringstream oss;
    oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
    wnd.SetTitle(oss.str());
	*/
}