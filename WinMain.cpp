#include <Windows.h>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Register window class
	const auto lpClassName = L"Usagi Primary Window Class";  // Wide string
	WNDCLASSEXW primaryWindowClass = { 0 }; // Wide-character version
	primaryWindowClass.cbSize = sizeof(primaryWindowClass);
	primaryWindowClass.style = CS_OWNDC;
	primaryWindowClass.lpfnWndProc = DefWindowProc;
	primaryWindowClass.cbClsExtra = 0;
	primaryWindowClass.cbWndExtra = 0;
	primaryWindowClass.hInstance = hInstance;
	primaryWindowClass.hIcon = nullptr;
	primaryWindowClass.hCursor = nullptr;
	primaryWindowClass.hbrBackground = nullptr;
	primaryWindowClass.lpszMenuName = nullptr;
	primaryWindowClass.lpszClassName = lpClassName;
	RegisterClassExW( &primaryWindowClass ); // Wide-character version

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
	while (true);
	return 0;
}