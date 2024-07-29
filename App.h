#pragma once
#include "Timer.h"
#include "Window.h"

class App
{
public:
	App();
	// Main frame / message game loop
	int Go();
private:
	void DoFrame();

private:
	Window wnd;
	UsagiTimer timer;
};