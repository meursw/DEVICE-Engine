#include "GFX_d3dclass.h"
#include "SYS_d3d_exception.h"

#include <vector>
#include <memory>

using namespace Microsoft::WRL;

D3DClass::D3DClass(int screenWidth, int screenHeight, bool vysnc, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) 
{
	int numerator{ 0 }, denominator{ 0 };

	GetVideoCardInformation(
		screenWidth,
		screenHeight,
		numerator,
		denominator
	);

	CreateDeviceAndSwapChain(
		screenWidth,
		screenHeight,
		vysnc,
		numerator,
		denominator,
		hwnd,
		fullscreen
	);

	CreateRenderTargetView();

	CreateDepthStencilBuffer(screenWidth, screenHeight);

	CreateDisabledDepthState();

	CreateRasterState();

	CreateBlendState();

	CreateViewport(screenWidth, screenHeight);

}

void D3DClass::GetVideoCardInformation(int screenWidth, int screenHeight, int& numerator, int& denominator)
{
	HRESULT hr{};

	ComPtr<IDXGIFactory> factory; // Used to create DXGI objects
	// Create DX graphics interface factory. We use uuidof to get identifier of an interface.
	D3D_THROW(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()));

	ComPtr<IDXGIAdapter> adapter; // Represents display subsystem (GPUs etc)
	// Use the factory to create an adapter for the primary graphics interface (video card).
	D3D_THROW(factory->EnumAdapters(
		0, adapter.GetAddressOf()
	));

	ComPtr<IDXGIOutput> adapterOutput; // Represents an adapter output (such as a monitor).
	// Enumerate the primary adapter output (monitor)
	D3D_THROW(adapter->EnumOutputs(
		0, adapterOutput.GetAddressOf()
	));

	unsigned int numModes{ 0 };
	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	D3D_THROW(adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL
	));

	std::vector<DXGI_MODE_DESC> displayModeList(numModes); // Describes a display mode, display modes get infromation about the monitor and use this information for the swap chain buffers
	// Create a list to hold all the possible display modes for this monitor/video card combination.

	// Now fill the display mode list structures.
	D3D_THROW(adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, 
		&numModes, displayModeList.data()
	));

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	unsigned int num{ 0 }, deno{ 0 }; // Refresh rate

	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				num = displayModeList[i].RefreshRate.Numerator;
				deno = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc; // Graphics card description (name, VRAM, etc)
	hr = adapter->GetDesc(&adapterDesc);

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024); // in megabytes

	unsigned long long stringLength;

	int error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
		throw DEVICE_Exception(__LINE__, __FILE__);

	// Store the values of numerator and denominator to be used when creating the swap chain
	numerator = num;
	denominator = deno;
}

void D3DClass::CreateDeviceAndSwapChain(int screenWidth, int screenHeight, bool vysnc, int n, int d, HWND hwnd, bool fullscreen)
{
	// Now that we have the refresh rate from the system, we can start the DirectX initialization.
	// The first thing we'll do is fill out the description of the swap chain. 
	// The swap chain is the front and back buffer to which the graphics will be drawn. 
	// Generally, you use a single back buffer, do all your drawing to it, and then swap it to the front buffer which then displays on the user's screen. 
	// That is why it is called a swap chain.

	DXGI_SWAP_CHAIN_DESC swapChainDesc; // Describes the settings of the swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	// Set width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// The next part of the description of the swap chain is the refresh rate. 
	// The refresh rate is how many times a second it draws the back buffer to the front buffer.
	m_vsync_enabled = vysnc;

	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = n;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = d;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = !fullscreen;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// END OF SWAP CHAIN INITIALIZATION

	HRESULT hr;

	D3D_THROW(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, 
		&featureLevel, 1, D3D11_SDK_VERSION, 
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext
	));
}

void D3DClass::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBufferPtr; // Pointer to the back buffer

	// Get the pointer to the back buffer from the swapchain.
	D3D_THROW(m_swapChain->GetBuffer(
		0, 
		__uuidof(ID3D11Texture2D), 
		(LPVOID*)backBufferPtr.GetAddressOf()
	));

	// Create the render target view with the back buffer pointer.
	D3D_THROW(m_device->CreateRenderTargetView(
		backBufferPtr.Get(), 
		NULL, 
		&m_renderTargetView)
	);
}

void D3DClass::CreateDepthStencilBuffer(int screenWidth, int screenHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc; // Describes the depth buffer texture

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create depth/stencil buffer texture. 
	D3D_THROW(m_device->CreateTexture2D(
		&depthBufferDesc,
		NULL,
		&m_depthStencilBuffer)
	);

	// Now we need to setup the depth stencil description. 
	// This allows us to control what type of depth test Direct3D will do for each pixel.

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc; // Describes depth-stencil state.

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	D3D_THROW(m_device->CreateDepthStencilState(
		&depthStencilDesc, 
		m_depthStencilState.GetAddressOf()
	));

	// With the created depth stencil state, we can now set it so that it takes effect.
	// Notice we use the device context to set it.
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; // Specifies the subresources of a texture that are accessible from a depth-stencil view.

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	D3D_THROW(m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(), 
		&depthStencilViewDesc, 
		m_depthStencilView.GetAddressOf()
	));

	// Bind the render target and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void D3DClass::CreateDisabledDepthState()
{
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr;
	D3D_THROW(m_device->CreateDepthStencilState(
		&depthDisabledStencilDesc, 
		m_depthDisabledStencilState.GetAddressOf()
	));
}

void D3DClass::CreateRasterState()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	D3D_THROW(m_device->CreateRasterizerState(
		&rasterDesc, 
		m_rasterState.GetAddressOf()
	));

	m_deviceContext->RSSetState(m_rasterState.Get());
}

void D3DClass::CreateBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC blendStateDescription;

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	D3D_THROW(m_device->CreateBlendState(
		&blendStateDescription, 
		m_alphaEnableBlendingState.GetAddressOf()
	));

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	D3D_THROW(m_device->CreateBlendState(
		&blendStateDescription, 
		m_alphaDisableBlendingState.GetAddressOf()
	));
}

void D3DClass::CreateViewport(int screenWidth, int screenHeight)
{
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);
}


void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	const float color[4]{ red, green, blue, alpha };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
}

// Present the back buffer to the screen since rendering is complete.
void D3DClass::EndScene()
{
	if (m_vsync_enabled)
		m_swapChain->Present(1, 0); // Lock to screen refresh rate.
	else
		m_swapChain->Present(0, 0); // Present as fast as possible.
}

ID3D11Device* D3DClass::GetDevice() const {
	return m_device.Get();
}


ID3D11DeviceContext* D3DClass::GetDeviceContext() const {
	return m_deviceContext.Get();
}

void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix) const {
	worldMatrix = m_worldMatrix;
	return;
}

const Camera* D3DClass::GetCamera() const
{
	return m_Camera;
}

void D3DClass::SetCamera(Camera* cam)
{
	m_Camera = cam;
}

void D3DClass::UpdateCurrentCamera() const
{
	m_Camera->UpdateViewMatrix();
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory) const {
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void D3DClass::ResetViewport()
{
	// Set the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void D3DClass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
}

void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
}


void D3DClass::EnableAlphaBlending()
{
	float blendFactor[4]{ 0.0f };

	// Turn on alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState.Get(), blendFactor, 0xffffffff);
}

void D3DClass::DisableAlphaBlending()
{
	float blendFactor[4]{ 0.0f };

	// Turn off alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState.Get(), blendFactor, 0xffffffff);
}
