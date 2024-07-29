#pragma once
#include "Timer.h"
#include "Window.h"

class App
{
public:
	App();
	// Main frame / message loop
	int Go();
private:
	void DoFrame();
	// render frame
	// fire frame
	// launch frame
	// execute frame
	// create frame
private:
	Window wnd;
	UsagiTimer timer;
};