#pragma once
#include "LightenedWindowsHeader.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	// Simplest step 1: present the back buffer to the front buffer, i.e. FLIPPING:
	void EndFrame();
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
};