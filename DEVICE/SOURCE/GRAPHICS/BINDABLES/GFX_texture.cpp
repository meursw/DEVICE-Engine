#pragma comment(lib, "dxguid.lib")

#include <fstream>
#include <string>

#include "GFX_texture.h"
#include "SYS_d3d_exception.h"

#include "WICTextureLoader11.h"

Texture::Texture(D3DClass* d3d, const std::wstring& path, HWND hwnd)
{
	std::ifstream fin(path);
	if (!fin)
	{
		std::wstring error = L"Could not load file: " + path;
		MessageBox(hwnd, error.c_str(), L"ERROR", MB_OK);
		throw DEVICE_Exception(__LINE__, __FILE__);
	}

	ID3D11Device* device = d3d->GetDevice();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();

	Microsoft::WRL::ComPtr<ID3D11Resource> resource = nullptr;
	
	HRESULT hr;

	D3D_THROW(DirectX::CreateWICTextureFromFile(
		device, 
		deviceContext,
		path.c_str(),
		resource.ReleaseAndGetAddressOf(),
		m_textureView.ReleaseAndGetAddressOf()
	));
}

void Texture::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->PSSetShaderResources(0u, 1u, m_textureView.GetAddressOf());
}
