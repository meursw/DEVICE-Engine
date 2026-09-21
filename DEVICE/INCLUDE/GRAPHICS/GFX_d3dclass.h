#pragma once

#pragma comment(lib, "d3d11.lib") // Contains D3D functionality for setting up and drawing 3D graphics.
#pragma comment(lib, "dxgi.lib") // Contains functionality to interface with the hardware.
#pragma comment(lib, "d3dcompiler.lib") // Contains functionality for compiling shaders.

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include <wrl/client.h> // Include for ComPtr

#include "GFX_camera.h"

class D3DClass
{
public:
	D3DClass(int, int, bool, HWND, bool, float, float);
	~D3DClass() = default;

private:
	void GetVideoCardInformation(int, int, int&, int&);
	void CreateDeviceAndSwapChain(int, int, bool, int, int, HWND, bool);
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer(int, int);
	void CreateDisabledDepthState();
	void CreateRasterState();
	void CreateBlendState();
	
	void CreateViewport(int, int);

public:
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void GetWorldMatrix(XMMATRIX&) const;

	const Camera* GetCamera() const;
	void SetCamera(Camera*);
	void UpdateCurrentCamera() const;

	void GetVideoCardInfo(char*, int&) const;

	void SetBackBufferRenderTarget();
	void ResetViewport();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void EnableAlphaBlending();
	void DisableAlphaBlending();

private:
	Microsoft::WRL::ComPtr <IDXGISwapChain> m_swapChain;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaEnableBlendingState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaDisableBlendingState;

	Camera* m_Camera;

	XMMATRIX m_worldMatrix;

	D3D11_VIEWPORT m_viewport;

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
};

