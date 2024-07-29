#include "Graphics.h"
#include "dxerr.h"
#include <sstream>

#pragma comment(lib,"d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

Graphics::Graphics(HWND hWnd)
{   
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    //sd.OutputWindow = hWnd;
    sd.OutputWindow = (HWND)696969;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    // Memver variable for checking the results of d3d functions
    HRESULT hr;

    // Create the device as well as front & back buffers, also the swap chain and rendering context
    GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        // 0, // RELEASE CONFIGURATION - 3D EXCEPTIONS GIVE NO INFO
        D3D11_CREATE_DEVICE_DEBUG, // DEBUG CONFIGURATION - FULL INFO ON ALL 3D EXCEPTIONS
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr, // Output pointer
        &pContext
    ));
    // Obtain access to texture sub-resource in the Swap Chain (back buffer)
    ID3D11Resource* pBackBuffer = nullptr;
    GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget));
    pBackBuffer->Release();
}

Graphics::~Graphics()
{
    if (pTarget != nullptr)
    {
        pTarget->Release();
    }
    if (pContext != nullptr)
    {
        pContext->Release();
    }
    if (pSwap != nullptr)
    {
        pSwap->Release();
    }
    if (pDevice != nullptr)
    {
        pDevice->Release();
    }

}

void Graphics::EndFrame()
{
    HRESULT hr;
    if (FAILED(hr = pSwap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_FAILED(hr);
        }
    }
}


// Direct3D (DirectX11 as far back as Windows 7) Graphics exception functionality
Graphics::HrException::HrException( int line,const char * file,HRESULT hr ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Usagi Direct3D Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString( hr );
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription( hr,buf,sizeof( buf ) );
	return buf;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Usagi Direct3D Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
