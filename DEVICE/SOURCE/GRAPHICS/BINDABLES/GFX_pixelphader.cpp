#include "GFX_pixelshader.h"

PixelShader::PixelShader(
	ShaderType shaderType,
	ID3D11Device* device,
	HWND hwnd,
	const wchar_t* shaderFilename,
	const char* shaderEntryPoint)
	:
	BaseShaderClass(shaderType, hwnd, shaderFilename, shaderEntryPoint)
{
	CreateShader(device);
}

void PixelShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	D3D_THROW(device->CreatePixelShader(
		m_shaderBuffer->GetBufferPointer(),
		m_shaderBuffer->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf()
	));
}

void PixelShader::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->PSSetShader(m_pixelShader.Get(), NULL, 0);
}