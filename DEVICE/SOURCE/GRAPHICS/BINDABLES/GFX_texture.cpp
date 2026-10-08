#pragma comment(lib, "dxguid.lib")

#include <fstream>
#include <string>

#include "GFX_texture.h"
#include "GFX_bindablecodex.h"

#include "WICTextureLoader11.h"

static std::wstring StringToWString(const std::string& str)
{
	std::wstring wstr;
	size_t size;
	wstr.resize(str.length());
	mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
	return wstr;
}

Texture::Texture(D3DClass* d3d, const std::string& path, HWND hwnd, UINT slot)
	:
	m_path(path),
	m_slot(slot)
{
	const auto wpath = StringToWString(path);
	std::ifstream fin(path);
	if (!fin)
	{
		std::wstring error = L"Could not load file: " + wpath;
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
		wpath.c_str(),
		resource.ReleaseAndGetAddressOf(),
		m_textureView.ReleaseAndGetAddressOf()
	));
}

std::shared_ptr<Bindable> Texture::Resolve(D3DClass* d3d, const std::string& path, HWND hwnd, UINT slot)
{
	return BindableCodex::Resolve<Texture>(d3d, path, hwnd, slot);
}

std::string Texture::GenerateUID(const std::string& path, HWND hwnd, UINT slot)
{
	using namespace std::string_literals;
	return typeid(Texture).name() + "#"s + path + std::to_string(slot);
}

std::string Texture::GetUID() const
{
	return GenerateUID(m_path, nullptr, m_slot);
}

void Texture::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->PSSetShaderResources(m_slot, 1u, m_textureView.GetAddressOf());
}

