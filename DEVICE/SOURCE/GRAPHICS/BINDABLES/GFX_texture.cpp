#pragma comment(lib, "dxguid.lib")

#include <fstream>
#include <string>

#include "GFX_texture.h"
#include "GFX_bindablecodex.h"

#include "GFX_imageloader.h"

#include "WICTextureLoader11.h"

using namespace Microsoft::WRL;


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
	Microsoft::WRL::ComPtr<ID3D11Resource> resource = nullptr;

	HRESULT hr;

	D3D_THROW(DirectX::CreateWICTextureFromFileEx(
		d3d->GetDevice(),
		d3d->GetDeviceContext(),
		StringToWString(path).c_str(),
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		WIC_LOADER_IGNORE_SRGB,
		resource.ReleaseAndGetAddressOf(),
		m_textureView.ReleaseAndGetAddressOf()
	));

	/*LoadTexture(path, hwnd);

	HRESULT hr;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_imageData.width;
	textureDesc.Height = m_imageData.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_imageData.giPixelFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureResource;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = m_imageData.data.data();
	sd.SysMemPitch = m_imageData.width * ((m_imageData.bpp + 7) / 8);

	D3D_THROW(d3d->GetDevice()->CreateTexture2D(
		&textureDesc, &sd, textureResource.GetAddressOf()
	));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D_THROW(d3d->GetDevice()->CreateShaderResourceView(
		textureResource.Get(), &srvDesc, m_textureView.GetAddressOf()
	));*/

}

void Texture::LoadTexture(const std::string& path, HWND hwnd)
{
	std::ifstream fin(path);
	if (!fin)
	{
		std::string error = "Could not load file: " + path;
		MessageBoxA(hwnd, error.c_str(), "ERROR", MB_OK);
		throw DEVICE_Exception(__LINE__, __FILE__);
	}

	ImageLoader::LoadImageFromDisk(StringToWString(path), m_imageData);

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

